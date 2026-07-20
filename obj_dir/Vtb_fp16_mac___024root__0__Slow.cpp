// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_fp16_mac.h for the primary calling header

#include "Vtb_fp16_mac__pch.h"

void Vtb_fp16_mac___024root___timing_ready(Vtb_fp16_mac___024root* vlSelf);

VL_ATTR_COLD void Vtb_fp16_mac___024root___eval_static(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_static\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (8ULL | vlSelfRef.__VactTriggered[0U]);
    vlSelfRef.__VactTriggered[0U] = (0x0000000000000010ULL 
                                     | vlSelfRef.__VactTriggered[0U]);
    vlSelfRef.__Vtrigprevexpr___TOP__tb_fp16_mac__DOT__clk__0 
        = vlSelfRef.tb_fp16_mac__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_fp16_mac__DOT__rst_n__0 
        = vlSelfRef.tb_fp16_mac__DOT__rst_n;
    vlSelfRef.__Vtrigprevexpr_h74914845__0 = (1U & 
                                              ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                                               >> 3U));
    vlSelfRef.__Vtrigprevexpr_h08083ddc__1 = (1U & 
                                              (~ ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                                                  >> 3U)));
    Vtb_fp16_mac___024root___timing_ready(vlSelf);
    do {
        vlSelfRef.__VactTriggeredAcc[vlSelfRef.__Vi] 
            = vlSelfRef.__VactTriggered[vlSelfRef.__Vi];
        vlSelfRef.__Vi = ((IData)(1U) + vlSelfRef.__Vi);
    } while ((0U >= vlSelfRef.__Vi));
}

VL_ATTR_COLD void Vtb_fp16_mac___024root___eval_final(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_final\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_fp16_mac___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtb_fp16_mac___024root___eval_phase__stl(Vtb_fp16_mac___024root* vlSelf);

VL_ATTR_COLD void Vtb_fp16_mac___024root___eval_settle(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_settle\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vtb_fp16_mac___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("output/testbench.sv", 7, "", "DIDNOTCONVERGE: Settle region did not converge after '--converge-limit' of 10000 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        vlSelfRef.__VstlPhaseResult = Vtb_fp16_mac___024root___eval_phase__stl(vlSelf);
        vlSelfRef.__VstlFirstIteration = 0U;
    } while (vlSelfRef.__VstlPhaseResult);
}

VL_ATTR_COLD void Vtb_fp16_mac___024root___eval_triggers_vec__stl(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_triggers_vec__stl\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered[0U]) 
                                     | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
}

VL_ATTR_COLD bool Vtb_fp16_mac___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_fp16_mac___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vtb_fp16_mac___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vtb_fp16_mac___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___trigger_anySet__stl\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

