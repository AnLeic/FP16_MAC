# FP16 MAC Unit Specification

## 1. Overview

This document specifies the microarchitectural design of a high-performance FP16 Multiply-Accumulate (MAC) unit for AI accelerators. The unit computes D = (A * B) + C where A and B are IEEE 754 half-precision (FP16) inputs, C and D are IEEE 754 single-precision (FP32) values.

## 2. Functional Specifications

### 2.1 Operation
The MAC unit performs the following computation:
```
D = (A * B) + C
```
Where:
- A, B: IEEE 754 FP16 inputs
- C, D: IEEE 754 FP32 values

### 2.2 Input/Output Interface

#### 2.2.1 Input Interface
| Signal | Width | Direction | Description |
|--------|-------|-----------|-------------|
| `a_valid` | 1 | Input | Valid signal for input A |
| `a_ready` | 1 | Output | Ready signal for input A |
| `a_data` | 16 | Input | FP16 value A |
| `b_valid` | 1 | Input | Valid signal for input B |
| `b_ready` | 1 | Output | Ready signal for input B |
| `b_data` | 16 | Input | FP16 value B |
| `c_valid` | 1 | Input | Valid signal for input C |
| `c_ready` | 1 | Output | Ready signal for input C |
| `c_data` | 32 | Input | FP32 value C |

#### 2.2.2 Output Interface
| Signal | Width | Direction | Description |
|--------|-------|-----------|-------------|
| `d_valid` | 1 | Output | Valid signal for output D |
| `d_ready` | 1 | Input | Ready signal for output D |
| `d_data` | 32 | Output | FP32 result D |
| `exception` | 3 | Output | Exception flags [Overflow, Underflow, Invalid] |

### 2.3 Protocol
The MAC unit implements a standard Valid/Ready handshake protocol:
- Inputs are accepted only when both `a_ready` and `b_ready` are asserted
- The unit asserts `c_ready` when it is ready to accept input C
- Output D is valid when `d_valid` is asserted
- All signals are synchronous to the clock (posedge)

## 3. Pipeline Architecture

### 3.1 Pipeline Stages

#### Stage 0: Input Latch and Preprocessing
- **Inputs**: a_data, b_data, c_data
- **Processing**:
  - Validate FP16 inputs for NaN/Infinity
  - Handle subnormal inputs with FTZ (Flush-to-Zero)
  - Extract sign, exponent, and mantissa fields
  - Normalize inputs to standard FP32 format
- **Outputs**: Preprocessed operands in FP32 format

#### Stage 1: Multiply Operation
- **Inputs**: Preprocessed A, B (FP32)
- **Processing**:
  - Perform FP32 multiplication
  - Apply rounding-to-nearest-even for intermediate result
  - Handle overflow/underflow detection during multiplication
- **Outputs**: Intermediate product in FP32 format

#### Stage 2: Accumulate Operation
- **Inputs**: Product (from stage 1), C (FP32)
- **Processing**:
  - Perform FP32 addition of product and C
  - Apply rounding-to-nearest-even for final result
  - Detect overflow/underflow conditions
  - Check for invalid operations (NaN propagation)
- **Outputs**: Final accumulated result

#### Stage 3: Output Formatting and Exception Handling
- **Inputs**: Accumulated result, exception flags from previous stages
- **Processing**:
  - Format final result according to IEEE 754 FP32
  - Generate exception flags for overflow, underflow, invalid operations
  - Apply FTZ for subnormal results if needed
  - Final rounding and normalization
- **Outputs**: Final D value, exception flags

### 3.2 Pipeline Depth
The design implements a 4-stage pipeline to maximize clock frequency:
- Stage 0: Input processing (1 cycle)
- Stage 1: Multiply operation (1 cycle)  
- Stage 2: Accumulate operation (1 cycle)
- Stage 3: Output formatting (1 cycle)

## 4. IEEE 754 Compliance

### 4.1 Format Support
- **Input A, B**: IEEE 754 half-precision (FP16)
- **Input C, Output D**: IEEE 754 single-precision (FP32)

