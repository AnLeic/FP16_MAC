// File: fp_unpacker.sv
// Module: fp_unpacker
// Description: Parameterized floating-point unpacker according to IEEE 754 standard
// Parameters:
//   EXP_WIDTH - Width of the exponent field
//   MANT_WIDTH - Width of the mantissa field
// Outputs:
//   sign - Single bit sign (0 for positive, 1 for negative)
//   unbiased_exp - Unbiased exponent (signed integer)
//   mantissa - Mantissa with implicit leading bit handled correctly

module fp_unpacker #(
    parameter int EXP_WIDTH = 8,
    parameter int MANT_WIDTH = 23
)(
    input  logic [EXP_WIDTH + MANT_WIDTH : 0] packed_fp,
    output logic sign,
    output logic [EXP_WIDTH - 1 : 0] unbiased_exp,
    output logic [MANT_WIDTH : 0] mantissa
);

    // Extract components from packed floating-point number
    // Layout: [sign][exponent (EXP_WIDTH)][mantissa (MANT_WIDTH)]
    assign sign = packed_fp[EXP_WIDTH + MANT_WIDTH];
    assign unbiased_exp = packed_fp[EXP_WIDTH + MANT_WIDTH - 1 : MANT_WIDTH];

    // Handle mantissa extraction with implicit leading bit
    // For normal numbers (exponent != 0), the implicit leading bit is 1
    // For subnormal numbers (exponent == 0), the implicit leading bit is 0
    logic is_subnormal;
    assign is_subnormal = (unbiased_exp == '0);
    
    // Extract mantissa bits
    logic [MANT_WIDTH - 1 : 0] mantissa_bits;
    assign mantissa_bits = packed_fp[MANT_WIDTH - 1 : 0];
    
    // Handle the implicit leading bit based on normal/subnormal state
    always_comb begin
        if (is_subnormal) begin
            // Subnormal: implicit leading bit is 0, so just output the raw mantissa bits with zero MSB
            mantissa = {1'b0, mantissa_bits};
        end else begin
            // Normal: implicit leading bit is 1, so set MSB to 1 and output mantissa bits
            mantissa = {1'b1, mantissa_bits};
        end
    end

endmodule
