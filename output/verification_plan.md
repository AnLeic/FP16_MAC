

# Formal Verification Plan for FP16 MAC Unit

## 1. Introduction
This document outlines a formal verification plan for the FP16 MAC unit, ensuring compliance with the specified requirements. The goal is to validate correct functionality across various corner cases and edge scenarios.

## 2. Corner Cases and Verification Requirements

### 2.1 Multiplying by Zero
- **Assertion**: When A or B is zero (including subnormal), product_fp16 should be zero.
- **Assumption**: Subnormals are flushed to zero in Stage 1.
- **Cover Points**:
  - A = 0, B arbitrary.
  - B = 0, A arbitrary.
  - Both A and B = 0.

### 2.2 NaN Propagation
- **Assertion**: If any input is NaN, D should be NaN.
- **Assumption**: Correct detection of NaNs in inputs.
- **Cover Points**:
  - A is NaN.
  - B is NaN.
  - C is NaN.

### 2.3 Extreme Exponents
- **Assertion**: Overflow/underflow correctly flagged during multiplication and addition.
- **Assumption**: Proper handling of exponent shifts during FP16 to FP32 conversion.
- **Cover Points**:
  - Maximum FP16 values multiplied.
  - Minimum FP16 products causing underflow.
  - Addition resulting in overflow or underflow.

### 2.4 Pipeline Stalls
- **Assertion**: Correct behavior when Ready_in is deasserted.
- **Assumption**: No data loss during stalls.
- **Cover Points**:
  - Stall at each pipeline stage.
  - Resuming after stall without data corruption.

### 2.5 Reset During Valid Data
- **Assertion**: Outputs reset to initial state post-reset.
- **Assumption**: All internal states are cleared on reset.
- **Cover Points**:
  - Reset during processing at each stage.
  - Valid_out deasserted until new valid data processed.

## 3. Exception Handling Verification
- **Overflow Detection**: Check if product exceeds max FP16 or sum exceeds max FP32.
- **Underflow Detection**: Check for products too small for FP16.
- **NaN Handling**: Ensure propagation and correct exception flagging.

## 4. Pipeline Efficiency
- **Assertion**: No stalls under normal operation.
- **Assumption**: Ready_in correctly managed across stages.
- **Cover Points**:
  - Continuous data flow without unnecessary stalls.
  - Proper synchronization between pipeline stages.

## 5. Timing Validation
- **Assertion**: Design meets frequency targets.
- **Assumption**: Synthesis tooling adheres to timing constraints.
- **Cover Points**:
  - Clock cycle measurements for each stage transition.
  - Ensuring no critical path exceeds target frequency.

## 6. Conclusion
This verification plan ensures comprehensive testing of the FP16 MAC unit across all identified corner cases, guaranteeing robust and compliant functionality as per specifications.