VL_ATTR_COLD void Vtb_fp16_mac___024root___stl_sequent__TOP__0(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___stl_sequent__TOP__0\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    SData/*9:0*/ tb_fp16_mac__DOT__uut__DOT__exp_diff;
    tb_fp16_mac__DOT__uut__DOT__exp_diff = 0;
    SData/*9:0*/ tb_fp16_mac__DOT__uut__DOT__shift_amt;
    tb_fp16_mac__DOT__uut__DOT__shift_amt = 0;
    IData/*26:0*/ tb_fp16_mac__DOT__uut__DOT__prod_mag;
    tb_fp16_mac__DOT__uut__DOT__prod_mag = 0;
    IData/*26:0*/ tb_fp16_mac__DOT__uut__DOT__c_mag;
    tb_fp16_mac__DOT__uut__DOT__c_mag = 0;
    IData/*26:0*/ tb_fp16_mac__DOT__uut__DOT__big_mag;
    tb_fp16_mac__DOT__uut__DOT__big_mag = 0;
    IData/*26:0*/ tb_fp16_mac__DOT__uut__DOT__small_mag;
    tb_fp16_mac__DOT__uut__DOT__small_mag = 0;
    CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__big_sign;
    tb_fp16_mac__DOT__uut__DOT__big_sign = 0;
    CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__small_sign;
    tb_fp16_mac__DOT__uut__DOT__small_sign = 0;
    IData/*26:0*/ tb_fp16_mac__DOT__uut__DOT__small_shifted;
    tb_fp16_mac__DOT__uut__DOT__small_shifted = 0;
    SData/*9:0*/ tb_fp16_mac__DOT__uut__DOT__clamped_shift;
    tb_fp16_mac__DOT__uut__DOT__clamped_shift = 0;
    CData/*4:0*/ tb_fp16_mac__DOT__uut__DOT__lead_zero;
    tb_fp16_mac__DOT__uut__DOT__lead_zero = 0;
    SData/*9:0*/ tb_fp16_mac__DOT__uut__DOT__norm_exp;
    tb_fp16_mac__DOT__uut__DOT__norm_exp = 0;
    IData/*27:0*/ tb_fp16_mac__DOT__uut__DOT__norm_mag;
    tb_fp16_mac__DOT__uut__DOT__norm_mag = 0;
    CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__norm_sticky;
    tb_fp16_mac__DOT__uut__DOT__norm_sticky = 0;
    SData/*10:0*/ tb_fp16_mac__DOT__uut__DOT__final_exp_biased;
    tb_fp16_mac__DOT__uut__DOT__final_exp_biased = 0;
    CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__special_inf;
    tb_fp16_mac__DOT__uut__DOT__special_inf = 0;
    IData/*23:0*/ __VdfgRegularize_h6e95ff9d_0_4;
    __VdfgRegularize_h6e95ff9d_0_4 = 0;
    // Body
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__c_is_sub 
        = (IData)(((0U == (0x7f800000U & vlSelfRef.tb_fp16_mac__DOT__c_i)) 
                   & (0U != (0x007fffffU & vlSelfRef.tb_fp16_mac__DOT__c_i))));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_inf 
        = (IData)((0x7c00U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i))));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_inf 
        = (IData)((0x7c00U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i))));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__stall = (
                                                   (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__ready_i)) 
                                                   & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                                                      >> 3U));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__raw_product 
        = (0x003fffffU & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_b_mant) 
                          * (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_a_mant)));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_sub 
        = (IData)(((0U == (0x7c00U & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i))) 
                   & (0U != (0x000003ffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i)))));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_sub 
        = (IData)(((0U == (0x7c00U & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i))) 
                   & (0U != (0x000003ffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i)))));
    tb_fp16_mac__DOT__uut__DOT__special_inf = ((~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_invalid)) 
                                               & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_a_inf) 
                                                  | ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_b_inf) 
                                                     | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_c_inf))));
    tb_fp16_mac__DOT__uut__DOT__prod_mag = ((0x04000000U 
                                             | vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_frac) 
                                            & (- (IData)(
                                                         (1U 
                                                          & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_zero))))));
    tb_fp16_mac__DOT__uut__DOT__c_mag = ((0x04000000U 
                                          | vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_frac) 
                                         & (- (IData)(
                                                      (1U 
                                                       & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_zero))))));
    tb_fp16_mac__DOT__uut__DOT__lead_zero = 0x1bU;
    tb_fp16_mac__DOT__uut__DOT__norm_sticky = (1U & 
                                               ((0x08000000U 
                                                 & vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sum_mag)
                                                 ? 
                                                (vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sum_mag 
                                                 | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sticky_in))
                                                 : (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sticky_in)));
    if ((1U & (~ (vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sum_mag 
                  >> 0x1bU)))) {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__unnamedblk1__DOT__i = 0x0000001aU;
        {
            while (VL_LTES_III(32, 0U, vlSelfRef.tb_fp16_mac__DOT__uut__DOT__unnamedblk1__DOT__i)) {
                if (((0x1bU >= (0x0000001fU & vlSelfRef.tb_fp16_mac__DOT__uut__DOT__unnamedblk1__DOT__i)) 
                     && (1U & (vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sum_mag 
                               >> (0x0000001fU & vlSelfRef.tb_fp16_mac__DOT__uut__DOT__unnamedblk1__DOT__i))))) {
                    tb_fp16_mac__DOT__uut__DOT__lead_zero 
                        = (0x0000001fU & ((IData)(0x1aU) 
                                          - vlSelfRef.tb_fp16_mac__DOT__uut__DOT__unnamedblk1__DOT__i));
                    goto __Vlabel0;
                }
                vlSelfRef.tb_fp16_mac__DOT__uut__DOT__unnamedblk1__DOT__i 
                    = (vlSelfRef.tb_fp16_mac__DOT__uut__DOT__unnamedblk1__DOT__i 
                       - (IData)(1U));
            }
            __Vlabel0: ;
        }
    }
    if ((0x08000000U & vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sum_mag)) {
        tb_fp16_mac__DOT__uut__DOT__norm_exp = (0x000003ffU 
                                                & ((IData)(1U) 
                                                   + 
                                                   VL_EXTENDS_II(10,10, (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_exp))));
        tb_fp16_mac__DOT__uut__DOT__norm_mag = (0x0fffffffU 
                                                & VL_SHIFTR_III(28,28,32, vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sum_mag, 1U));
    } else if ((0U == (IData)(tb_fp16_mac__DOT__uut__DOT__lead_zero))) {
        tb_fp16_mac__DOT__uut__DOT__norm_exp = (0x000003ffU 
                                                & (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_exp));
        tb_fp16_mac__DOT__uut__DOT__norm_mag = (0x0fffffffU 
                                                & vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sum_mag);
    } else {
        tb_fp16_mac__DOT__uut__DOT__norm_exp = (0x000003ffU 
                                                & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_exp) 
                                                   - (IData)(tb_fp16_mac__DOT__uut__DOT__lead_zero)));
        tb_fp16_mac__DOT__uut__DOT__norm_mag = (0x0fffffffU 
                                                & (vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sum_mag 
                                                   << (IData)(tb_fp16_mac__DOT__uut__DOT__lead_zero)));
    }
    tb_fp16_mac__DOT__uut__DOT__exp_diff = (0x000003ffU 
                                            & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_exp) 
                                               - (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_exp)));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_zero_f 
        = ((IData)((0U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i)))) 
           | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_sub));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_zero_f 
        = ((IData)((0U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i)))) 
           | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_sub));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__prod_ge_c 
        = ((~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_zero)) 
           & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_zero) 
              | VL_LTES_III(32, 0U, VL_EXTENDS_II(32,10, (IData)(tb_fp16_mac__DOT__uut__DOT__exp_diff)))));
    __VdfgRegularize_h6e95ff9d_0_4 = (0x00ffffffU & 
                                      ((1U & ((IData)(
                                                      ((0U 
                                                        != 
                                                        (0x0000000bU 
                                                         & tb_fp16_mac__DOT__uut__DOT__norm_mag)) 
                                                       | (IData)(tb_fp16_mac__DOT__uut__DOT__norm_sticky))) 
                                              & (tb_fp16_mac__DOT__uut__DOT__norm_mag 
                                                 >> 2U))) 
                                       + (0x007fffffU 
                                          & (tb_fp16_mac__DOT__uut__DOT__norm_mag 
                                             >> 3U))));
    if (vlSelfRef.tb_fp16_mac__DOT__uut__DOT__prod_ge_c) {
        tb_fp16_mac__DOT__uut__DOT__big_mag = tb_fp16_mac__DOT__uut__DOT__prod_mag;
        tb_fp16_mac__DOT__uut__DOT__big_sign = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_sign;
        tb_fp16_mac__DOT__uut__DOT__small_sign = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_sign;
        tb_fp16_mac__DOT__uut__DOT__small_mag = tb_fp16_mac__DOT__uut__DOT__c_mag;
        tb_fp16_mac__DOT__uut__DOT__shift_amt = (0x000003ffU 
                                                 & (IData)(tb_fp16_mac__DOT__uut__DOT__exp_diff));
    } else {
        tb_fp16_mac__DOT__uut__DOT__big_mag = tb_fp16_mac__DOT__uut__DOT__c_mag;
        tb_fp16_mac__DOT__uut__DOT__big_sign = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_sign;
        tb_fp16_mac__DOT__uut__DOT__small_sign = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_sign;
        tb_fp16_mac__DOT__uut__DOT__small_mag = tb_fp16_mac__DOT__uut__DOT__prod_mag;
        tb_fp16_mac__DOT__uut__DOT__shift_amt = (0x000003ffU 
                                                 & (- (IData)(tb_fp16_mac__DOT__uut__DOT__exp_diff)));
    }
    tb_fp16_mac__DOT__uut__DOT__final_exp_biased = 
        (0x000007ffU & ((IData)(0x007fU) + VL_EXTENDS_II(11,10, 
                                                         (0x000003ffU 
                                                          & ((IData)(tb_fp16_mac__DOT__uut__DOT__norm_exp) 
                                                             + 
                                                             (1U 
                                                              & (- (IData)(
                                                                           (1U 
                                                                            & (__VdfgRegularize_h6e95ff9d_0_4 
                                                                               >> 0x00000017U))))))))));
    tb_fp16_mac__DOT__uut__DOT__clamped_shift = ((0x001bU 
                                                  < (IData)(tb_fp16_mac__DOT__uut__DOT__shift_amt))
                                                  ? 0x001bU
                                                  : (IData)(tb_fp16_mac__DOT__uut__DOT__shift_amt));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__result_overflow 
        = ((~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_is_zero)) 
           & VL_LTES_III(11, 0x00ffU, (IData)(tb_fp16_mac__DOT__uut__DOT__final_exp_biased)));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__result_underflow 
        = ((~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_is_zero)) 
           & VL_GTES_III(11, 0U, (IData)(tb_fp16_mac__DOT__uut__DOT__final_exp_biased)));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__shift_sticky 
        = ((0U != (IData)(tb_fp16_mac__DOT__uut__DOT__clamped_shift)) 
           & (0U != (tb_fp16_mac__DOT__uut__DOT__small_mag 
                     & VL_SHIFTR_III(27,27,10, 0x07ffffffU, 
                                     (0x000003ffU & 
                                      ((IData)(0x001bU) 
                                       - (IData)(tb_fp16_mac__DOT__uut__DOT__clamped_shift)))))));
    tb_fp16_mac__DOT__uut__DOT__small_shifted = VL_SHIFTR_III(27,27,10, tb_fp16_mac__DOT__uut__DOT__small_mag, (IData)(tb_fp16_mac__DOT__uut__DOT__clamped_shift));
    if (vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_invalid) {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__packed_exp = 0x000000ffU;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__packed_mant = 0x00400000U;
    } else if (((IData)(tb_fp16_mac__DOT__uut__DOT__special_inf) 
                | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__result_overflow))) {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__packed_exp = 0x000000ffU;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__packed_mant = 0U;
    } else if (((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_is_zero) 
                | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__result_underflow))) {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__packed_exp = 0U;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__packed_mant = 0U;
    } else {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__packed_exp 
            = (0x000000ffU & (IData)(tb_fp16_mac__DOT__uut__DOT__final_exp_biased));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__packed_mant 
            = (0x007fffffU & __VdfgRegularize_h6e95ff9d_0_4);
    }
    if (((IData)(tb_fp16_mac__DOT__uut__DOT__big_sign) 
         == (IData)(tb_fp16_mac__DOT__uut__DOT__small_sign))) {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__sum_mag 
            = (0x0fffffffU & (tb_fp16_mac__DOT__uut__DOT__big_mag 
                              + tb_fp16_mac__DOT__uut__DOT__small_shifted));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__sum_sign 
            = tb_fp16_mac__DOT__uut__DOT__big_sign;
    } else if ((tb_fp16_mac__DOT__uut__DOT__big_mag 
                > tb_fp16_mac__DOT__uut__DOT__small_shifted)) {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__sum_mag 
            = (0x0fffffffU & (tb_fp16_mac__DOT__uut__DOT__big_mag 
                              - tb_fp16_mac__DOT__uut__DOT__small_shifted));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__sum_sign 
            = tb_fp16_mac__DOT__uut__DOT__big_sign;
    } else if ((tb_fp16_mac__DOT__uut__DOT__big_mag 
                == tb_fp16_mac__DOT__uut__DOT__small_shifted)) {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__sum_mag 
            = (0x0fffffffU & 0U);
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__sum_sign 
            = ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__shift_sticky) 
               & (IData)(tb_fp16_mac__DOT__uut__DOT__small_sign));
    } else {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__sum_mag 
            = (0x0fffffffU & (tb_fp16_mac__DOT__uut__DOT__small_shifted 
                              - tb_fp16_mac__DOT__uut__DOT__big_mag));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__sum_sign 
            = tb_fp16_mac__DOT__uut__DOT__small_sign;
    }
}

