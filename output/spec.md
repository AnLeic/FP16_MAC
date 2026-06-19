# FP16 MAC Unit Specification

## 1. Overview

This document specifies the microarchitectural and functional design of a high-performance FP16 Multiply-Accumulate (MAC) unit for AI accelerators. The unit computes D = (A * B) + C with mixed-precision inputs and outputs, implementing IEEE 754 compliance with specific handling for edge cases.

## 2. Functional Specification

### 2.1 Operation
The MAC unit performs the operation: **D = (A × B) + C**
- **Inputs**: A, B (FP16), C (FP32)
- **Output**: D (FP32)
- **Precision**: Mixed-precision computation with FP16 inputs and FP32 output

### 2.2 Input/Output Interface
```verilog
input  logic clk;
input  logic rst_n;

// Input valid/ready protocol
input  logic [15:0] a_i;     // FP16 input A
input  logic [15:0] b_i;     // FP16 input B  
input  logic [31:0] c_i;     // FP32 input C
input  logic valid_i;        // Input valid signal
output logic ready_o;        // Input ready signal

// Output valid/ready protocol
output logic [31:0] d_o;     // FP32 output D
output logic valid_o;        // Output valid signal
input  logic ready_i;        // Output ready signal

// Exception flags
output logic overflow_o;
output logic underflow_o;
output logic invalid_o;
```

### 2.3 Exception Handling
- **Overflow**: When result exceeds FP32 maximum representable value
- **Underflow**: When result is subnormal and FTZ is enabled
- **Invalid Operation**: NaN inputs or operations producing NaN results

## 3. Pipeline Architecture

### 3.1 Pipeline Stages (4-stage)
```
Stage 0: Input Processing & Conversion
Stage 1: Multiplication
Stage 2: Addition
Stage 3: Rounding & Output Formatting
```

### 3.2 Stage Details

#### Stage 0: Input Processing & Conversion
- **Function**: Convert FP16 inputs to internal extended precision
- **Operations**:
  - Parse FP16 inputs A and B
  - Handle subnormal inputs with FTZ
  - Convert to extended precision (48-bit mantissa + 16-bit exponent)
  - Validate input formats

#### Stage 1: Multiplication
- **Function**: Perform FP16 × FP16 multiplication
- **Operations**:
  - Multiply significands (48-bit × 48-bit = 96-bit result)
  - Add exponents with bias adjustment
  - Handle special cases (zero, infinity, NaN)
  - Normalize intermediate result

#### Stage 2: Addition
- **Function**: Add FP32 accumulator C to multiplication result
- **Operations**:
  - Align exponents of multiplication result and C
  - Perform addition/subtraction of significands
  - Handle exponent overflow/underflow
  - Maintain extended precision during computation

#### Stage 3: Rounding & Output Formatting
- **Function**: Round to FP32 and format output
- **Operations**:
  - Apply Round-to-Nearest-Even (RNE) rounding
  - Convert extended precision result to FP32 format
  - Generate exception flags
  - Format final IEEE 754 FP32 output

## 4. IEEE 754 Compliance

### 4.1 Rounding Mode
- **Round-to-Nearest-Even (RNE)** applied in:
  - Normalization phase
  - Truncation for FP32 conversion
  - Intermediate precision handling

### 4.2 Subnormal Handling
- **Flush-to-Zero (FTZ) approach**:
  - All subnormal inputs are treated as zero
  - Subnormal outputs are flushed to zero
  - Exception flag set when subnormal input detected

### 4.3 Special Cases
| Input Combination | Output Behavior |
|-------------------|-----------------|
| Any operand NaN   | Result = NaN, Invalid flag set |
| Infinity × Zero   | Result = NaN, Invalid flag set |
| Infinity × Infinity | Result = Infinity with sign |
| Zero × Infinity   | Result = NaN, Invalid flag set |

## 5. Exception Detection and Flagging

### 5.1 Exception Flags
```verilog
output logic overflow_o;    // Set when result overflows FP32 range
output logic underflow_o;   // Set when result underflows FP32 range  
output logic invalid_o;     // Set for invalid operations (NaN, inf×0, etc.)
```

### 5.2 Detection Logic

#### Overflow Detection
- **Condition**: Result exponent > 127 (FP32 max exponent)
- **Action**: Set overflow_o flag, output infinity with appropriate sign

#### Underflow Detection  
- **Condition**: Result exponent < -126 (FP32 min normal exponent)
- **Action**: Set underflow_o flag, flush to zero if FTZ enabled

#### Invalid Operation Detection
- **NaN inputs**: Any operand is NaN → set invalid_o
- **Indeterminate forms**: 0 × ∞, ∞ ÷ ∞, etc. → set invalid_o
- **Invalid operations**: Division by zero (non-NaN) → set invalid_o

## 6. Timing and Performance

### 6.1 Clock Frequency
- **Target**: Maximum frequency of 1 GHz
- **Pipeline Depth**: 4 stages
- **Latency**: 4 cycles from valid input to valid output

### 6.2 Throughput
- **Issue Rate**: 1 MAC operation per cycle
- **Completion Rate**: 1 result per cycle (after 4-cycle latency)

## 7. Mathematical Edge Cases

### 7.1 Subnormal Input Handling
```c
if (input_is_subnormal) {
    input_value = 0.0;  // FTZ: flush to zero
    underflow_flag = 1;
}
```

### 7.2 Overflow Scenarios
- **Positive overflow**: Result > +3.4028235e+38
- **Negative overflow**: Result < -3.4028235e+38  
- **Output**: ±∞ with appropriate sign

### 7.3 Underflow Scenarios
- **Subnormal results**: Exponent < -126
- **FTZ behavior**: Results flushed to zero
- **Exception flag**: Set for subnormal input detection

## 8. Implementation Constraints

### 8.1 Hardware Resources
- **Logic gates**: Optimized for high-frequency operation
- **Memory**: Minimal internal registers (4 pipeline stages)
- **Multipliers**: 48×48 bit multiplier for intermediate computation
- **Adders**: Extended precision adders for accumulation phase

### 8.2 Power Considerations
- **Dynamic power**: Minimized through pipelining and clock gating
- **Static power**: Optimized gate sizing for target frequency
- **Clock gating**: Enabled at pipeline stage boundaries

### 8.3 Area Optimization
- **Critical path**: Minimize propagation delay through key logic
- **Register usage**: 4-stage pipeline with minimal register overhead
- **Reusability**: Modular design supporting multiple MAC units in array

## 9. Verification Plan

### 9.1 Test Vectors
- Standard arithmetic operations
- Edge case inputs (zero, infinity, NaN)
- Subnormal number handling
- Overflow/underflow conditions
- Rounding behavior verification

### 9.2 Coverage Metrics
- **Functional coverage**: 100% IEEE 754 compliance
- **Exception coverage**: All exception conditions tested
- **Timing coverage**: Full pipeline operation verified
- **Corner case coverage**: Subnormal, overflow, underflow scenarios

## 10. Summary

This FP16 MAC unit specification defines a high-performance mixed-precision compute block optimized for AI accelerator applications. The design implements IEEE 754 compliance with FTZ handling, RNE rounding, and comprehensive exception detection while maintaining optimal pipeline depth for maximum frequency operation.