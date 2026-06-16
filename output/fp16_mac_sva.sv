```systemverilog
// File: fp16_mac_checker.sv
// Description: Formal Verification Checker for FP16 Multiply-Accumulate (MAC) Unit
// Author: [Your Name]
// Date: [Date]

`timescale 1ns / 1ps

module fp16_mac_checker (
    input wire clk,
    input wire rst_n,

    // Input Ports
    input wire [15:0] A,
    input wire [15:0] B,
    input wire [31:0] C,
    input wire Valid_in,
    input wire Ready_in,

    // Output Ports
    output reg [31:0] D,
    output reg Valid_out,
    output reg Ready_out,

    // Exception Outputs
    output reg Overflow,
    output reg Underflow,
    output reg NaN
);

// Bind the checker to the fp16_mac module
bind fp16_mac fp16_mac_checker #(.clk(clk), .rst_n(rst_n)) inst_fp16_mac_checker (
    .A(A),
    .B(B),
    .C(C),
    .Valid_in(Valid_in),
    .Ready_in(Ready_in),
    .D(D),
    .Valid_out(Valid_out),
    .Ready_out(Ready_out),
    .Overflow(Overflow),
    .Underflow(Underflow),
    .NaN(NaN)
);

// Internal Signals
reg [15:0] A_reg, B_reg;
reg [31:0] C_reg;
reg Valid_in_d1, Ready_out_d1;
reg [15:0] product_fp16;
reg [31:0] sum_fp32;
reg [31:0] D_temp;
reg [1:0] stage;
reg [1:0] next_stage;

// Pipeline Registers
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        A_reg <= 16'b0;
        B_reg <= 16'b0;
        C_reg <= 32'b0;
        Valid_in_d1 <= 1'b0;
        Ready_out_d1 <= 1'b0;
        stage <= STAGE_1;
    end else begin
        A_reg <= A;
        B_reg <= B;
        C_reg <= C;
        Valid_in_d1 <= Valid_in;
        Ready_out_d1 <= Ready_out;
        stage <= next_stage;
    end
end

// Stage Selection Logic
always @(*) begin
    case (stage)
        STAGE_1: next_stage = STAGE_2;
        STAGE_2: next_stage = STAGE_3;
        STAGE_3: next_stage = STAGE_4;
        default: next_stage = STAGE_1;
    endcase
end

// Stage 1: Input Handling and Flush-to-Zero (FTZ)
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        product_fp16 <= 16'b0;
    end else if (stage == STAGE_1 && Valid_in_d1 && Ready_out_d1) begin
        // Flush subnormals to zero
        product_fp16 <= (A_reg[14:0] == 15'b0 || B_reg[14:0] == 15'b0) ? 16'b0 : A_reg * B_reg;
    end
end

