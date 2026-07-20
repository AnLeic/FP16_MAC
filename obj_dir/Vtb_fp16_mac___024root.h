// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtb_fp16_mac.h for the primary calling header

#ifndef VERILATED_VTB_FP16_MAC___024ROOT_H_
#define VERILATED_VTB_FP16_MAC___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vtb_fp16_mac__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtb_fp16_mac___024root final {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ tb_fp16_mac__DOT__clk;
        CData/*0:0*/ tb_fp16_mac__DOT__rst_n;
        CData/*0:0*/ tb_fp16_mac__DOT__valid_i;
        CData/*0:0*/ tb_fp16_mac__DOT__ready_i;
        CData/*0:0*/ tb_fp16_mac__DOT__overflow_o;
        CData/*0:0*/ tb_fp16_mac__DOT__underflow_o;
        CData/*0:0*/ tb_fp16_mac__DOT__invalid_o;
        CData/*3:0*/ tb_fp16_mac__DOT__uut__DOT__valid_q;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__stall;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__a_is_sub;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__b_is_sub;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__c_is_sub;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__a_is_inf;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__b_is_inf;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__a_zero_f;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__b_zero_f;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s1_a_sign;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s1_b_sign;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s1_c_sign;
        CData/*4:0*/ tb_fp16_mac__DOT__uut__DOT__s1_a_exp;
        CData/*4:0*/ tb_fp16_mac__DOT__uut__DOT__s1_b_exp;
        CData/*7:0*/ tb_fp16_mac__DOT__uut__DOT__s1_c_exp;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s1_a_zero;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s1_b_zero;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s1_c_zero;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s1_a_inf;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s1_b_inf;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s1_c_inf;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s1_invalid;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s2_prod_sign;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s2_c_sign;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s2_prod_zero;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s2_c_zero;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s2_a_inf;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s2_b_inf;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s2_c_inf;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s2_invalid;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__prod_ge_c;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__shift_sticky;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__sum_sign;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s3_sign;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s3_sticky_in;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s3_is_zero;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s3_a_inf;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s3_b_inf;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s3_c_inf;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__s3_invalid;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__result_overflow;
        CData/*0:0*/ tb_fp16_mac__DOT__uut__DOT__result_underflow;
        CData/*7:0*/ tb_fp16_mac__DOT__uut__DOT__packed_exp;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __VstlPhaseResult;
        CData/*0:0*/ __Vtrigprevexpr___TOP__tb_fp16_mac__DOT__clk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__tb_fp16_mac__DOT__rst_n__0;
        CData/*0:0*/ __Vtrigprevexpr_h74914845__0;
        CData/*0:0*/ __Vtrigprevexpr_h08083ddc__1;
        CData/*0:0*/ __VactPhaseResult;
        CData/*0:0*/ __VinactPhaseResult;
        CData/*0:0*/ __VnbaPhaseResult;
        SData/*15:0*/ tb_fp16_mac__DOT__a_i;
        SData/*15:0*/ tb_fp16_mac__DOT__b_i;
        SData/*10:0*/ tb_fp16_mac__DOT__uut__DOT__s1_a_mant;
        SData/*10:0*/ tb_fp16_mac__DOT__uut__DOT__s1_b_mant;
        SData/*9:0*/ tb_fp16_mac__DOT__uut__DOT__s2_prod_exp;
    };
    struct {
        SData/*9:0*/ tb_fp16_mac__DOT__uut__DOT__s2_c_exp;
        SData/*9:0*/ tb_fp16_mac__DOT__uut__DOT__s3_exp;
        IData/*31:0*/ tb_fp16_mac__DOT__c_i;
        IData/*31:0*/ tb_fp16_mac__DOT__d_o;
        IData/*22:0*/ tb_fp16_mac__DOT__uut__DOT__s1_c_mant;
        IData/*21:0*/ tb_fp16_mac__DOT__uut__DOT__raw_product;
        IData/*25:0*/ tb_fp16_mac__DOT__uut__DOT__s2_prod_frac;
        IData/*25:0*/ tb_fp16_mac__DOT__uut__DOT__s2_c_frac;
        IData/*27:0*/ tb_fp16_mac__DOT__uut__DOT__sum_mag;
        IData/*27:0*/ tb_fp16_mac__DOT__uut__DOT__s3_sum_mag;
        IData/*22:0*/ tb_fp16_mac__DOT__uut__DOT__packed_mant;
        IData/*31:0*/ tb_fp16_mac__DOT__uut__DOT__unnamedblk1__DOT__i;
        IData/*31:0*/ __VactIterCount;
        IData/*31:0*/ __VinactIterCount;
        IData/*31:0*/ __Vi;
        VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VactTriggeredAcc;
        VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;
    };
    VlDelayScheduler __VdlySched;
    VlTriggerScheduler __VtrigSched_h56ff3649__0;
    VlTriggerScheduler __VtrigSched_hf0a630d2__0;
    VlTriggerScheduler __VtrigSched_h5faf2a49__0;

    // INTERNAL VARIABLES
    Vtb_fp16_mac__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vtb_fp16_mac___024root(Vtb_fp16_mac__Syms* symsp, const char* namep);
    ~Vtb_fp16_mac___024root();
    VL_UNCOPYABLE(Vtb_fp16_mac___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
