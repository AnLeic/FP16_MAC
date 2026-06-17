# Formal Verification Plan for FP16 MAC Unit

## Overview
This verification plan addresses the formal verification of the FP16 MAC unit implementing IEEE 754 compliance for AI accelerators. The design computes D = (A * B) + C where A,B are FP16 inputs and C,D are FP32 values.

## Assumptions

### Pipeline Operation Assumptions
- All pipeline stages operate synchronously with the clock
- Valid/Ready handshake protocol is correctly implemented across all stages
- Reset is asynchronous and active-low
- Pipeline stages are properly synchronized with valid/ready signals
- No structural hazards in pipeline execution

### IEEE 754 Compliance Assumptions  
- FP16 inputs are properly formatted according to IEEE 754 half-precision standard
- FP32 outputs maintain IEEE 754 single-precision compliance
- Rounding-to-nearest-even (RNE) mode is consistently applied throughout all operations
- Flush-to-Zero (FTZ) behavior correctly handles subnormal inputs

### Functional Assumptions
- Input data is valid when `a_valid`, `b_valid`, and `c_valid` are asserted
- The unit operates correctly under normal pipeline conditions
- Exception flags are properly propagated through the pipeline stages
- Output data is valid when `d_valid` is asserted and `d_ready` is accepted

## Assertions

### Pipeline Protocol Assertions
```systemverilog
// Input handshake protocol assertions
property p_input_protocol;
    @(posedge clk) disable iff (!rst_n)
    (a_valid && b_valid && c_valid) |-> (a_ready && b_ready && c_ready);
endproperty

assert property(p_input_protocol) 
    "Input handshake protocol must be maintained";

// Pipeline stage ready propagation assertions
property p_stage_ready_propagation;
    @(posedge clk) disable iff (!rst_n)
    (stage0_valid && !stage1_ready) |-> (stage0_ready == 1'b0);
endproperty

assert property(p_stage_ready_propagation)
    "Stage ready signals must propagate correctly";

// Pipeline enable assertions
property p_pipeline_enable;
    @(posedge clk) disable iff (!rst_n)
    (a_valid && b_valid && c_valid) |-> (pipe_enable == 1'b1);
endproperty

assert property(p_pipeline_enable)
    "Pipeline should enable only when inputs are valid";
```

### IEEE 754 Compliance Assertions
```systemverilog
// NaN propagation assertions
property p_nan_propagation;
    @(posedge clk) disable iff (!rst_n)
    (a_data[15:10] == 5'b11111 || b_data[15:10] == 5'b11111) |-> 
    (exception[2] == 1'b1);
endproperty

assert property(p_nan_propagation)
    "Invalid operation flag should be set for NaN inputs";

// Zero multiplication assertions
property p_zero_multiplication;
    @(posedge clk) disable iff (!rst_n)
    (a_data[15:10] == 5'b00000 && b_data[15:10] == 5'b00000) |-> 
    (stage1_product[31:23] == 9'b000000000);
endproperty

assert property(p_zero_multiplication)
    "Zero multiplication should result in zero";

// Overflow detection assertions
property p_overflow_detection;
    @(posedge clk) disable iff (!rst_n)
    (stage1_product[30:23] == 9'b111111111 || stage2_accum[30:23] == 9'b111111111) |-> 
    (exception[0] == 1'b1);
endproperty

assert property(p_overflow_detection)
    "Overflow flag should be set for overflow conditions";
```

### Exception Handling Assertions
```systemverilog
// Exception flag consistency assertions
property p_exception_consistency;
    @(posedge clk) disable iff (!rst_n)
    (stage0_exception[2] || stage1_exception[2] || stage2_exception[2]) |-> 
    (exception[2] == 1'b1);
endproperty

assert property(p_exception_consistency)
    "Invalid operation flag should propagate correctly";

// Underflow handling assertions
property p_underflow_handling;
    @(posedge clk) disable iff (!rst_n)
    (stage0_exception[1] || stage1_exception[1] || stage2_exception[1]) |-> 
    (exception[1] == 1'b1);
endproperty

assert property(p_underflow_handling)
    "Underflow flag should be set for underflow conditions";
```

### Data Integrity Assertions
```systemverilog
// Output data validity assertions
property p_output_validity;
    @(posedge clk) disable iff (!rst_n)
    (d_valid && d_ready) |-> (d_data != 32'h0);
endproperty

assert property(p_output_validity)
    "Valid output should contain meaningful data";

// Result computation assertions
property p_result_computation;
    @(posedge clk) disable iff (!rst_n)
    (stage2_valid) |-> (stage2_accum == stage1_product + stage0_c_preproc);
endproperty

assert property(p_result_computation)
    "Accumulation should compute correct result";
```

## Cover Points