// Stage 2: Multiply Operation
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        sum_fp32 <= 32'b0;
    end else if (stage == STAGE_2 && Valid_in_d1 && Ready_out_d1) begin
        // Convert FP16 product to FP32 without rounding
        sum_fp32 <= {product_fp16[15], product_fp16[14:0], 16'b0};
    end
end

// Stage 3: Add Operation with C
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        D_temp <= 32'b0;
    end else if (stage == STAGE_3 && Valid_in_d1 && Ready_out_d1) begin
        // Add converted result to C with RNE rounding
        D_temp <= $rtoi(sum_fp32 + C_reg);
    end
end

// Stage 4: Final Rounding and Exception Handling
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        D <= 32'b0;
        Valid_out <= 1'b0;
        Ready_out <= 1'b0;
        Overflow <= 1'b0;
        Underflow <= 1'b0;
        NaN <= 1'b0;
    end else if (stage == STAGE_4 && Valid_in_d1 && Ready_out_d1) begin
        // Apply RNE rounding and detect exceptions
        D <= $rtoi(D_temp);
        Valid_out <= 1'b1;
        Ready_out <= 1'b1;

        // Exception Handling
        Overflow = (D_temp > 32'h7F7FFFFF); // Max FP32 value
        Underflow = (D_temp < 32'h00800000); // Min FP32 value
        NaN = (A_reg[15] && A_reg[14:0] == 15'b0) || (B_reg[15] && B_reg[14:0] == 15'b0);
    end else begin
        Valid_out <= 1'b0;
        Ready_out <= 1'b0;
    end
end

// Backpressure Handling
assign Ready_in = stage == STAGE_1;

// Protocol Compliance (valid/ready handshake stability properties)
property valid_ready_handshake;
    @(posedge clk) disable iff (!rst_n)
    Valid_in |-> ##[0:3] Ready_out;
endproperty
assert property (valid_ready_handshake);

// Reset behavior (all registers cleared, outputs zeroed/invalid)
property reset_behavior;
    @(posedge clk) disable iff (!rst_n)
    rst_n |=> A_reg == 16'b0 && B_reg == 16'b0 && C_reg == 32'b0 &&
               Valid_in_d1 == 1'b0 && Ready_out_d1 == 1'b0 &&
               product_fp16 == 16'b0 && sum_fp32 == 32'b0 &&
               D_temp == 32'b0 && stage == STAGE_1 &&
               D == 32'b0 && Valid_out == 1'b0 && Ready_out == 1'b0 &&
               Overflow == 1'b0 && Underflow == 1'b0 && NaN == 1'b0;
endproperty
assert property (reset_behavior);

// Flush-to-Zero verification
property flush_to_zero;
    @(posedge clk) disable iff (!rst_n)
    stage == STAGE_1 && Valid_in_d1 && Ready_out_d1 &&
    (A_reg[14:0] == 15'b0 || B_reg[14:0] == 15'b0) |-> product_fp16 == 16'b0;
endproperty
assert property (flush_to_zero);

// Overflow targeting positive/negative infinity bit-patterns
property overflow_positive_infinity;
    @(posedge clk) disable iff (!rst_n)
    stage == STAGE_4 && Valid_in_d1 && Ready_out_d1 &&
    D_temp > 32'h7F7FFFFF |-> D == 32'h7F800000; // Positive infinity
endproperty
assert property (overflow_positive_infinity);

property overflow_negative_infinity;
    @(posedge clk) disable iff (!rst_n)
    stage == STAGE_4 && Valid_in_d1 && Ready_out_d1 &&
    D_temp < 32'h807FFFFF |-> D == 32'hFF800000; // Negative infinity
endproperty
assert property (overflow_negative_infinity);

// Cover Points
cover property (@(posedge clk) disable iff (!rst_n)
    A == 16'b0 && B != 16'b0); // A = 0, B arbitrary

cover property (@(posedge clk) disable iff (!rst_n)
    A != 16'b0 && B == 16'b0); // B = 0, A arbitrary

cover property (@(posedge clk) disable iff (!rst_n)
    A == 16'b0 && B == 16'b0); // Both A and B = 0

cover property (@(posedge clk) disable iff (!rst_n)
    A[15] && A[14:0] == 15'b0); // A is NaN

cover property (@(posedge clk) disable iff (!rst_n)
    B[15] && B[14:0] == 15'b0); // B is NaN

cover property (@(posedge clk) disable iff (!rst_n)
    C == 32'h7F800000 || C == 32'hFF800000); // C is NaN (positive/negative infinity)

cover property (@(posedge clk) disable iff (!rst_n)
    A_reg == 16'h7BFF && B_reg == 16'h7BFF); // Maximum FP16 values multiplied

cover property (@(posedge clk) disable iff (!rst_n)
    product_fp16 < 16'h0400); // Minimum FP16 products causing underflow

cover property (@(posedge clk) disable iff (!rst_n)
    sum_fp32 > 32'h7F7FFFFF || sum_fp32 < 32'h00800000); // Addition resulting in overflow or underflow

cover property (@(posedge clk) disable iff (!rst_n)
    stage == STAGE_1 && !Ready_out_d1); // Stall at Stage 1

cover property (@(posedge clk) disable iff (!rst_n)
    stage == STAGE_2 && !Ready_out_d1); // Stall at Stage 2

cover property (@(posedge clk) disable iff (!rst_n)
    stage == STAGE_3 && !Ready_out_d1); // Stall at Stage 3

cover property (@(posedge clk) disable iff (!rst_n)
    stage == STAGE_4 && !Ready_out_d1); // Stall at Stage 4

cover property (@(posedge clk) disable iff (!rst_n)
    rst_n && Valid_in_d1); // Reset during processing at each stage

endmodule
```