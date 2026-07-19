# FP16×FP16 + FP32 Mixed-Precision MAC Unit Specification

## 1. Overview & Scope
This document defines the microarchitectural and functional specification for a high-throughput, mixed-precision Multiply-Accumulate (MAC) sub-module targeting AI accelerator data paths. The unit computes:
`D = (A × B) + C`
where `A` and `B` are IEEE 754 FP16 operands, and `C` and `D` are IEEE 754 FP32 operands. The design prioritizes maximum operating frequency through a strictly balanced 4-stage pipeline, deterministic latency, robust exception handling, and compliance with IEEE 754 rounding and subnormal policies as constrained.

## 2. Interface Definition
The module implements a standard synchronous Valid/Ready handshake protocol for both input and output ports. Backpressure is supported via ready signal propagation.

| Signal Name | Direction | Width | Description |
|-------------|-----------|-------|-------------|
| `clk_i`     | In        | 1     | Positive-edge system clock |
| `rst_ni`    | In        | 1     | Active-low asynchronous reset, synchronous release |
| `valid_i`   | In        | 1     | Input data valid assertion |
| `ready_o`   | Out       | 1     | Module ready to accept input (high when pipeline stage 0 is empty) |
| `a_i`       | In        | 16    | FP16 operand A (S:1, E:5, M:10) |
| `b_i`       | In        | 16    | FP16 operand B (S:1, E:5, M:10) |
| `c_i`       | In        | 32    | FP32 accumulator operand C (S:1, E:8, M:23) |
| `valid_o`   | Out       | 1     | Output data valid assertion |
| `ready_i`   | In        | 1     | Downstream ready to accept output |
| `d_o`       | Out       | 32    | FP32 result D (S:1, E:8, M:23) |
| `flags_o`   | Out       | 3     | Exception flags: `[2] Overflow, [1] Underflow, [0] Invalid` |

**Handshake Protocol Rules:**
- Data is sampled when `valid_i && ready_o == 1'b1`.
- Output drives `valid_o == 1'b1` when Stage 3 completes. Result is valid for one cycle unless `ready_i == 1'b0`, in which case it holds until acknowledged.
- Backpressure stalls the pipeline at the appropriate stage without corrupting state.

## 3. Pipeline Architecture (4-Stage)
The datapath is partitioned into four balanced combinational segments separated by pipeline registers. Each register includes enable logic driven by a ready-propagation chain to support backpressure.

| Stage | Function | Combinational Logic | Pipeline Register Output |
|-------|----------|---------------------|--------------------------|
| **S0** | Input Decode & FTZ | FP16 field extraction, subnormal detection, zero/Inf/NaN classification, control signal generation | Decoded fields, internal control flags, pre-converted mantissas/exponents |
| **S1** | Multiplication | Exponent addition with bias correction, 11×11 bit mantissa multiplication, sign XOR, product pre-normalization | Product exponent (8b), Product mantissa (22b), Product sign, intermediate flags |
| **S2** | Alignment & Addition | Exponent comparison, mantissa right-shift alignment, 24-bit two's complement addition, carry generation | Aligned sum mantissa (25b with GRS), sum exponent, pre-round control |
| **S3** | Normalize & Round | Leading Zero Count (LZC), left/right normalization shift, RNE rounding logic, FP32 packing, exception flag latching | Final `d_o`, `flags_o`, output valid/ready handshake |

**Latency:** 4 clock cycles  
**Throughput:** 1 MAC/cycle (when `ready_i` is continuously asserted)

## 4. Functional Arithmetic Specification

### 4.1 Input Handling & Flush-to-Zero (FTZ)
- **Subnormal Detection:** FP16 subnormals are identified by `exp[4:0] == 5'b00000 && man[9:0] != 10'b0`. 
- **FTZ Policy:** Upon detection, the operand is replaced with exact zero (`exp=0, man=0`). No normalization or hidden-bit insertion is performed for subnormals. This eliminates critical-path subnormal normalization logic in S0.
- **Special Values:** Zeros, Infinities, and NaNs are tagged via control signals `is_zero`, `is_inf`, `is_nan`. These bypass arithmetic units where applicable to preserve timing.

### 4.2 Multiplication Unit (Stage 1)
- **Exponent Arithmetic:** `exp_prod = exp_a + exp_b - 15` (FP16 bias removal). Result is sign-extended to 8 bits to match FP32 exponent width.
- **Mantissa Multiplication:** Effective mantissas include the implicit leading bit for normals: `m_eff = {1'b1, man[9:0]}`. A dedicated 11×11 bit array multiplier computes `man_prod = m_a_eff × m_b_eff`, yielding a 22-bit product.
- **Sign:** `sign_prod = sign_a ^ sign_b`.
- **Pre-Normalization:** The product mantissa is left-aligned by 1 position (since `1.M × 1.M` ranges from `1.0...0` to `1.11...1 × 1.11...1 ≈ 3.99`). A single-cycle left shift and exponent decrement prepares the format for Stage 2 alignment.

