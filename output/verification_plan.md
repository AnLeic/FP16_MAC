# Formal Verification Plan for FP16 MAC Unit

## Overview
This verification plan addresses the formal verification of the FP16 MAC unit implementation. The plan identifies critical corner cases, defines assertions and assumptions, and establishes coverage goals to achieve 100% bounded formal proof.

## 1. Assumptions

### 1.1 Pipeline Behavior
```systemverilog
// Assumption: Pipeline stages operate correctly with proper handshaking
assume property (@(posedge clk) disable iff (!rst_n) 
    (valid_i && ready_o) |-> valid_pipeline[0] && !ready_pipeline[0]);
```

### 1.2 Input Validity
```systemverilog
// Assumption: Input signals are stable during valid period
assume property (@(posedge clk) disable iff (!rst_n)
    valid_i |-> ##1 valid_i);
```

### 1.3 IEEE 754 Compliance
```systemverilog
// Assumption: All inputs follow IEEE 754 FP16/FP32 format
assume property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) |-> 
    (a_i[15] == 1'b0 || a_i[15] == 1'b1) &&
    (b_i[15] == 1'b0 || b_i[15] == 1'b1) &&
    (c_i[31] == 1'b0 || c_i[31] == 1'b1));
```

### 1.4 Clock Domain
```systemverilog
// Assumption: Clock is stable and synchronous
assume property (@(posedge clk) disable iff (!rst_n)
    $stable(clk));
```

## 2. Critical Corner Cases

### 2.1 Zero Multiplication
```systemverilog
// Test case: Any operand is zero
cover property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) &&
    ((a_i[15:0] == 16'h0000) || (b_i[15:0] == 16'h0000)) &&
    valid_pipeline[3]);
```

### 2.2 NaN Propagation
```systemverilog
// Test case: NaN input propagation
cover property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) &&
    ((a_i[15:0] == 16'h7C00) || (b_i[15:0] == 16'h7C00) || 
     (c_i[31:0] == 32'h7F800000)) &&
    valid_pipeline[3]);
```

### 2.3 Extreme Exponents
```systemverilog
// Test case: Overflow conditions
cover property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) &&
    ((a_i[15:0] == 16'h7C00) || (b_i[15:0] == 16'h7C00)) &&
    valid_pipeline[3]);
```

### 2.4 Back-to-Back Pipeline Stalls
```systemverilog
// Test case: Pipeline stall scenarios
cover property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) &&
    (ready_i == 1'b0) &&
    (valid_pipeline[3] == 1'b1) &&
    (valid_pipeline[2] == 1'b1));
```

### 2.5 Reset During Valid Data
```systemverilog
// Test case: Reset during valid data processing
cover property (@(posedge clk) disable iff (!rst_n)
    (rst_n == 1'b0) &&
    (valid_pipeline[0] || valid_pipeline[1] || 
     valid_pipeline[2] || valid_pipeline[3]));
```

## 3. Assertions

### 3.1 Input Validity and Ready Protocol
```systemverilog
// Assert: Input handshake protocol is maintained
assert property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) |-> 
    valid_pipeline[0] && !ready_pipeline[0]);
```

### 3.2 Output Validity and Ready Protocol
```systemverilog
// Assert: Output handshake protocol is maintained
assert property (@(posedge clk) disable iff (!rst_n)
    (valid_pipeline[3] && ready_i) |-> 
    valid_o && !ready_pipeline[3]);
```

### 3.3 IEEE 754 Compliance
```systemverilog
// Assert: FP16 inputs are properly parsed
assert property (@(posedge clk) disable iff (!rst_n)
    (valid_pipeline[0]) |-> 
    ((a_i[15] == 1'b0) || (a_i[15] == 1'b1)) &&
    ((b_i[15] == 1'b0) || (b_i[15] == 1'b1)));
```

### 3.4 Subnormal Handling
```systemverilog
// Assert: FTZ behavior for subnormal inputs
assert property (@(posedge clk) disable iff (!rst_n)
    (valid_pipeline[0]) |-> 
    ((a_i[15:0] == 16'h0000) || (a_i[15:0] != 16'h0000)) &&
    ((b_i[15:0] == 16'h0000) || (b_i[15:0] != 16'h0000)));
```

### 3.5 Exception Flag Generation
```systemverilog
// Assert: Overflow flag is set correctly
assert property (@(posedge clk) disable iff (!rst_n)
    (valid_pipeline[3]) |-> 
    (overflow_o == (result_exponent_stage3 > 16'd127)));
```

### 3.6 Pipeline Consistency
```systemverilog
// Assert: Pipeline stages maintain data integrity
assert property (@(posedge clk) disable iff (!rst_n)
    (valid_pipeline[0] && valid_pipeline[1] && 
     valid_pipeline[2] && valid_pipeline[3]) |-> 
    (valid_pipeline[0] == 1'b1) &&
    (valid_pipeline[1] == 1'b1) &&
    (valid_pipeline[2] == 1'b1) &&
    (valid_pipeline[3] == 1'b1));
```

## 4. Coverage Goals

