`timescale 1ns / 1ps
module tb;

logic clk, rst_n;
logic [15:0] a_i, b_i;
logic [31:0] c_i;
logic valid_i, ready_o;
logic [31:0] d_o;
logic valid_o, ready_i;
logic overflow_o, underflow_o, invalid_o;

int errors = 0;

fp16_mac dut (
    .clk(clk), .rst_n(rst_n),
    .a_i(a_i), .b_i(b_i), .c_i(c_i), .valid_i(valid_i), .ready_o(ready_o),
    .d_o(d_o), .valid_o(valid_o), .ready_i(ready_i),
    .overflow_o(overflow_o), .underflow_o(underflow_o), .invalid_o(invalid_o)
);

bind fp16_mac fp16_mac_sva inst_fp16_mac_sva (
    .clk(clk), .rst_n(rst_n),
    .a_i(a_i), .b_i(b_i), .c_i(c_i), .valid_i(valid_i), .ready_o(ready_o),
    .d_o(d_o), .valid_o(valid_o), .ready_i(ready_i),
    .overflow_o(overflow_o), .underflow_o(underflow_o), .invalid_o(invalid_o)
);

initial begin
    clk = 0;
    forever #5 clk = ~clk;
end

task automatic apply(input logic [15:0] a, input logic [15:0] b,
                      input logic [31:0] c, input logic [31:0] expected,
                      input string name);
    begin
        @(posedge clk);
        a_i <= a; b_i <= b; c_i <= c; valid_i <= 1'b1;
        @(posedge clk);
        valid_i <= 1'b0;
        wait (valid_o === 1'b1);
        #1;
        if (d_o !== expected) begin
            $display("FAIL %-24s got=%h expected=%h ovf=%b unf=%b inv=%b",
                      name, d_o, expected, overflow_o, underflow_o, invalid_o);
            errors++;
        end else begin
            $display("PASS %-24s got=%h", name, d_o);
        end
        @(posedge clk);
    end
endtask

initial begin
    rst_n = 0;
    a_i = 16'b0; b_i = 16'b0; c_i = 32'b0;
    valid_i = 1'b0;
    ready_i = 1'b1;

    repeat (3) @(posedge clk);
    rst_n = 1;
    @(posedge clk);

    // 1.0 * 2.0 + 3.0 = 5.0
    apply(16'h3C00, 16'h4000, 32'h40400000, 32'h40A00000, "1x2+3=5");
    // 0.0 * 2.0 + 2.0 = 2.0
    apply(16'h0000, 16'h4000, 32'h40000000, 32'h40000000, "0x2+2=2");
    // subnormal FP16 (FTZ) * 2.0 + 2.0 = 2.0
    apply(16'h0001, 16'h4000, 32'h40000000, 32'h40000000, "subnormalx2+2=2(FTZ)");
    // NaN propagation
    apply(16'h7E00, 16'h4000, 32'h40000000, 32'h7FC00000, "NaNx2+2=NaN");
    // Infinity propagation
    apply(16'h7C00, 16'h4000, 32'h40000000, 32'h7F800000, "Infx2+2=Inf");

    if (errors == 0) $display("ALL TESTS PASSED");
    else $display("%0d TEST(S) FAILED", errors);

    #20 $finish;
end

endmodule
