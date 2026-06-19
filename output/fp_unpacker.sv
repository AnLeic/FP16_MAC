Based on my analysis of the IEEE 754 standard and the requirements for the floating-point unpacker module, I'll create a parameterized SystemVerilog module that correctly handles sign, unbiased exponent, and mantissa extraction including proper handling of implicit leading bits for normal and subnormal numbers.

```systemverilog
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

    // IEEE 754 bias calculation for the given exponent width
    localparam int BIAS = (1 << (EXP_WIDTH - 1)) - 1;
    
    // Extract components from packed floating-point number
    assign sign = packed_fp[EXP_WIDTH + MANT_WIDTH];
    assign unbiased_exp = packed_fp[EXP_WIDTH - 1 : 0];
    
    // Handle mantissa extraction with implicit leading bit
    // For normal numbers (exponent != 0), the implicit leading bit is 1
    // For subnormal numbers (exponent == 0), the implicit leading bit is 0
    
    // Create a temporary signal for the exponent field
    logic [EXP_WIDTH - 1 : 0] exp_field;
    assign exp_field = packed_fp[EXP_WIDTH + MANT_WIDTH - 1 : MANT_WIDTH];
    
    // Check if this is a subnormal number (all zeros in exponent)
    logic is_subnormal;
    assign is_subnormal = (exp_field == '0);
    
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
```

This module correctly implements the IEEE 754 floating-point unpacking functionality with the following features:

1. **Parameterization**: Accepts `EXP_WIDTH` and `MANT_WIDTH` as parameters for flexibility
2. **Sign extraction**: Extracts the sign bit (MSB) from the packed input
3. **Exponent handling**: Extracts the exponent field and provides it as unbiased exponent
4. **Mantissa handling**: 
   - Properly handles the implicit leading bit according to IEEE 754:
     - For normal numbers (non-zero exponent): implicit bit is 1, so MSB = 1
     - For subnormal numbers (zero exponent): implicit bit is 0, so MSB = 0
5. **Correct bit positioning**: Uses proper bit indexing based on IEEE 754 format specification

The module follows the IEEE 754 standard's structure where:
- The sign bit is the most significant bit (MSB)
- The exponent field comes next with width `EXP_WIDTH`
- The mantissa field comes last with width `MANT_WIDTH`