### 4.3 Alignment & Addition Unit (Stage 2)
- **Exponent Comparison:** `exp_diff = exp_prod - exp_c`. If `exp_diff < 0`, C's mantissa is shifted right; otherwise, product mantissa shifts right. Shift amount is clamped to ≤24 to prevent precision loss beyond sticky bit generation.
- **Mantissa Alignment:** The smaller operand's mantissa is right-shifted using a barrel shifter. Lower bits are OR-reduced to form the initial Sticky bit.
- **Addition:** Aligned mantissas (extended to 25 bits: 23 significant + Guard + Round) are added/subtracted based on sign comparison. Two's complement addition handles mixed signs natively. Carry-out is captured for overflow detection.

### 4.4 Normalization & RNE Rounding (Stage 3)
- **Normalization:** 
  - *Left Shift:* If MSB of sum mantissa is 0, a Leading Zero Counter (LZC) determines shift amount. Mantissa left-shifts, exponent decrements accordingly.
  - *Right Shift:* If addition generates a carry into bit 24, mantissa right-shifts by 1, exponent increments.
- **RNE Rounding Logic:** After normalization, the mantissa is truncated to 23 bits. Three LSBs are extracted:
  - `Round (R)`: Bit 23 (immediately after truncation point)
  - `Tie (T)`: Bit 24 (MSB of discarded portion)
  - `Sticky (S)`: OR-reduction of all bits below T
  - **RNE Decision:** If `R == 1 && (S == 1 || T == 1)`, increment the 23-bit mantissa. This may trigger a secondary right-shift and exponent increment if rounding causes overflow into bit 24.
- **Packing:** Final sign, biased exponent, and rounded mantissa are concatenated into IEEE 754 FP32 format.

## 5. Exception Handling & Flag Generation
Exception flags are computed combinatorially in Stage 3 and latched with the output. Flags follow IEEE 754 semantics adapted for FTZ/RNE constraints.

| Exception | Detection Condition | Action on `d_o` | Flag Behavior |
|-----------|---------------------|-----------------|---------------|
| **Invalid** (`flags_o[0]`) | - Any input is NaN (QNaN or SNaN)<br>- `Inf × 0` or `0 × Inf`<br>- `Inf + (-Inf)` during accumulation | Output Quiet NaN (`0x7FC00000` or sign-preserving) | Asserted for the cycle of result output |
| **Overflow** (`flags_o[1]`) | Normalized exponent > 254 (FP32 max) after rounding | Output signed Infinity (`sign || {8'hFF, 23'h0}`) | Asserted; RNE rounding applied before clamping to Inf |
| **Underflow** (`flags_o[2]`) | Normalized result magnitude < `2^-126` (min FP32 normal) AND inexact after rounding | Flush to signed Zero (`sign || 31'h0`) | Asserted only if result is tiny and inexact. FTZ policy applies; no subnormal output generated. |

**Flag Latching Rule:** Flags correspond strictly to the `d_o` value on the same cycle. If backpressure stalls Stage 3, flags hold until `ready_i` asserts.

## 6. Timing, Backpressure & Performance
- **Critical Path:** Stage 2 (Alignment + 25-bit Adder) and Stage 3 (LZC + RNE logic) are typically timing-critical. Barrel shifter depth is limited to 24 bits; LZC uses a tree-based O(log N) structure for predictable delay.
- **Backpressure Propagation:** `ready_o` is generated as `~pipe_full[0]`. Inter-stage ready signals (`rdy_1`, `rdy_2`, `rdy_3`) are derived from downstream readiness. Pipeline registers use `en = valid_stage && rdy_next` to prevent overwriting stalled data.
- **Frequency Target:** Balanced stage delays enable >800 MHz on 5nm/7nm nodes. Combinational logic per stage is capped at ~1.2 ns equivalent delay.

## 7. RTL Implementation Guidelines
1. **Register Clustering:** Group pipeline registers with their combinational logic to minimize routing congestion. Use `(* pipeline_reg *)` or vendor-specific attributes for retiming control.
2. **Multiplier Selection:** Instantiate a dedicated DSP/ALU macro for the 11×11 bit multiplication if available. Otherwise, use a Wallace-tree multiplier optimized for area-delay product.
3. **LZC Implementation:** Use a hierarchical LZC (e.g., 8→4→2→1) rather than a linear counter to guarantee logarithmic delay. Mask leading zeros based on sign-extension alignment.
4. **RNE Combinational Block:** Implement rounding as a pure combinational block post-adder. Avoid sequential rounding states to maintain single-cycle Stage 3 throughput.
5. **Reset Strategy:** Synchronous reset deassertion only. Pipeline registers clear to zero; control flags default to `3'b0`. Output drives `valid_o=0` during reset.
6. **Verification Hooks:** Provide internal test ports for injecting special FP16/FP32 patterns (NaN, Inf, subnormals) and bypassing FTZ for corner-case validation.

---
**Document Version:** 1.0  
**Target Technology:** Advanced FinFET/GAA AI Accelerator PDKs  
**Compliance:** IEEE 754-2019 (FP16/FP32), RNE, FTZ, Valid/Ready AXI-Stream Compatible