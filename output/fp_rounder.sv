// File: fp_rounder.sv
// Module: fp_rounder
// Description: IEEE 754 Round-to-Nearest, Ties-to-Even (RNE).
// Parameters:
//   MAN_WIDTH   - Width of the kept mantissa (e.g. 23 for FP32)
//   GUARD_WIDTH - Width of the discarded region below the kept mantissa
// expanded_mantissa: {kept_mantissa[MAN_WIDTH-1:0], guard_region[GUARD_WIDTH-1:0]}
//   guard  = MSB of the discarded region (first bit beyond kept precision)
//   round  = next bit below guard
//   sticky = OR of all remaining discarded bits (including any shifted out
//            during alignment/normalization upstream of this module)

module fp_rounder #(
    parameter int MAN_WIDTH   = 23,
    parameter int GUARD_WIDTH = 3
)(
    /* verilator lint_off UNUSEDSIGNAL */
    input  logic [MAN_WIDTH + GUARD_WIDTH - 1 : 0] expanded_mantissa,
    /* verilator lint_on UNUSEDSIGNAL */
    input  logic guard,
    input  logic round,
    input  logic sticky,
    output logic [MAN_WIDTH - 1 : 0] rounded_mantissa,
    output logic exp_increment
);

    logic [MAN_WIDTH - 1 : 0] current_mantissa;
    assign current_mantissa = expanded_mantissa[MAN_WIDTH + GUARD_WIDTH - 1 : GUARD_WIDTH];

    logic round_up;
    // RNE: round up if guard bit set AND (round or sticky set, i.e. strictly
    // more than halfway) OR it's an exact tie (guard set, round/sticky clear)
    // and the kept mantissa's LSB is odd (round to even).
    assign round_up = guard && (round || sticky || current_mantissa[0]);

    always_comb begin
        {exp_increment, rounded_mantissa} = round_up
            ? ({1'b0, current_mantissa} + 1'b1)
            : {1'b0, current_mantissa};
    end

endmodule