### 4.2 Rounding Mode
All intermediate and final computations use Round-to-Nearest-Even (RNE) mode:
- During multiplication: RNE for intermediate product
- During accumulation: RNE for final result
- For truncation operations: RNE rounding

### 4.3 Underflow Handling
Subnormal inputs are handled via Flush-to-Zero (FTZ):
- Subnormal FP16 inputs are converted to zero
- Exception flag is set for underflow condition
- All subnormal results are flushed to zero in final output

## 5. Exception Handling

### 5.1 Exception Flags
The MAC unit generates three exception flags:
| Flag | Meaning |
|------|---------|
| `overflow` | Arithmetic overflow occurred |
| `underflow` | Arithmetic underflow occurred |
| `invalid` | Invalid operation detected (NaN propagation) |

### 5.2 Exception Detection

#### Overflow Detection
- **Multiply Stage**: Detect when product magnitude exceeds FP32 maximum
- **Accumulate Stage**: Detect when sum magnitude exceeds FP32 maximum
- **Flags**: Set `overflow` = 1 when overflow occurs

#### Underflow Detection  
- **Input Stage**: Detect subnormal inputs and flag with FTZ
- **Multiply Stage**: Detect when product magnitude is below FP32 minimum
- **Accumulate Stage**: Detect when sum magnitude is below FP32 minimum
- **Flags**: Set `underflow` = 1 when underflow occurs

#### Invalid Operation Detection
- **NaN Propagation**: If either operand A or B is NaN, result is NaN
- **Infinity Operations**: 
  - ∞ × 0 = NaN
  - ∞ + (-∞) = NaN  
  - 0 × ∞ = NaN
- **Flags**: Set `invalid` = 1 for all invalid operations

## 6. Mathematical Edge Cases

### 6.1 Special Values Handling

#### Zero Operations
- 0 × x = 0 (for any finite x)
- 0 + x = x (for any finite x)
- 0 × 0 = 0

#### Infinity Operations
- ∞ × ∞ = ∞
- ∞ + ∞ = ∞
- ∞ × 0 = NaN
- ∞ + (-∞) = NaN

#### NaN Operations
- Any operation involving NaN results in NaN
- NaN propagation through all pipeline stages

### 6.2 Subnormal Handling
- Subnormal FP16 inputs are converted to zero at input stage
- Subnormal intermediate results are flushed to zero
- Exception flags are set for underflow conditions

### 6.3 Precision Considerations
- Intermediate computations maintain full FP32 precision
- Final rounding applies RNE to ensure IEEE compliance
- Pipeline stages maintain consistent precision throughout

## 7. Performance Characteristics

### 7.1 Throughput
- **Latency**: 4 cycles (pipeline depth)
- **Throughput**: 1 MAC operation per cycle (when pipeline is full)
- **Cycle Time**: Optimized for maximum frequency operation

### 7.2 Resource Usage
- **Area**: Minimal hardware footprint optimized for AI accelerators
- **Power**: Low power consumption with efficient pipeline structure
- **Clock Frequency**: Target > 1 GHz for high-performance AI applications

## 8. Implementation Notes

### 8.1 Pipeline Optimization
- All stages designed for maximum clock frequency
- Critical path minimized through careful logic optimization
- No structural hazards in pipeline execution

### 8.2 Exception Handling
- Exception flags are generated at each stage and propagated forward
- Final exception output is synchronized with result data
- Exception handling does not add pipeline latency

### 8.3 Compatibility
- Fully compliant with IEEE 754-2008 standards
- Supports mixed-precision operations as required for AI accelerators
- Compatible with existing AI accelerator architectures

## 9. Testability

### 9.1 Coverage
- All special value combinations tested
- Exception conditions verified
- Edge case scenarios validated
- Pipeline behavior confirmed across all stages

### 9.2 Verification Approach
- Formal verification of IEEE compliance
- Monte Carlo testing with random inputs
- Edge case validation with known test vectors
- Performance benchmarking against target specifications