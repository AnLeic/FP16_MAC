// File: fp16_mac.sv
// D = (A * B) + C  where A,B are FP16 and C,D are FP32 (IEEE 754).
// FTZ subnormal inputs, RNE rounding, 4-stage pipeline, valid/ready handshake.

`default_nettype none

module fp16_mac (
    input  logic clk,
    input  logic rst_n,

    input  logic [15:0] a_i,
    input  logic [15:0] b_i,
    input  logic [31:0] c_i,
    input  logic        valid_i,
    output logic        ready_o,

    output logic [31:0] d_o,
    output logic        valid_o,
    input  logic        ready_i,

    output logic overflow_o,
    output logic underflow_o,
    output logic invalid_o
);

    localparam int FP16_EXP  = 5;
    localparam int FP16_MANT = 10;
    localparam int FP32_EXP  = 8;
    localparam int FP32_MANT = 23;
    localparam int FRAC_W    = FP32_MANT + 3; // 26: 23 kept + 3 guard/round/sticky region

    // Single-slot-per-stage pipeline: a stage advances only when the next
    // stage can accept it (or is empty), so the whole pipe stalls together.
    logic [3:0] valid_q;
    logic       stall;
    assign stall   = valid_q[3] & ~ready_i;
    assign ready_o = ~stall;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) valid_q <= '0;
        else if (!stall) valid_q <= {valid_q[2:0], valid_i & ready_o};
    end

    assign valid_o = valid_q[3];

    //--------------------------------------------------------------
    // Stage 0 (comb): unpack A, B, C and classify special values
    //--------------------------------------------------------------
    logic a_sign_u, b_sign_u, c_sign_u;
    logic [FP16_EXP-1:0]  a_exp_u,  b_exp_u;
    logic [FP32_EXP-1:0]  c_exp_u;
    logic [FP16_MANT:0]   a_mant_u, b_mant_u;
    logic [FP32_MANT:0]   c_mant_u;

    fp_unpacker #(.EXP_WIDTH(FP16_EXP), .MANT_WIDTH(FP16_MANT)) u_unpack_a (
        .packed_fp(a_i), .sign(a_sign_u), .unbiased_exp(a_exp_u), .mantissa(a_mant_u));
    fp_unpacker #(.EXP_WIDTH(FP16_EXP), .MANT_WIDTH(FP16_MANT)) u_unpack_b (
        .packed_fp(b_i), .sign(b_sign_u), .unbiased_exp(b_exp_u), .mantissa(b_mant_u));
    fp_unpacker #(.EXP_WIDTH(FP32_EXP), .MANT_WIDTH(FP32_MANT)) u_unpack_c (
        .packed_fp(c_i), .sign(c_sign_u), .unbiased_exp(c_exp_u), .mantissa(c_mant_u));

    logic a_is_zero, b_is_zero, c_is_zero, a_is_sub, b_is_sub, c_is_sub;
    logic a_is_inf, b_is_inf, c_is_inf, a_is_nan, b_is_nan, c_is_nan;

    assign a_is_zero = (a_exp_u == '0) && (a_i[FP16_MANT-1:0] == '0);
    assign b_is_zero = (b_exp_u == '0) && (b_i[FP16_MANT-1:0] == '0);
    assign c_is_zero = (c_exp_u == '0) && (c_i[FP32_MANT-1:0] == '0);
    assign a_is_sub   = (a_exp_u == '0) && (a_i[FP16_MANT-1:0] != '0);
    assign b_is_sub   = (b_exp_u == '0) && (b_i[FP16_MANT-1:0] != '0);
    assign c_is_sub   = (c_exp_u == '0) && (c_i[FP32_MANT-1:0] != '0);
    assign a_is_inf   = (a_exp_u == {FP16_EXP{1'b1}}) && (a_i[FP16_MANT-1:0] == '0);
    assign b_is_inf   = (b_exp_u == {FP16_EXP{1'b1}}) && (b_i[FP16_MANT-1:0] == '0);
    assign c_is_inf   = (c_exp_u == {FP32_EXP{1'b1}}) && (c_i[FP32_MANT-1:0] == '0);
    assign a_is_nan    = (a_exp_u == {FP16_EXP{1'b1}}) && (a_i[FP16_MANT-1:0] != '0);
    assign b_is_nan    = (b_exp_u == {FP16_EXP{1'b1}}) && (b_i[FP16_MANT-1:0] != '0);
    assign c_is_nan    = (c_exp_u == {FP32_EXP{1'b1}}) && (c_i[FP32_MANT-1:0] != '0);

    // FTZ: subnormal operands are treated as (signed) zero.
    logic [FP16_EXP-1:0] a_exp_f,  b_exp_f;
    logic [FP16_MANT:0]  a_mant_f, b_mant_f;
    logic [FP32_EXP-1:0] c_exp_f;
    /* verilator lint_off UNUSEDSIGNAL */
    logic [FP32_MANT:0]  c_mant_f; // bit[23] (implicit) intentionally unused;
                                    // magnitude is reconstructed as {1'b1, frac}
                                    // downstream once FTZ has cleared subnormals
    /* verilator lint_on UNUSEDSIGNAL */
    logic a_zero_f, b_zero_f, c_zero_f;

    assign a_exp_f  = a_is_sub ? '0 : a_exp_u;
    assign b_exp_f  = b_is_sub ? '0 : b_exp_u;
    assign c_exp_f  = c_is_sub ? '0 : c_exp_u;
    assign a_mant_f = a_is_sub ? '0 : a_mant_u;
    assign b_mant_f = b_is_sub ? '0 : b_mant_u;
    assign c_mant_f = c_is_sub ? '0 : c_mant_u;
    assign a_zero_f = a_is_zero | a_is_sub;
    assign b_zero_f = b_is_zero | b_is_sub;
    assign c_zero_f = c_is_zero | c_is_sub;

    logic invalid_s0;
    assign invalid_s0 = a_is_nan | b_is_nan | c_is_nan |
                         (a_is_inf & b_zero_f) | (b_is_inf & a_zero_f);

    // Stage 0 -> Stage 1 registers
    logic s1_a_sign, s1_b_sign, s1_c_sign;
    logic [FP16_EXP-1:0] s1_a_exp, s1_b_exp;
    logic [FP16_MANT:0]  s1_a_mant, s1_b_mant;
    logic [FP32_EXP-1:0] s1_c_exp;
    logic [FP32_MANT-1:0] s1_c_mant; // implicit bit dropped: C's magnitude is
                                      // reconstructed as {1'b1, frac} downstream,
                                      // valid because FTZ already zeroed subnormal C
    logic s1_a_zero, s1_b_zero, s1_c_zero;
    logic s1_a_inf, s1_b_inf, s1_c_inf, s1_invalid;

    always_ff @(posedge clk) begin
        if (!stall) begin
            s1_a_sign <= a_sign_u; s1_b_sign <= b_sign_u; s1_c_sign <= c_sign_u;
            s1_a_exp  <= a_exp_f;  s1_b_exp  <= b_exp_f;
            s1_a_mant <= a_mant_f; s1_b_mant <= b_mant_f;
            s1_c_exp  <= c_exp_f;  s1_c_mant <= c_mant_f[FP32_MANT-1:0];
            s1_a_zero <= a_zero_f; s1_b_zero <= b_zero_f; s1_c_zero <= c_zero_f;
            s1_a_inf  <= a_is_inf; s1_b_inf  <= b_is_inf; s1_c_inf <= c_is_inf;
            s1_invalid <= invalid_s0;
        end
    end

    //--------------------------------------------------------------
    // Stage 1 (comb): 11x11 mantissa multiply, normalize product
    //--------------------------------------------------------------
    logic [2*(FP16_MANT+1)-1:0] raw_product;      // 22 bits, Q2.20
    assign raw_product = s1_a_mant * s1_b_mant;

    logic [FRAC_W-1:0] prod_frac;                 // 26-bit fraction (post implicit bit)
    logic prod_exp_adj;
    assign prod_exp_adj = raw_product[21];
    assign prod_frac = prod_exp_adj ? {raw_product[20:0], {(FRAC_W-21){1'b0}}}
                                     : {raw_product[19:0], {(FRAC_W-20){1'b0}}};

    localparam logic signed [9:0] FP16_BIAS_S = 10'sd15;
    localparam logic signed [9:0] FP32_BIAS_S = 10'sd127;

    logic signed [9:0] true_exp_a, true_exp_b, true_exp_prod;
    assign true_exp_a = $signed({1'b0, 4'b0, s1_a_exp}) - FP16_BIAS_S;
    assign true_exp_b = $signed({1'b0, 4'b0, s1_b_exp}) - FP16_BIAS_S;
    assign true_exp_prod = true_exp_a + true_exp_b + 10'(prod_exp_adj);

    logic prod_sign, prod_zero;
    assign prod_sign = s1_a_sign ^ s1_b_sign;
    assign prod_zero = s1_a_zero | s1_b_zero;

    logic signed [9:0] true_exp_c;
    assign true_exp_c = $signed({1'b0, 1'b0, s1_c_exp}) - FP32_BIAS_S;

    // Stage 1 -> Stage 2 registers
    logic s2_prod_sign, s2_c_sign;
    logic signed [9:0] s2_prod_exp, s2_c_exp;
    logic [FRAC_W-1:0] s2_prod_frac, s2_c_frac;
    logic s2_prod_zero, s2_c_zero;
    logic s2_a_inf, s2_b_inf, s2_c_inf, s2_invalid;

    always_ff @(posedge clk) begin
        if (!stall) begin
            s2_prod_sign <= prod_sign; s2_c_sign <= s1_c_sign;
            s2_prod_exp  <= true_exp_prod; s2_c_exp <= true_exp_c;
            s2_prod_frac <= prod_frac;
            s2_c_frac    <= {s1_c_mant, 3'b0};
            s2_prod_zero <= prod_zero; s2_c_zero <= s1_c_zero;
            s2_a_inf <= s1_a_inf; s2_b_inf <= s1_b_inf; s2_c_inf <= s1_c_inf;
            s2_invalid <= s1_invalid;
        end
    end

    //--------------------------------------------------------------
    // Stage 2 (comb): align exponents, add/subtract significands
    //--------------------------------------------------------------
    logic signed [9:0] exp_diff;
    logic prod_ge_c;
    assign exp_diff  = s2_prod_exp - s2_c_exp;
    assign prod_ge_c = (exp_diff >= 0);

    logic [9:0] shift_amt;
    assign shift_amt = prod_ge_c ? exp_diff[9:0] : (-exp_diff[9:0]);

    // magnitudes with explicit implicit-bit, one extra headroom bit for carry
    logic [FRAC_W:0] prod_mag, c_mag;
    assign prod_mag = s2_prod_zero ? '0 : {1'b1, s2_prod_frac};
    assign c_mag    = s2_c_zero    ? '0 : {1'b1, s2_c_frac};

    logic [FRAC_W:0] big_mag, small_mag;
    logic signed [9:0] result_exp_pre;
    logic big_sign, small_sign;
    assign big_mag   = prod_ge_c ? prod_mag : c_mag;
    assign small_mag = prod_ge_c ? c_mag    : prod_mag;
    assign big_sign  = prod_ge_c ? s2_prod_sign : s2_c_sign;
    assign small_sign= prod_ge_c ? s2_c_sign    : s2_prod_sign;
    assign result_exp_pre = prod_ge_c ? s2_prod_exp : s2_c_exp;

    // Right-shift the smaller magnitude by shift_amt, OR-reducing shifted
    // out bits into a sticky flag so precision loss is preserved.
    logic [FRAC_W:0] small_shifted;
    logic shift_sticky;
    logic [9:0] clamped_shift;
    assign clamped_shift = (shift_amt > 10'(FRAC_W+1)) ? 10'(FRAC_W+1) : shift_amt;
    assign small_shifted = small_mag >> clamped_shift;
    // sticky = OR of the bits shifted out (the low clamped_shift bits of small_mag)
    logic [FRAC_W:0] full_ones_mask;
    assign full_ones_mask = {(FRAC_W+1){1'b1}};
    assign shift_sticky  = (clamped_shift == 10'd0) ? 1'b0
                          : |(small_mag & (full_ones_mask >> (10'(FRAC_W+1) - clamped_shift)));

    logic [FRAC_W+1:0] sum_mag; // extra bit for carry-out of addition
    logic sum_sign;

    always_comb begin
        if (big_sign == small_sign) begin
            sum_mag  = {1'b0, big_mag} + {1'b0, small_shifted};
            sum_sign = big_sign;
        end else if (big_mag >= small_shifted) begin
            sum_mag  = {1'b0, big_mag} - {1'b0, small_shifted};
            sum_sign = big_sign;
        end else begin
            sum_mag  = {1'b0, small_shifted} - {1'b0, big_mag};
            sum_sign = small_sign;
        end
    end

    logic result_is_zero;
    assign result_is_zero = (sum_mag == '0);

    // Stage 2 -> Stage 3 registers
    logic s3_sign;
    logic signed [9:0] s3_exp;
    logic [FRAC_W+1:0] s3_sum_mag;
    logic s3_sticky_in, s3_is_zero;
    logic s3_a_inf, s3_b_inf, s3_c_inf, s3_invalid;

    always_ff @(posedge clk) begin
        if (!stall) begin
            s3_sign     <= sum_sign;
            s3_exp      <= result_exp_pre;
            s3_sum_mag  <= sum_mag;
            s3_sticky_in<= shift_sticky;
            s3_is_zero  <= result_is_zero;
            s3_a_inf <= s2_a_inf; s3_b_inf <= s2_b_inf; s3_c_inf <= s2_c_inf;
            s3_invalid  <= s2_invalid;
        end
    end

    //--------------------------------------------------------------
    // Stage 3 (comb): normalize (LZC), round (RNE), pack, flag
    //--------------------------------------------------------------
    // s3_sum_mag is FRAC_W+2 = 28 bits wide: bit[27] = carry-out from add,
    // bit[26] = implicit '1' when already normalized (no carry), bits[25:0]
    // = fraction. lead_zero counts left-shifts needed to bring the leading
    // '1' in bits[26:0] up to bit 26 (0 = already normalized).
    logic [4:0] lead_zero;
    always_comb begin
        lead_zero = 5'd27; // all-zero fallback (result is exactly zero)
        if (!s3_sum_mag[27]) begin
            for (int i = 26; i >= 0; i--) begin
                if (s3_sum_mag[i]) begin
                    lead_zero = 5'(26 - i);
                    break;
                end
            end
        end
    end

    logic signed [9:0] norm_exp;
    /* verilator lint_off UNUSEDSIGNAL */
    logic [FRAC_W+1:0] norm_mag; // bits [27:26] are carry/implicit headroom,
                                  // never read directly after normalization
    /* verilator lint_on UNUSEDSIGNAL */
    logic norm_sticky;

    always_comb begin
        if (s3_sum_mag[27]) begin
            // carry-out: shift right by 1, exponent +1, LSB becomes sticky
            norm_mag    = s3_sum_mag >> 1;
            norm_exp    = s3_exp + 1;
            norm_sticky = s3_sum_mag[0] | s3_sticky_in;
        end else if (lead_zero == 0) begin
            // already normalized (implicit bit at position 26)
            norm_mag    = s3_sum_mag;
            norm_exp    = s3_exp;
            norm_sticky = s3_sticky_in;
        end else begin
            // cancellation: shift left to renormalize
            norm_mag    = s3_sum_mag << lead_zero;
            norm_exp    = s3_exp - {5'b0, lead_zero};
            norm_sticky = s3_sticky_in;
        end
    end

    // norm_mag[25:0] is the 26-bit fraction: bits[25:3] are the 23 kept
    // mantissa bits (most significant, closest to the binary point), and
    // bits[2:0] are the guard/round/sticky region to be rounded away.
    logic [FRAC_W-1:0] expanded_mant;
    logic g_bit, r_bit, s_bit;
    assign expanded_mant = norm_mag[FRAC_W-1:0];
    assign g_bit = expanded_mant[2];
    assign r_bit = expanded_mant[1];
    assign s_bit = expanded_mant[0] | norm_sticky;

    logic [FP32_MANT-1:0] rounded_mant;
    logic round_exp_inc;
    fp_rounder #(.MAN_WIDTH(FP32_MANT), .GUARD_WIDTH(3)) u_round (
        .expanded_mantissa(expanded_mant),
        .guard(g_bit), .round(r_bit), .sticky(s_bit),
        .rounded_mantissa(rounded_mant), .exp_increment(round_exp_inc));

    logic signed [9:0] final_exp_true;
    assign final_exp_true = norm_exp + (round_exp_inc ? 10'sd1 : 10'sd0);

    logic signed [10:0] final_exp_biased;
    assign final_exp_biased = 11'(final_exp_true) + 11'sd127;

    logic result_overflow, result_underflow;
    assign result_overflow  = ~s3_is_zero && (final_exp_biased >= 11'sd255);
    assign result_underflow = ~s3_is_zero && (final_exp_biased <= 11'sd0);

    logic [FP32_EXP-1:0] packed_exp;
    logic [FP32_MANT-1:0] packed_mant;
    logic special_inf;
    assign special_inf = (s3_a_inf | s3_b_inf | s3_c_inf) & ~s3_invalid;

    always_comb begin
        if (s3_invalid) begin
            packed_exp  = {FP32_EXP{1'b1}};
            packed_mant = {1'b1, {(FP32_MANT-1){1'b0}}}; // quiet NaN
        end else if (special_inf || result_overflow) begin
            packed_exp  = {FP32_EXP{1'b1}};
            packed_mant = '0;
        end else if (s3_is_zero || result_underflow) begin
            packed_exp  = '0;
            packed_mant = '0; // FTZ on underflow
        end else begin
            packed_exp  = final_exp_biased[FP32_EXP-1:0];
            packed_mant = rounded_mant;
        end
    end

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            d_o <= '0;
            overflow_o <= 1'b0;
            underflow_o <= 1'b0;
            invalid_o <= 1'b0;
        end else if (!stall) begin
            d_o <= {s3_sign & ~s3_invalid, packed_exp, packed_mant};
            overflow_o  <= result_overflow & ~s3_invalid;
            underflow_o <= result_underflow & ~s3_invalid & ~s3_is_zero;
            invalid_o   <= s3_invalid;
        end
    end

endmodule : fp16_mac