### Special Value Coverage
```systemverilog
// Cover point for NaN inputs
covergroup cg_nan_inputs;
    cp_nan_a: coverpoint a_data[15:10] {
        bins nan = {5'b11111};
    }
    cp_nan_b: coverpoint b_data[15:10] {
        bins nan = {5'b11111};
    }
    cp_nan_combination: cross cp_nan_a, cp_nan_b;
endgroup

// Cover point for infinity inputs
covergroup cg_infinity_inputs;
    cp_inf_a: coverpoint a_data[15:10] {
        bins inf = {5'b11111};
    }
    cp_inf_b: coverpoint b_data[15:10] {
        bins inf = {5'b11111};
    }
    cp_inf_combination: cross cp_inf_a, cp_inf_b;
endgroup

// Cover point for zero inputs
covergroup cg_zero_inputs;
    cp_zero_a: coverpoint a_data[15:10] {
        bins zero = {5'b00000};
    }
    cp_zero_b: coverpoint b_data[15:10] {
        bins zero = {5'b00000};
    }
    cp_zero_combination: cross cp_zero_a, cp_zero_b;
endgroup

// Cover point for subnormal inputs
covergroup cg_subnormal_inputs;
    cp_subnorm_a: coverpoint a_data[15:10] {
        bins subnormal = {5'b00000};
    }
    cp_subnorm_b: coverpoint b_data[15:10] {
        bins subnormal = {5'b00000};
    }
    cp_subnorm_combination: cross cp_subnorm_a, cp_subnorm_b;
endgroup
```

### Pipeline Behavior Coverage
```systemverilog
// Cover point for pipeline stall conditions
covergroup cg_pipeline_stall;
    cp_a_stall: coverpoint a_valid && !a_ready;
    cp_b_stall: coverpoint b_valid && !b_ready;
    cp_c_stall: coverpoint c_valid && !c_ready;
    cp_full_stall: cross cp_a_stall, cp_b_stall, cp_c_stall;
endgroup

// Cover point for reset conditions
covergroup cg_reset_conditions;
    cp_reset_active: coverpoint !rst_n;
    cp_reset_deactive: coverpoint rst_n;
    cp_reset_transition: cross cp_reset_active, cp_reset_deactive;
endgroup

// Cover point for back-to-back operations
covergroup cg_back_to_back;
    cp_valid_cycle: coverpoint (a_valid && b_valid && c_valid);
    cp_consecutive_valid: coverpoint (a_valid && b_valid && c_valid) &&
                          (a_valid[1] && b_valid[1] && c_valid[1]);
endgroup
```

### Exception Condition Coverage
```systemverilog
// Cover point for exception flags
covergroup cg_exceptions;
    cp_overflow: coverpoint exception[0];
    cp_underflow: coverpoint exception[1];
    cp_invalid: coverpoint exception[2];
    cp_exception_combination: cross cp_overflow, cp_underflow, cp_invalid;
endgroup

// Cover point for operation types
covergroup cg_operations;
    cp_multiply: coverpoint stage1_valid;
    cp_addition: coverpoint stage2_valid;
    cp_result_formatting: coverpoint stage3_valid;
    cp_operation_combination: cross cp_multiply, cp_addition, cp_result_formatting;
endgroup

// Cover point for rounding modes
covergroup cg_rounding;
    cp_rne: coverpoint 1'b1; // Round-to-nearest-even
    cp_rn: coverpoint 1'b0;  // Round-to-nearest (other modes)
endgroup
```

## Critical Corner Cases

### Zero Multiplication
- **Case**: A = 0, B ≠ 0 or A ≠ 0, B = 0  
- **Expected**: Result should be 0
- **Verification**: Assert that zero inputs produce zero results in multiply stage

### NaN Propagation
- **Case**: A = NaN or B = NaN
- **Expected**: Result should be NaN, invalid flag set
- **Verification**: Monitor input exceptions and verify propagation through pipeline

### Infinity Operations  
- **Case**: ∞ × 0, ∞ + (-∞), ∞ × ∞
- **Expected**: Special values handled according to IEEE 754
- **Verification**: Test all infinity combinations and check exception flags

### Subnormal Handling
- **Case**: FP16 subnormal inputs
- **Expected**: Inputs converted to zero, underflow flag set
- **Verification**: Verify FTZ behavior in input stage

### Back-to-Back Pipeline Stalls
- **Case**: Multiple consecutive stalls during pipeline operation
- **Expected**: Pipeline should recover correctly without data corruption
- **Verification**: Monitor pipeline ready signals and ensure proper recovery

### Reset During Valid Data
- **Case**: Reset asserted while valid data is present
- **Expected**: Pipeline should flush cleanly, no partial data processing
- **Verification**: Check that reset clears all pipeline stages properly

## Verification Metrics

### Coverage Targets
- **Exception coverage**: 100%
- **Pipeline stall coverage**: 95%  
- **Special value coverage**: 100%
- **Operation combination coverage**: 90%

### Assertion Coverage
- **Protocol assertions**: 100%
- **IEEE compliance assertions**: 100%
- **Data integrity assertions**: 100%
- **Exception handling assertions**: 100%

### Formal Verification Boundaries
- **Pipeline depth**: 4 stages (bounded)
- **Signal propagation**: Clock cycle bounded
- **Reset behavior**: Asynchronous reset with bounded recovery time
- **Exception propagation**: Stage-bound (3 stages maximum)

## Testbench Requirements

### Stimulus Generation
- Random test vectors for all special values
- Edge case combinations (zero, infinity, NaN)
- Pipeline stall sequences
- Reset scenarios during operation
- Back-to-back valid data patterns

### Coverage Monitoring
- Continuous monitoring of all cover points
- Automated coverage reporting
- Coverage closure verification
- Assertion result logging

This formal verification plan ensures comprehensive testing of the FP16 MAC unit's IEEE 754 compliance, pipeline behavior, and exception handling under all critical corner cases.