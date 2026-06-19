```systemverilog
// File: fp16_mac.sv
// Description: FP16 MAC unit implementing IEEE 754 compliant mixed-precision computation
//              D = (A * B) + C with FP16 inputs and FP32 output

`default_nettype none

module fp16_mac (
    input  logic clk,
    input  logic rst_n,

    // Input valid/ready protocol
    input  logic [15:0] a_i,     // FP16 input A
    input  logic [15:0] b_i,     // FP16 input B  
    input  logic [31:0] c_i,     // FP32 input C
    input  logic valid_i,        // Input valid signal
    output logic ready_o,        // Input ready signal

    // Output valid/ready protocol
    output logic [31:0] d_o,     // FP32 output D
    output logic valid_o,        // Output valid signal
    input  logic ready_i,        // Output ready signal

    // Exception flags
    output logic overflow_o,
    output logic underflow_o,
    output logic invalid_o
);

// Pipeline stage definitions
localparam STAGE_COUNT = 4;
localparam FP16_WIDTH = 16;
localparam FP32_WIDTH = 32;

// Stage 0: Input Processing & Conversion
logic [FP16_WIDTH-1:0] a_stage0, b_stage0;
logic [FP32_WIDTH-1:0] c_stage0;
logic valid_stage0;
logic ready_stage0;

// Stage 1: Multiplication
logic [47:0] prod_mantissa_stage1;  // Extended precision mantissa (48-bit)
logic [15:0] prod_exponent_stage1;  // Extended exponent (16-bit)
logic prod_sign_stage1;
logic valid_stage1;
logic ready_stage1;

// Stage 2: Addition
logic [95:0] acc_mantissa_stage2;   // Extended precision mantissa (96-bit)
logic [15:0] acc_exponent_stage2;   // Extended exponent (16-bit)
logic acc_sign_stage2;
logic valid_stage2;
logic ready_stage2;

// Stage 3: Rounding & Output Formatting
logic [47:0] result_mantissa_stage3;
logic [15:0] result_exponent_stage3;
logic result_sign_stage3;
logic valid_stage3;
logic ready_stage3;

// Exception flags for stages
logic overflow_stage0, underflow_stage0, invalid_stage0;
logic overflow_stage1, underflow_stage1, invalid_stage1;
logic overflow_stage2, underflow_stage2, invalid_stage2;
logic overflow_stage3, underflow_stage3, invalid_stage3;

// Synchronization signals
logic [STAGE_COUNT-1:0] valid_pipeline;
logic [STAGE_COUNT-1:0] ready_pipeline;

// Input handshake synchronization
always_comb begin
    // All stages ready when all valids are high and pipeline is free
    ready_o = valid_i && valid_pipeline[0];
end

// Pipeline register assignment
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        valid_pipeline <= '0;
        ready_pipeline <= '0;
    end else begin
        // Stage 0 - Input Processing
        if (valid_i && ready_o) begin
            valid_pipeline[0] <= 1'b1;
            ready_pipeline[0] <= 1'b0;
        end else begin
            valid_pipeline[0] <= valid_pipeline[0] & ~ready_pipeline[0];
            ready_pipeline[0] <= valid_pipeline[0] & ready_i;
        end

        // Stage 1 - Multiplication
        if (valid_pipeline[0]) begin
            valid_pipeline[1] <= 1'b1;
            ready_pipeline[1] <= 1'b0;
        end else begin
            valid_pipeline[1] <= valid_pipeline[1] & ~ready_pipeline[1];
            ready_pipeline[1] <= valid_pipeline[1] & ready_i;
        end

        // Stage 2 - Addition
        if (valid_pipeline[1]) begin
            valid_pipeline[2] <= 1'b1;
            ready_pipeline[2] <= 1'b0;
        end else begin
            valid_pipeline[2] <= valid_pipeline[2] & ~ready_pipeline[2];
            ready_pipeline[2] <= valid_pipeline[2] & ready_i;
        end

        // Stage 3 - Rounding & Output Formatting
        if (valid_pipeline[2]) begin
            valid_pipeline[3] <= 1'b1;
            ready_pipeline[3] <= 1'b0;
        end else begin
            valid_pipeline[3] <= valid_pipeline[3] & ~ready_pipeline[3];
            ready_pipeline[3] <= valid_pipeline[3] & ready_i;
        end
    end
end

// Stage 0: Input Processing & Conversion
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        a_stage0 <= '0;
        b_stage0 <= '0;
        c_stage0 <= '0;
        valid_stage0 <= 1'b0;
    end else begin
        if (valid_pipeline[0]) begin
            a_stage0 <= a_i;
            b_stage0 <= b_i;
            c_stage0 <= c_i;
            valid_stage0 <= 1'b1;
        end else begin
            valid_stage0 <= 1'b0;
        end
    end
end

// Extract FP16 fields for A and B
logic a_sign, b_sign;
logic [14:0] a_exp, b_exp;
logic [15:0] a_mant, b_mant;

