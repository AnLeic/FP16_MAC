`timescale 1ns / 1ps
module tb;

/* verilator lint_off SYNCASYNCNET */

/* verilator lint_off UNUSEDSIGNAL */

logic clk, rst_n;
logic [15:0] A, B;
logic [31:0] C;
logic Valid_in, Ready_in;
logic[31:0] D;
logic Valid_out, Ready_out, Overflow, Underflow, NaN;

/* verilator lint_on UNUSEDSIGNAL */



fp16_mac dut(
    .clk(clk), .rst_n(rst_n), .A(A), .B(B), .C(C),
    .D(D), .Valid_in(Valid_in), .Valid_out(Valid_out),
    .Ready_in(Ready_in), .Ready_out(Ready_out), .Overflow(Overflow),
    .Underflow(Underflow), .NaN(NaN)
);

bind fp16_mac fp16_mac_sva inst_fp16_mac_checker (
        .clk(clk), .rst_n(rst_n), .A(A), .B(B), .C(C),
        .Valid_in(Valid_in), .Ready_in(Ready_in), .D(D),
        .Valid_out(Valid_out), .Ready_out(Ready_out),
        .Overflow(Overflow), .Underflow(Underflow), .NaN(NaN)
    );
    /* verilator lint_off SYNCASYNCNET */

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    initial begin
        rst_n = 0;
        A = 16'b0;
        B = 16'b0;
        C = 32'b0;
        Valid_in = 1'b0;
        Ready_out = 1'b1;

        #20 rst_n = 1;

        #100 $finish;
    end

endmodule