VL_ATTR_COLD void Vtb_fp16_mac___024root___eval_stl(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_stl\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        Vtb_fp16_mac___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD bool Vtb_fp16_mac___024root___eval_phase__stl(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_phase__stl\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtb_fp16_mac___024root___eval_triggers_vec__stl(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_fp16_mac___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
    __VstlExecute = Vtb_fp16_mac___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vtb_fp16_mac___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vtb_fp16_mac___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_fp16_mac___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vtb_fp16_mac___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge tb_fp16_mac.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @(negedge tb_fp16_mac.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 2U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 2 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
    if ((1U & (IData)((triggers[0U] >> 3U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 3 is active: @( tb_fp16_mac.uut.valid_q[3])\n");
    }
    if ((1U & (IData)((triggers[0U] >> 4U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 4 is active: @( (~ tb_fp16_mac.uut.valid_q[3]))\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb_fp16_mac___024root___ctor_var_reset(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___ctor_var_reset\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->tb_fp16_mac__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8251767146676800396ull);
    vlSelf->tb_fp16_mac__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3462428487158280040ull);
    vlSelf->tb_fp16_mac__DOT__a_i = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 1374876208586851482ull);
    vlSelf->tb_fp16_mac__DOT__b_i = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 9226794615923833087ull);
    vlSelf->tb_fp16_mac__DOT__c_i = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18396736646426988807ull);
    vlSelf->tb_fp16_mac__DOT__valid_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13420606042707742371ull);
    vlSelf->tb_fp16_mac__DOT__d_o = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7358700403911756048ull);
    vlSelf->tb_fp16_mac__DOT__ready_i = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17594928243450699179ull);
    vlSelf->tb_fp16_mac__DOT__overflow_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16615070537223102333ull);
    vlSelf->tb_fp16_mac__DOT__underflow_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4754317736355237914ull);
    vlSelf->tb_fp16_mac__DOT__invalid_o = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13785505185636409116ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__valid_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 1796257374523746540ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__stall = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10461626156025665752ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__a_is_sub = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10630500720320359433ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__b_is_sub = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10747598836586556932ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__c_is_sub = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17751983669597836928ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__a_is_inf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16403975924735423673ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__b_is_inf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15305493685360657617ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__a_zero_f = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15558292673898454451ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__b_zero_f = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7164495769348759272ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_a_sign = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11217278999777300747ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_b_sign = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4674369435395896316ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_c_sign = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9277231012445414625ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_a_exp = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 12864646989026691425ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_b_exp = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 9914268598266637362ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_a_mant = VL_SCOPED_RAND_RESET_I(11, __VscopeHash, 3758869833341642207ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_b_mant = VL_SCOPED_RAND_RESET_I(11, __VscopeHash, 5252910732900404450ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_c_exp = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 15816747528009618062ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_c_mant = VL_SCOPED_RAND_RESET_I(23, __VscopeHash, 2748842996446123048ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_a_zero = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6240093512715292187ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_b_zero = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13569113640733350618ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_c_zero = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3432135854811006530ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_a_inf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12052778287598041455ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_b_inf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7008133876424284157ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_c_inf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15485606069775103418ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s1_invalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3028788656819381462ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__raw_product = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 7854428994879757952ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s2_prod_sign = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18260528392139756811ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s2_c_sign = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14127094595642481692ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s2_prod_exp = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 13311315068663754894ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s2_c_exp = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 12395666432688267624ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s2_prod_frac = VL_SCOPED_RAND_RESET_I(26, __VscopeHash, 2828661579224079689ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s2_c_frac = VL_SCOPED_RAND_RESET_I(26, __VscopeHash, 5481886892686459257ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s2_prod_zero = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15126296719432068922ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s2_c_zero = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17481091933602400972ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s2_a_inf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 866377221652451460ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s2_b_inf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2685959822102431238ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s2_c_inf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8927512007485075330ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s2_invalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15979526592126973213ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__prod_ge_c = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17905921734686401983ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__shift_sticky = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6164639836985959034ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__sum_mag = VL_SCOPED_RAND_RESET_I(28, __VscopeHash, 4823611653236647566ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__sum_sign = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13325664277605226895ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s3_sign = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16620314196844865235ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s3_exp = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 8397516813316011705ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s3_sum_mag = VL_SCOPED_RAND_RESET_I(28, __VscopeHash, 10097259018413998896ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s3_sticky_in = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4039461427294263475ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s3_is_zero = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8393873639777478251ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s3_a_inf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14358126838286283836ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s3_b_inf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3081969656168523901ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s3_c_inf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15893444658821926118ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__s3_invalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10616520808702291525ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__result_overflow = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5862148183359902160ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__result_underflow = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9311477848346084974ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__packed_exp = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 16370496158174103967ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__packed_mant = VL_SCOPED_RAND_RESET_I(23, __VscopeHash, 14149487416790290483ull);
    vlSelf->tb_fp16_mac__DOT__uut__DOT__unnamedblk1__DOT__i = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggeredAcc[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__tb_fp16_mac__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__tb_fp16_mac__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr_h74914845__0 = 0;
    vlSelf->__Vtrigprevexpr_h08083ddc__1 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
    vlSelf->__Vi = 0;
}