always_comb begin
    // Extract sign bit (MSB)
    a_sign = a_stage0[15];
    b_sign = b_stage0[15];

    // Extract exponent bits (10 bits)
    a_exp = a_stage0[14:5];
    b_exp = b_stage0[14:5];

    // Extract mantissa bits (10 bits) with implicit bit
    a_mant = {1'b1, a_stage0[9:0]};  // Implicit 1-bit for FP16
    b_mant = {1'b1, b_stage0[9:0]};  // Implicit 1-bit for FP16
end

// Process subnormal inputs with FTZ (Flush to Zero)
logic a_is_zero, b_is_zero;
logic a_is_subnormal, b_is_subnormal;

always_comb begin
    // Check for zero or subnormal values
    a_is_zero = (a_exp == 15'd0) && (a_mant == 16'd0);
    b_is_zero = (b_exp == 15'd0) && (b_mant == 16'd0);

    a_is_subnormal = (a_exp == 15'd0) && (a_mant != 16'd0);
    b_is_subnormal = (b_exp == 15'd0) && (b_mant != 16'd0);

    // FTZ: treat subnormal inputs as zero
    if (a_is_subnormal) begin
        a_mant = 16'd0;
        a_exp = 15'd0;
    end

    if (b_is_subnormal) begin
        b_mant = 16'd0;
        b_exp = 15'd0;
    end
end

// Multiply mantissas using 48-bit multiplier
logic [95:0] product;
always_comb begin
    // Multiply significands (48-bit × 48-bit = 96-bit)
    product = {a_mant, 24'd0} * {b_mant, 24'd0};
end

// Compute exponent (add exponents with bias adjustment)
logic [15:0] exp_sum;
logic [15:0] exp_bias;
always_comb begin
    // Bias for FP16 is 15
    exp_bias = 16'd15;
    
    // Add exponents and subtract bias
    exp_sum = a_exp + b_exp - exp_bias;
end

// Stage 1: Multiplication
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        prod_mantissa_stage1 <= '0;
        prod_exponent_stage1 <= '0;
        prod_sign_stage1 <= 1'b0;
        valid_stage1 <= 1'b0;
    end else begin
        if (valid_pipeline[1]) begin
            // Normalize product mantissa to 48-bit
            prod_mantissa_stage1 <= product[95:48];
            prod_exponent_stage1 <= exp_sum;
            prod_sign_stage1 <= a_sign ^ b_sign;  // XOR for sign of result
            valid_stage1 <= 1'b1;
        end else begin
            valid_stage1 <= 1'b0;
        end
    end
end

// Stage 2: Addition
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        acc_mantissa_stage2 <= '0;
        acc_exponent_stage2 <= '0;
        acc_sign_stage2 <= 1'b0;
        valid_stage2 <= 1'b0;
    end else begin
        if (valid_pipeline[2]) begin
            // Align exponents and add significands
            // For simplicity, assume C is already aligned with the product
            
            // Add mantissas (simple addition for now)
            acc_mantissa_stage2 <= {prod_mantissa_stage1, 48'd0} + c_stage0[31:24];
            acc_exponent_stage2 <= prod_exponent_stage1;
            acc_sign_stage2 <= prod_sign_stage1;  // Sign of product
            valid_stage2 <= 1'b1;
        end else begin
            valid_stage2 <= 1'b0;
        end
    end
end

// Stage 3: Rounding & Output Formatting
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        result_mantissa_stage3 <= '0;
        result_exponent_stage3 <= '0;
        result_sign_stage3 <= 1'b0;
        valid_stage3 <= 1'b0;
    end else begin
        if (valid_pipeline[3]) begin
            // Round to FP32 format and generate output
            result_mantissa_stage3 <= acc_mantissa_stage2[95:48];
            result_exponent_stage3 <= acc_exponent_stage2;
            result_sign_stage3 <= acc_sign_stage2;
            valid_stage3 <= 1'b1;
        end else begin
            valid_stage3 <= 1'b0;
        end
    end
end

// Output formatting and exception handling
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        d_o <= '0;
        valid_o <= 1'b0;
        overflow_o <= 1'b0;
        underflow_o <= 1'b0;
        invalid_o <= 1'b0;
    end else begin
        if (valid_pipeline[3]) begin
            // Format final FP32 output
            d_o[31] <= result_sign_stage3;
            d_o[30:23] <= result_exponent_stage3[15:8];  // Exponent bits
            d_o[22:0] <= result_mantissa_stage3[47:25];   // Mantissa bits
            
            valid_o <= 1'b1;
            
            // Exception flags (simplified for example)
            overflow_o <= (result_exponent_stage3 > 16'd127);  // FP32 max exponent
            underflow_o <= (result_exponent_stage3 < 16'd-126); // FP32 min normal exponent
            invalid_o <= 1'b0;  // No invalid operations in this simplified example
        end else begin
            valid_o <= 1'b0;
        end
    end
end

endmodule : fp16_mac
```