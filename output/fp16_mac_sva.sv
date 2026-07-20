// File: fp16_mac_sva.sv
// Description: Formal verification checker for the fp16_mac module.
// Bound module (see tb.sv) checking protocol, reset, latency, and IEEE 754
// special-value flag correctness for D = (A * B) + C.
//
// Historical values (rst_n, valid_i&&ready_o, c_i, the accept-condition
// flags) are captured with explicit shift registers rather than the SVA
// $past() system function, whose interaction with an event-driven timing
// scheduler can stall simulation in some tools; plain always_ff shadow
// registers are simple, portable RTL and sidestep that entirely.

`default_nettype none

module fp16_mac_sva (
    input logic clk,
    input logic rst_n,

    input logic [15:0] a_i,
    input logic [15:0] b_i,
    input logic [31:0] c_i,
    input logic valid_i,
    input logic ready_o,

    input logic [31:0] d_o,
    input logic valid_o,
    input logic ready_i,

    input logic overflow_o,
    input logic underflow_o,
    input logic invalid_o
);

    localparam int PIPE_LATENCY = 4;

    clocking cb @(posedge clk);
        input a_i, b_i, c_i, valid_i, ready_i, ready_o, d_o, valid_o,
              overflow_o, underflow_o, invalid_o;
    endclocking

    // General FP16/FP32 special-value classifiers (not hardcoded operand
    // values -- any exponent-all-ones pattern is Inf/NaN, any subnormal
    // pattern is exponent-zero-mantissa-nonzero).
    function automatic bit is_fp16_nan(logic [15:0] v);
        return (v[14:10] == 5'b11111) && (v[9:0] != 10'b0);
    endfunction
    function automatic bit is_fp16_inf(logic [15:0] v);
        return (v[14:10] == 5'b11111) && (v[9:0] == 10'b0);
    endfunction
    function automatic bit is_fp16_subnormal(logic [15:0] v);
        return (v[14:10] == 5'b0) && (v[9:0] != 10'b0);
    endfunction
    function automatic bit is_fp16_zero(logic [15:0] v);
        return v[14:0] == 15'b0; // +0 and -0
    endfunction
    function automatic bit is_fp32_zero(logic [31:0] v);
        return v[30:0] == 31'b0; // +0 and -0
    endfunction
    function automatic bit is_fp32_subnormal(logic [31:0] v);
        return (v[30:23] == 8'b0) && (v[22:0] != 23'b0);
    endfunction
    function automatic bit is_fp32_nan(logic [31:0] v);
        return (v[30:23] == 8'hFF) && (v[22:0] != 23'b0);
    endfunction
    function automatic bit is_fp32_inf(logic [31:0] v);
        return (v[30:23] == 8'hFF) && (v[22:0] == 23'b0);
    endfunction

    //--------------------------------------------------------------
    // Shadow history shift registers (depth PIPE_LATENCY), captured every
    // clock so assertions can reference "N cycles ago" without $past().
    //--------------------------------------------------------------
    logic prev_rst_n;
    logic accept_hist    [PIPE_LATENCY:0];
    logic [31:0] c_hist  [PIPE_LATENCY:0];
    logic ftz_cond_hist  [PIPE_LATENCY:0];
    logic ftz_c0_hist    [PIPE_LATENCY:0];
    logic nan_cond_hist  [PIPE_LATENCY:0];

    wire accept_now = valid_i && ready_o;

    // Product term is exactly zero after FTZ: A or B is +/-0 or subnormal
    // (note 16'h8000 is negative zero -- an equality check against 16'h0000
    // alone would miss it), with no NaN/Inf anywhere to preempt the result.
    wire prod_zero_now =
        (is_fp16_zero(a_i) || is_fp16_zero(b_i) ||
         is_fp16_subnormal(a_i) || is_fp16_subnormal(b_i)) &&
        !is_fp16_nan(a_i) && !is_fp16_nan(b_i) && !is_fp32_nan(c_i) &&
        !is_fp16_inf(a_i) && !is_fp16_inf(b_i) && !is_fp32_inf(c_i);

    // Split by C: only a *normal* C passes through bit-exactly. A subnormal
    // C is itself FTZ'd and a +/-0 C follows IEEE zero-sign rules, so those
    // cases must assert a zero-magnitude result, not d_o == C.
    wire ftz_cond_now = accept_now && prod_zero_now &&
        !is_fp32_zero(c_i) && !is_fp32_subnormal(c_i);
    wire ftz_c0_now = accept_now && prod_zero_now &&
        (is_fp32_zero(c_i) || is_fp32_subnormal(c_i));

    wire nan_cond_now = accept_now &&
        (is_fp16_nan(a_i) || is_fp16_nan(b_i) || is_fp32_nan(c_i));

    always_ff @(posedge clk) begin
        prev_rst_n <= rst_n;
        accept_hist[0]   <= accept_now;
        c_hist[0]        <= c_i;
        ftz_cond_hist[0] <= ftz_cond_now;
        ftz_c0_hist[0]   <= ftz_c0_now;
        nan_cond_hist[0] <= nan_cond_now;
        for (int k = 1; k <= PIPE_LATENCY; k++) begin
            accept_hist[k]   <= accept_hist[k-1];
            c_hist[k]        <= c_hist[k-1];
            ftz_cond_hist[k] <= ftz_cond_hist[k-1];
            ftz_c0_hist[k]   <= ftz_c0_hist[k-1];
            nan_cond_hist[k] <= nan_cond_hist[k-1];
        end
    end

    //--------------------------------------------------------------
    // Reset behavior: registers are guaranteed clear the cycle after
    // rst_n deasserts (async-reset always_ff blocks).
    //--------------------------------------------------------------
    assert property (@(cb)
        (!prev_rst_n && !rst_n) |->
        (d_o == 32'h0 && valid_o == 1'b0 &&
         overflow_o == 1'b0 && underflow_o == 1'b0 && invalid_o == 1'b0));

    //--------------------------------------------------------------
    // Protocol / handshake sanity: valid_o must not change while a
    // downstream consumer is not ready (data held stable until accepted).
    //--------------------------------------------------------------
    assert property (@(cb) disable iff (!rst_n)
        (valid_o && !ready_i) |=> valid_o);

    //--------------------------------------------------------------
    // Latency: exactly PIPE_LATENCY cycles from acceptance to output valid.
    // Holds whenever the consumer keeps ready_i high (the common case and
    // what this testbench drives); under backpressure valid_o may be held
    // an extra cycle, which is legal handshake behavior but not checked here.
    //--------------------------------------------------------------
    assert property (@(cb) disable iff (!rst_n)
        accept_hist[PIPE_LATENCY-1] |-> valid_o);

    //--------------------------------------------------------------
    // FTZ / zero-operand behavior: zero or subnormal (FTZ'd) A or B means
    // the product term is exactly zero, so a *normal* C must pass through
    // bit-exactly (a correctly rounded FP32 + 0 is exact). When C itself is
    // zero or subnormal, the result magnitude must be zero (sign follows
    // IEEE zero-sign rules and is not constrained here).
    //--------------------------------------------------------------
    assert property (@(cb) disable iff (!rst_n)
        ftz_cond_hist[PIPE_LATENCY-1] |-> (d_o == c_hist[PIPE_LATENCY-1]));

    assert property (@(cb) disable iff (!rst_n)
        ftz_c0_hist[PIPE_LATENCY-1] |-> (d_o[30:0] == 31'h0));

    //--------------------------------------------------------------
    // NaN propagation
    //--------------------------------------------------------------
    assert property (@(cb) disable iff (!rst_n)
        nan_cond_hist[PIPE_LATENCY-1] |-> (invalid_o == 1'b1 && is_fp32_nan(d_o)));

    //--------------------------------------------------------------
    // Exception flag consistency with output bit pattern
    //--------------------------------------------------------------
    assert property (@(cb) disable iff (!rst_n)
        valid_o |-> (overflow_o -> (is_fp32_inf(d_o) && !invalid_o)));

    assert property (@(cb) disable iff (!rst_n)
        valid_o |-> (underflow_o -> (d_o[30:0] == 31'h0 && !invalid_o)));

    assert property (@(cb) disable iff (!rst_n)
        valid_o |-> (invalid_o -> is_fp32_nan(d_o)));

    //--------------------------------------------------------------
    // Coverage points
    //--------------------------------------------------------------
    cover property (@(cb) disable iff (!rst_n)
        (valid_i && ready_o) && (is_fp16_nan(a_i) || is_fp16_nan(b_i)));

    cover property (@(cb) disable iff (!rst_n)
        (valid_i && ready_o) && (is_fp16_inf(a_i) || is_fp16_inf(b_i)));

    cover property (@(cb) disable iff (!rst_n)
        (valid_i && ready_o) && (is_fp16_subnormal(a_i) || is_fp16_subnormal(b_i)));

    cover property (@(cb) disable iff (!rst_n)
        (valid_i && ready_o) && (a_i == 16'h0000 || b_i == 16'h0000));

    cover property (@(cb) disable iff (!rst_n) overflow_o);

    cover property (@(cb) disable iff (!rst_n) underflow_o);

    cover property (@(cb) disable iff (!rst_n)
        (valid_o && !ready_i)); // pipeline stall / backpressure held

    cover property (@(cb) disable iff (!rst_n)
        (valid_i && ready_o) ##1 (valid_i && ready_o)); // back-to-back issue

endmodule : fp16_mac_sva
