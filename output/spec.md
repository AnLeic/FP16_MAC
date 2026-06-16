

# High-Performance FP16 Multiply-Accumulate (MAC) Unit Specification Document

## 1. Introduction
This document outlines the specifications for an AI accelerator MAC unit designed to perform mixed-precision operations. The operation is defined as D = (A * B) + C, where A and B are IEEE 754 FP16 numbers, and C and D are IEEE 754 FP32 numbers.

## 2. Interface Specification
### 2.1 Input Ports
- **A**: FP16 input operand.
- **B**: FP16 input operand.
- **C**: FP32 input operand.
- **Valid_in**: Indicates valid data on A, B, and C.
  
### 2.2 Output Port
- **D**: FP32 output result.
- **Valid_out**: Indicates valid data on D.

### 2.3 Control Signals
- **Ready_in**: Acknowledgment from the MAC unit for input data.
- **Ready_out**: Acknowledgment to the next stage that the result is available.

## 3. Pipeline Stages
The MAC unit is pipelined into four stages for maximum frequency:

1. **Stage 1: Input Handling and Flush-to-Zero (FTZ)**
   - Check if A or B are subnormal.
   - Flush subnormals to zero as per FTZ.

2. **Stage 2: Multiply Operation**
   - Perform FP16 multiplication of A and B.
   - Handle intermediate results without rounding yet.

3. **Stage 3: Add Operation with C**
   - Convert the FP16 product to FP32, applying RNE rounding.
   - Add the converted result to C (FP32).
   - Check for overflow/underflow during addition.

4. **Stage 4: Final Rounding and Exception Handling**
   - Apply RNE rounding to the sum.
   - Detect exceptions (Overflow, Underflow, NaNs).
   - Format D as FP32 with correct rounding.

## 4. Mathematical Edge Cases
- **Underflow**: Subnormal A or B are flushed to zero.
- **Overflow**: Detected during multiplication (FP16) and addition (FP32).
- **NaN Handling**: Propagate NaN through the pipeline, resulting in D as NaN.

## 5. Implementation Details
- **Multiplier Unit**: Optimized for FP16 operations.
- **Conversion Logic**: Handles exponent shift and mantissa extension from FP16 to FP32.
- **Rounding Logic**: Precise RNE rounding at both stages (multiply result conversion and final sum).

## 6. Exception Handling
- **Overflow**: Flagged if the product exceeds max FP16 or sum exceeds max FP32.
- **Underflow**: Flagged if the product is too small for FP16.
- **Invalid Operations**: NaN propagation detected and flagged.

## 7. Verification Plan
- **Test Cases**: Include normal, subnormal, special values (NaNs, infinities).
- **Exception Checks**: Ensure correct flagging of exceptions.
- **Pipeline Efficiency**: Validate no stalls under normal operation.
- **Timing Validation**: Confirm meeting frequency targets.

This specification ensures the MAC unit is robust, efficient, and compliant with IEEE 754 standards for mixed-precision AI acceleration.