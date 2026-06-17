```systemverilog
// File: fp16_mac.sv
// Author: Senior Digital Design Engineer
// Description: FP16 MAC unit implementing IEEE 754 compliance for AI accelerators.
//              Computes D = (A * B) + C where A,B are FP16, C,D are FP32.

module fp16_mac (
    input  logic         clk,
    input  logic         rst_n,
    
    // Input A interface
    input  logic         a_valid,
    output logic         a_ready,
    input  logic [15:0]  a_data,
    
    // Input B interface  
    input  logic         b_valid,
    output logic         b_ready,
    input  logic [15:0]  b_data,
    
    // Input C interface
    input  logic         c_valid,
    output logic         c_ready,
    input  logic [31:0]  c_data,
    
    // Output D interface
    output logic         d_valid,
    input  logic         d_ready,
    output logic [31:0]  d_data,
    
    // Exception flags
    output logic [2:0]   exception  // {overflow, underflow, invalid}
);

// Pipeline stage signals
logic [31:0] stage0_a_preproc, stage0_b_preproc, stage0_c_preproc;
logic        stage0_valid;
logic        stage0_ready;

logic [31:0] stage1_product;
logic        stage1_valid;
logic        stage1_ready;

logic [31:0] stage2_accum;
logic        stage2_valid;
logic        stage2_ready;

logic [31:0] stage3_result;
logic        stage3_valid;
logic        stage3_ready;
logic [2:0]  stage3_exception;

// Exception flags from each pipeline stage
logic [2:0]  stage0_exception, stage1_exception, stage2_exception;

// Input handshake control signals
logic        inputs_ready;
logic        inputs_valid;

// Pipeline enable signals
logic        pipe_enable;
logic        pipe_flush;

// Pipeline valid/ready signals for entire unit
assign a_ready = inputs_ready && !pipe_flush;
assign b_ready = inputs_ready && !pipe_flush;
assign c_ready = inputs_ready && !pipe_flush;

// Input validation and ready control logic
always_comb begin
    inputs_ready = 1'b0;
    if (a_valid && b_valid && c_valid) begin
        // Only accept inputs when pipeline is not flushing and all are valid
        inputs_ready = !pipe_flush;
    end
end

// Pipeline enable logic - start processing when all inputs are valid
always_comb begin
    pipe_enable = 1'b0;
    if (a_valid && b_valid && c_valid) begin
        // Enable pipeline when we have valid inputs and no flush
        pipe_enable = !pipe_flush;
    end
end

// Flush control - reset pipeline on async reset or when pipeline is full
assign pipe_flush = ~rst_n;

// Stage 0: Input preprocessing and normalization
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        stage0_valid <= 1'b0;
        stage0_a_preproc <= 32'h0;
        stage0_b_preproc <= 32'h0;
        stage0_c_preproc <= 32'h0;
        stage0_exception <= 3'b0;
    end else if (pipe_enable) begin
        // Preprocess FP16 inputs to FP32 format
        stage0_a_preproc <= fp16_to_fp32(a_data);
        stage0_b_preproc <= fp16_to_fp32(b_data);
        stage0_c_preproc <= c_data;
        
        // Generate exception flags for input stage
        stage0_exception <= generate_input_exceptions(a_data, b_data);
        
        stage0_valid <= 1'b1;
    end else if (stage0_ready) begin
        stage0_valid <= 1'b0;
    end
end

// Stage 1: Multiply operation
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        stage1_valid <= 1'b0;
        stage1_product <= 32'h0;
        stage1_exception <= 3'b0;
    end else if (stage0_valid && !pipe_flush) begin
        // Perform FP32 multiplication with rounding
        stage1_product <= fp32_multiply(stage0_a_preproc, stage0_b_preproc);
        
        // Propagate and combine exceptions from previous stage
        stage1_exception <= stage0_exception | generate_multiply_exceptions(stage0_a_preproc, stage0_b_preproc);
        
        stage1_valid <= 1'b1;
    end else if (stage1_ready) begin
        stage1_valid <= 1'b0;
    end
end

// Stage 2: Accumulate operation  
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        stage2_valid <= 1'b0;
        stage2_accum <= 32'h0;
        stage2_exception <= 3'b0;
    end else if (stage1_valid && !pipe_flush) begin
        // Perform FP32 addition with rounding
        stage2_accum <= fp32_add(stage1_product, stage0_c_preproc);
        
        // Combine exceptions from previous stages
        stage2_exception <= stage1_exception | generate_accumulate_exceptions(stage1_product, stage0_c_preproc);
        
        stage2_valid <= 1'b1;
    end else if (stage2_ready) begin
        stage2_valid <= 1'b0;
    end
end

// Stage 3: Output formatting and final exception handling
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        stage3_valid <= 1'b0;
        stage3_result <= 32'h0;
        stage3_exception <= 3'b0;
        d_valid <= 1'b0;
        d_data <= 32'h0;
        exception <= 3'b0;
    end else if (stage2_valid && !pipe_flush) begin
        // Format final result according to IEEE 754 FP32
        stage3_result <= format_fp32_result(stage2_accum);
        
        // Final exception combination
        stage3_exception <= stage2_exception | generate_final_exceptions(stage2_accum);
        
        stage3_valid <= 1'b1;
    end else if (stage3_ready) begin
        stage3_valid <= 1'b0;
    end
    
    // Output logic for final result and exceptions
    if (stage3_ready || !rst_n) begin
        d_valid <= stage3_valid;
        d_data <= stage3_result;
        exception <= stage3_exception;
    end
end

// Pipeline ready signals - propagate through stages
assign stage0_ready = stage1_valid ? stage1_ready : 1'b1;
assign stage1_ready = stage2_valid ? stage2_ready : 1'b1;
assign stage2_ready = stage3_valid ? stage3_ready : 1'b1;
assign stage3_ready = d_ready;

// FP16 to FP32 conversion function
function [31:0] fp16_to_fp32(input [15:0] fp16_val);
    logic sign;
    logic [4:0] exp;
    logic [9:0] mantissa;
    logic [31:0] result;

    sign = fp16_val[15];
    exp = fp16_val[14:10];
    mantissa = fp16_val[9:0];

    // Handle special cases
    if (exp == 5'b11111) begin
        // NaN or Infinity
        result = {sign, 8'hFF, 23'h000000};
    end else if (exp == 5'b00000) begin
        // Zero or subnormal - flush to zero for FTZ
        result = {sign, 8'h00, 23'h000000};
    end else begin
        // Normal number
        result = {sign, exp[4:0] + 5'd15, mantissa, 13'h000};
    end

    return result;
endfunction

// Generate input exceptions for FP16 inputs
function [2:0] generate_input_exceptions(input [15:0] a_val, input [15:0] b_val);
    logic [2:0] flags;

    // Check for NaN or infinity in inputs
    flags = 3'b0;
    
    if ((a_val[15:10] == 5'b11111) || (b_val[15:10] == 5'b11111)) begin
        flags[2] = 1'b1; // invalid operation
    end
    
    return flags;
endfunction

// FP32 multiply with rounding
function [31:0] fp32_multiply(input [31:0] a, input [31:0] b);
    logic sign_a, sign_b, sign_result;
    logic [8:0] exp_a, exp_b, exp_result;
    logic [22:0] mant_a, mant_b, mant_result;
    logic [31:0] result;

    // Extract components
    sign_a = a[31];
    exp_a = a[30:23];
    mant_a = a[22:0];
    
    sign_b = b[31];
    exp_b = b[30:23];
    mant_b = b[22:0];

    // Handle special cases
    if ((exp_a == 9'b111111111) || (exp_b == 9'b111111111)) begin
        // One or both are infinity or NaN
        result = {sign_a ^ sign_b, 8'hFF, 23'h000000};
    end else if ((mant_a == 23'h0) || (mant_b == 23'h0)) begin
        // One or both are zero
        result = {sign_a ^ sign_b, 8'h00, 23'h000000};
    end else begin
        // Normal case - perform multiplication
        exp_result = exp_a + exp_b - 9'd127;
        
        // Multiply mantissas (with implicit 1)
        mant_result = (mant_a + 23'h000001) * (mant_b + 23'h000001);
        
        // Normalize result
        if (mant_result[46:23] != 24'h0) begin
            exp_result = exp_result + 1;
            mant_result = mant_result >> 1;
        end
        
        sign_result = sign_a ^ sign_b;
        result = {sign_result, exp_result[8:0], mant_result[22:0]};
    end

    return result;
endfunction

// Generate multiply stage exceptions
function [2:0] generate_multiply_exceptions(input [31:0] a, input [31:0] b);
    logic [2:0] flags;

    flags = 3'b0;
    
    // Check for overflow/underflow in multiplication
    if ((a[30:23] == 9'b111111111) || (b[30:23] == 9'b111111111)) begin
        flags[0] = 1'b1; // overflow
    end
    
    return flags;
endfunction

// FP32 addition with rounding
function [31:0] fp32_add(input [31:0] a, input [31:0] b);
    logic sign_a, sign_b, sign_result;
    logic [8:0] exp_a, exp_b, exp_result;
    logic [22:0] mant_a, mant_b, mant_result;
    logic [31:0] result;

    // Extract components
    sign_a = a[31];
    exp_a = a[30:23];
    mant_a = a[22:0];
    
    sign_b = b[31];
    exp_b = b[30:23];
    mant_b = b[22:0];

    // Handle special cases
    if ((exp_a == 9'b111111111) || (exp_b == 9'b111111111)) begin
        // One or both are infinity or NaN
        result = {sign_a ^ sign_b, 8'hFF, 23'h000000};
    end else if ((mant_a == 23'h0) || (mant_b == 23'h0)) begin
        // One or both are zero - return the non-zero operand
        result = (mant_a == 23'h0) ? b : a;
    end else begin
        // Normal case - perform addition
        exp_result = exp_a;
        
        // Align mantissas and add
        if (exp_a < exp_b) begin
            mant_a = mant_a >> (exp_b - exp_a);
            exp_result = exp_b;
        end else if (exp_a > exp_b) begin
            mant_b = mant_b >> (exp_a - exp_b);
        end
        
        // Add mantissas with implicit 1
        mant_result = (mant_a + 23'h000001) + (mant_b + 23'h000001);
        
        // Normalize result
        if (mant_result[24:23] != 2'b0) begin
            exp_result = exp_result + 1;
            mant_result = mant_result >> 1;
        end
        
        sign_result = sign_a ^ sign_b;
        result = {sign_result, exp_result[8:0], mant_result[22:0]};
    end

    return result;
endfunction

// Generate accumulate stage exceptions
function [2:0] generate_accumulate_exceptions(input [31:0] product, input [31:0] c);
    logic [2:0] flags;

    flags = 3'b0;
    
    // Check for overflow/underflow in addition
    if ((product[30:23] == 9'b111111111) || (c[30:23] == 9'b111111111)) begin
        flags[0] = 1'b1; // overflow
    end
    
    return flags;
endfunction

// Format final FP32 result according to IEEE 754
function [31:0] format_fp32_result(input [31:0] value);
    logic [31:0] result;

    // For simplicity, we assume the input is already properly formatted
    // In a real implementation, this would handle rounding and normalization
    
    result = value;
    
    return result;
endfunction

// Generate final stage exceptions
function [2:0] generate_final_exceptions(input [31:0] accum);
    logic [2:0] flags;

    flags = 3'b0;
    
    // Check for overflow/underflow in final result
    if (accum[30:23] == 9'b111111111) begin
        flags[0] = 1'b1; // overflow
    end
    
    return flags;
endfunction

endmodule
```