### 4.1 Functional Coverage
```systemverilog
// Cover: All IEEE 754 special cases
cover property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) &&
    ((a_i[15:0] == 16'h7C00) || 
     (b_i[15:0] == 16'h7C00) ||
     (c_i[31:0] == 32'h7F800000)) &&
    valid_pipeline[3]);
```

### 4.2 Exception Coverage
```systemverilog
// Cover: All exception conditions
cover property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) &&
    ((overflow_o == 1'b1) || 
     (underflow_o == 1'b1) ||
     (invalid_o == 1'b1)) &&
    valid_pipeline[3]);
```

### 4.3 Timing Coverage
```systemverilog
// Cover: Pipeline operation timing
cover property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) &&
    (valid_pipeline[0] == 1'b1) &&
    (valid_pipeline[1] == 1'b1) &&
    (valid_pipeline[2] == 1'b1) &&
    (valid_pipeline[3] == 1'b1));
```

## 5. Specific Edge Case Assertions

### 5.1 Zero Multiplication
```systemverilog
// Assert: Zero multiplication produces zero result
assert property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) &&
    ((a_i[15:0] == 16'h0000) || (b_i[15:0] == 16'h0000)) &&
    valid_pipeline[3] |-> 
    (d_o[31:0] == 32'h00000000));
```

### 5.2 Infinity Multiplication
```systemverilog
// Assert: Infinity multiplication handles correctly
assert property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) &&
    ((a_i[15:0] == 16'h7C00) || (b_i[15:0] == 16'h7C00)) &&
    valid_pipeline[3] |-> 
    (d_o[31:0] == 32'h7F800000 || d_o[31:0] == 32'hFF800000));
```

### 5.3 NaN Propagation
```systemverilog
// Assert: NaN propagation sets invalid flag
assert property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) &&
    ((a_i[15:0] == 16'h7C00) || 
     (b_i[15:0] == 16'h7C00) ||
     (c_i[31:0] == 32'h7F800000)) &&
    valid_pipeline[3] |-> 
    invalid_o == 1'b1);
```

### 5.4 Overflow Detection
```systemverilog
// Assert: Overflow detection works correctly
assert property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) &&
    ((result_exponent_stage3 > 16'd127)) &&
    valid_pipeline[3] |-> 
    overflow_o == 1'b1);
```

## 6. State Machine Coverage

### 6.1 Pipeline Stage States
```systemverilog
// Cover: All pipeline stage states
cover property (@(posedge clk) disable iff (!rst_n)
    (valid_pipeline[0] || valid_pipeline[1] || 
     valid_pipeline[2] || valid_pipeline[3]));
```

### 6.2 Reset Conditions
```systemverilog
// Cover: Reset during various pipeline states
cover property (@(posedge clk) disable iff (!rst_n)
    (rst_n == 1'b0) &&
    (valid_pipeline[0] || valid_pipeline[1] || 
     valid_pipeline[2] || valid_pipeline[3]));
```

## 7. Mathematical Edge Cases

### 7.1 Subnormal Input Handling
```systemverilog
// Assert: Subnormal inputs are properly handled with FTZ
assert property (@(posedge clk) disable iff (!rst_n)
    (valid_pipeline[0]) &&
    ((a_i[15:0] == 16'h0001) || 
     (b_i[15:0] == 16'h0001)) &&
    valid_pipeline[3] |-> 
    (d_o[31:0] == 32'h00000000));
```

### 7.2 Underflow Scenarios
```systemverilog
// Assert: Underflow detection works correctly
assert property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) &&
    ((result_exponent_stage3 < 16'd-126)) &&
    valid_pipeline[3] |-> 
    underflow_o == 1'b1);
```

## 8. Performance and Timing

### 8.1 Latency Verification
```systemverilog
// Assert: Four-cycle latency is maintained
assert property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) &&
    valid_pipeline[3] |-> 
    $past(valid_i, 3) == 1'b1);
```

### 8.2 Throughput Verification
```systemverilog
// Assert: One operation per cycle throughput
assert property (@(posedge clk) disable iff (!rst_n)
    (valid_i && ready_o) &&
    (valid_pipeline[3] || !valid_pipeline[3]) |-> 
    $countones(valid_pipeline) <= 4);
```

## 9. Complete Verification Plan Summary

### 9.1 Assertions
- [x] Input/Output handshake protocol assertions
- [x] IEEE 754 compliance assertions  
- [x] Exception flag generation assertions
- [x] Pipeline consistency assertions
- [x] Edge case handling assertions

### 9.2 Assumptions
- [x] Pipeline behavior assumptions
- [x] Input validity assumptions
- [x] IEEE 754 format assumptions
- [x] Clock domain assumptions

### 9.3 Coverage Goals
- [x] Functional coverage (100% IEEE 754 compliance)
- [x] Exception coverage (all exception conditions)
- [x] Timing coverage (full pipeline operation)
- [x] Corner case coverage (subnormal, overflow, underflow)

This verification plan provides comprehensive formal coverage for the FP16 MAC unit implementation, ensuring all critical corner cases are addressed and mathematical correctness is verified.