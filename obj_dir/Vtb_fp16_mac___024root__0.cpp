// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_fp16_mac.h for the primary calling header

#include "Vtb_fp16_mac__pch.h"

VlCoroutine Vtb_fp16_mac___024root___eval_initial__TOP__Vtiming__0(Vtb_fp16_mac___024root* vlSelf);
VlCoroutine Vtb_fp16_mac___024root___eval_initial__TOP__Vtiming__1(Vtb_fp16_mac___024root* vlSelf);
VlCoroutine Vtb_fp16_mac___024root___eval_initial__TOP__Vtiming__2(Vtb_fp16_mac___024root* vlSelf);

void Vtb_fp16_mac___024root___eval_initial(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_initial\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtb_fp16_mac___024root___eval_initial__TOP__Vtiming__0(vlSelf);
    Vtb_fp16_mac___024root___eval_initial__TOP__Vtiming__1(vlSelf);
    Vtb_fp16_mac___024root___eval_initial__TOP__Vtiming__2(vlSelf);
}

VlCoroutine Vtb_fp16_mac___024root___eval_initial__TOP__Vtiming__0(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_initial__TOP__Vtiming__0\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_fp16_mac__DOT__clk = 0U;
    while (true) {
        co_await vlSelfRef.__VdlySched.delay(5ULL, 
                                             nullptr, 
                                             "output/testbench.sv", 
                                             51);
        vlSelfRef.tb_fp16_mac__DOT__clk = (1U & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__clk)));
    }
    co_return;
}

VlCoroutine Vtb_fp16_mac___024root___eval_initial__TOP__Vtiming__1(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_initial__TOP__Vtiming__1\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_fp16_mac__DOT__rst_n = 0U;
    co_await vlSelfRef.__VdlySched.delay(0x0000000000000014ULL, 
                                         nullptr, "output/testbench.sv", 
                                         59);
    vlSelfRef.tb_fp16_mac__DOT__rst_n = 1U;
    co_await vlSelfRef.__VdlySched.delay(0x0000000000000032ULL, 
                                         nullptr, "output/testbench.sv", 
                                         62);
    VL_WRITEF_NX("Testbench initialized at time %t\n",2, 'T',-12
                 , '#',64,VL_TIME_UNITED_Q(1));
    co_return;
}

void Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(Vtb_fp16_mac___024root* vlSelf, const char* __VeventDescription);
void Vtb_fp16_mac___024root____VbeforeTrig_hf0a630d2__0(Vtb_fp16_mac___024root* vlSelf, const char* __VeventDescription);
void Vtb_fp16_mac___024root____VbeforeTrig_h5faf2a49__0(Vtb_fp16_mac___024root* vlSelf, const char* __VeventDescription);

VlCoroutine Vtb_fp16_mac___024root___eval_initial__TOP__Vtiming__2(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_initial__TOP__Vtiming__2\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_fp16_mac__DOT__a_i = 0U;
    vlSelfRef.tb_fp16_mac__DOT__b_i = 0U;
    vlSelfRef.tb_fp16_mac__DOT__c_i = 0U;
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    vlSelfRef.tb_fp16_mac__DOT__ready_i = 1U;
    co_await vlSelfRef.__VdlySched.delay(0x0000000000000032ULL, 
                                         nullptr, "output/testbench.sv", 
                                         77);
    VL_WRITEF_NX("Starting test sequence at time %t\n",2, 'T',-12
                 , '#',64,VL_TIME_UNITED_Q(1));
    VL_WRITEF_NX("Test 1: Basic operation - 1.0 * 2.0 + 3.0 = 5.0\n",0);
    vlSelfRef.tb_fp16_mac__DOT__a_i = 0x3c00U;
    vlSelfRef.tb_fp16_mac__DOT__b_i = 0x4000U;
    vlSelfRef.tb_fp16_mac__DOT__c_i = 0x40400000U;
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 1U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         123);
    co_await vlSelfRef.__VdlySched.delay(1ULL, nullptr, 
                                         "output/testbench.sv", 
                                         124);
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    while ((1U & (~ ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                     >> 3U)))) {
        Vtb_fp16_mac___024root____VbeforeTrig_hf0a630d2__0(vlSelf, 
                                                           "@( tb_fp16_mac.uut.valid_q[3])");
        co_await vlSelfRef.__VtrigSched_hf0a630d2__0.trigger(1U, 
                                                             nullptr, 
                                                             "@( tb_fp16_mac.uut.valid_q[3])", 
                                                             "output/testbench.sv", 
                                                             127);
    }
    if ((0x40a00000U == vlSelfRef.tb_fp16_mac__DOT__d_o)) {
        VL_WRITEF_NX("PASS: Basic operation result = 5.0\n",0);
    } else {
        VL_WRITEF_NX("FAIL: Basic operation result = %h, expected = 40A00000\n",1
                     , '#',32,vlSelfRef.tb_fp16_mac__DOT__d_o);
    }
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         137);
    VL_WRITEF_NX("Test 2: Zero multiplication\n",0);
    vlSelfRef.tb_fp16_mac__DOT__a_i = 0U;
    vlSelfRef.tb_fp16_mac__DOT__b_i = 0x4000U;
    vlSelfRef.tb_fp16_mac__DOT__c_i = 0x40000000U;
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 1U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         151);
    co_await vlSelfRef.__VdlySched.delay(1ULL, nullptr, 
                                         "output/testbench.sv", 
                                         152);
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    while ((1U & (~ ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                     >> 3U)))) {
        Vtb_fp16_mac___024root____VbeforeTrig_hf0a630d2__0(vlSelf, 
                                                           "@( tb_fp16_mac.uut.valid_q[3])");
        co_await vlSelfRef.__VtrigSched_hf0a630d2__0.trigger(1U, 
                                                             nullptr, 
                                                             "@( tb_fp16_mac.uut.valid_q[3])", 
                                                             "output/testbench.sv", 
                                                             155);
    }
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    if ((0x40000000U == vlSelfRef.tb_fp16_mac__DOT__d_o)) {
        VL_WRITEF_NX("PASS: Zero multiplication result = 2.0\n",0);
    } else {
        VL_WRITEF_NX("FAIL: Zero multiplication result = %h, expected = 40000000\n",1
                     , '#',32,vlSelfRef.tb_fp16_mac__DOT__d_o);
    }
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         165);
    VL_WRITEF_NX("Test 3: Subnormal inputs with FTZ\n",0);
    vlSelfRef.tb_fp16_mac__DOT__a_i = 1U;
    vlSelfRef.tb_fp16_mac__DOT__b_i = 0x4000U;
    vlSelfRef.tb_fp16_mac__DOT__c_i = 0x40000000U;
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 1U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         179);
    co_await vlSelfRef.__VdlySched.delay(1ULL, nullptr, 
                                         "output/testbench.sv", 
                                         180);
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    while ((1U & (~ ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                     >> 3U)))) {
        Vtb_fp16_mac___024root____VbeforeTrig_hf0a630d2__0(vlSelf, 
                                                           "@( tb_fp16_mac.uut.valid_q[3])");
        co_await vlSelfRef.__VtrigSched_hf0a630d2__0.trigger(1U, 
                                                             nullptr, 
                                                             "@( tb_fp16_mac.uut.valid_q[3])", 
                                                             "output/testbench.sv", 
                                                             183);
    }
    if ((0x40000000U == vlSelfRef.tb_fp16_mac__DOT__d_o)) {
        VL_WRITEF_NX("PASS: Subnormal input handling result = 2.0\n",0);
    } else {
        VL_WRITEF_NX("FAIL: Subnormal input handling result = %h, expected = 40000000\n",1
                     , '#',32,vlSelfRef.tb_fp16_mac__DOT__d_o);
    }
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         193);
    VL_WRITEF_NX("Test 4: NaN propagation\n",0);
    vlSelfRef.tb_fp16_mac__DOT__a_i = 0x7e00U;
    vlSelfRef.tb_fp16_mac__DOT__b_i = 0x4000U;
    vlSelfRef.tb_fp16_mac__DOT__c_i = 0x40000000U;
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 1U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         207);
    co_await vlSelfRef.__VdlySched.delay(1ULL, nullptr, 
                                         "output/testbench.sv", 
                                         208);
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    while ((1U & (~ ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                     >> 3U)))) {
        Vtb_fp16_mac___024root____VbeforeTrig_hf0a630d2__0(vlSelf, 
                                                           "@( tb_fp16_mac.uut.valid_q[3])");
        co_await vlSelfRef.__VtrigSched_hf0a630d2__0.trigger(1U, 
                                                             nullptr, 
                                                             "@( tb_fp16_mac.uut.valid_q[3])", 
                                                             "output/testbench.sv", 
                                                             211);
    }
    if (vlSelfRef.tb_fp16_mac__DOT__invalid_o) {
        VL_WRITEF_NX("PASS: NaN propagation sets invalid flag\n",0);
    } else {
        VL_WRITEF_NX("FAIL: NaN propagation did not set invalid flag\n",0);
    }
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         221);
    VL_WRITEF_NX("Test 5: Infinity operations\n",0);
    vlSelfRef.tb_fp16_mac__DOT__a_i = 0x7c00U;
    vlSelfRef.tb_fp16_mac__DOT__b_i = 0x4000U;
    vlSelfRef.tb_fp16_mac__DOT__c_i = 0x40000000U;
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 1U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         235);
    co_await vlSelfRef.__VdlySched.delay(1ULL, nullptr, 
                                         "output/testbench.sv", 
                                         236);
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    while ((1U & (~ ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                     >> 3U)))) {
        Vtb_fp16_mac___024root____VbeforeTrig_hf0a630d2__0(vlSelf, 
                                                           "@( tb_fp16_mac.uut.valid_q[3])");
        co_await vlSelfRef.__VtrigSched_hf0a630d2__0.trigger(1U, 
                                                             nullptr, 
                                                             "@( tb_fp16_mac.uut.valid_q[3])", 
                                                             "output/testbench.sv", 
                                                             239);
    }
    if (((IData)((0xff800000U == (0xff800000U & vlSelfRef.tb_fp16_mac__DOT__d_o))) 
         | (IData)((0x7f800000U == (0xff800000U & vlSelfRef.tb_fp16_mac__DOT__d_o))))) {
        VL_WRITEF_NX("PASS: Infinity operation result = infinity\n",0);
    } else {
        VL_WRITEF_NX("FAIL: Infinity operation result = %h\n",1
                     , '#',32,vlSelfRef.tb_fp16_mac__DOT__d_o);
    }
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         250);
    VL_WRITEF_NX("Test 6: Overflow detection\n",0);
    vlSelfRef.tb_fp16_mac__DOT__a_i = 0x7bffU;
    vlSelfRef.tb_fp16_mac__DOT__b_i = 0x7bffU;
    vlSelfRef.tb_fp16_mac__DOT__c_i = 0x7f800000U;
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 1U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         264);
    co_await vlSelfRef.__VdlySched.delay(1ULL, nullptr, 
                                         "output/testbench.sv", 
                                         265);
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    while ((1U & (~ ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                     >> 3U)))) {
        Vtb_fp16_mac___024root____VbeforeTrig_hf0a630d2__0(vlSelf, 
                                                           "@( tb_fp16_mac.uut.valid_q[3])");
        co_await vlSelfRef.__VtrigSched_hf0a630d2__0.trigger(1U, 
                                                             nullptr, 
                                                             "@( tb_fp16_mac.uut.valid_q[3])", 
                                                             "output/testbench.sv", 
                                                             268);
    }
    if (vlSelfRef.tb_fp16_mac__DOT__overflow_o) {
        VL_WRITEF_NX("PASS: Overflow detection works correctly\n",0);
    } else {
        VL_WRITEF_NX("FAIL: Overflow detection did not set overflow flag\n",0);
    }
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         278);
    VL_WRITEF_NX("Test 7: FTZ result is exact zero, underflow must NOT assert\n",0);
    vlSelfRef.tb_fp16_mac__DOT__a_i = 1U;
    vlSelfRef.tb_fp16_mac__DOT__b_i = 1U;
    vlSelfRef.tb_fp16_mac__DOT__c_i = 0U;
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 1U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         293);
    co_await vlSelfRef.__VdlySched.delay(1ULL, nullptr, 
                                         "output/testbench.sv", 
                                         294);
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    while ((1U & (~ ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                     >> 3U)))) {
        Vtb_fp16_mac___024root____VbeforeTrig_hf0a630d2__0(vlSelf, 
                                                           "@( tb_fp16_mac.uut.valid_q[3])");
        co_await vlSelfRef.__VtrigSched_hf0a630d2__0.trigger(1U, 
                                                             nullptr, 
                                                             "@( tb_fp16_mac.uut.valid_q[3])", 
                                                             "output/testbench.sv", 
                                                             297);
    }
    if (((~ (IData)(vlSelfRef.tb_fp16_mac__DOT__underflow_o)) 
         & (0U == vlSelfRef.tb_fp16_mac__DOT__d_o))) {
        VL_WRITEF_NX("PASS: FTZ exact-zero result does not assert underflow\n",0);
    } else {
        VL_WRITEF_NX("FAIL: underflow_o=%b d_o=%h, expected underflow_o=0, d_o=00000000\n",2
                     , '#',1,vlSelfRef.tb_fp16_mac__DOT__underflow_o
                     , '#',32,vlSelfRef.tb_fp16_mac__DOT__d_o);
    }
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         308);
    VL_WRITEF_NX("Test 8: Pipeline stall and resume\n",0);
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         318);
    co_await vlSelfRef.__VdlySched.delay(1ULL, nullptr, 
                                         "output/testbench.sv", 
                                         319);
    vlSelfRef.tb_fp16_mac__DOT__ready_i = 0U;
    vlSelfRef.tb_fp16_mac__DOT__a_i = 0x3c00U;
    vlSelfRef.tb_fp16_mac__DOT__b_i = 0x4000U;
    vlSelfRef.tb_fp16_mac__DOT__c_i = 0x40000000U;
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 1U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         329);
    co_await vlSelfRef.__VdlySched.delay(1ULL, nullptr, 
                                         "output/testbench.sv", 
                                         330);
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    vlSelfRef.tb_fp16_mac__DOT__ready_i = 1U;
    while ((1U & (~ ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                     >> 3U)))) {
        Vtb_fp16_mac___024root____VbeforeTrig_hf0a630d2__0(vlSelf, 
                                                           "@( tb_fp16_mac.uut.valid_q[3])");
        co_await vlSelfRef.__VtrigSched_hf0a630d2__0.trigger(1U, 
                                                             nullptr, 
                                                             "@( tb_fp16_mac.uut.valid_q[3])", 
                                                             "output/testbench.sv", 
                                                             336);
    }
    if ((0x40800000U == vlSelfRef.tb_fp16_mac__DOT__d_o)) {
        VL_WRITEF_NX("PASS: Pipeline stall/resume works correctly\n",0);
    } else {
        VL_WRITEF_NX("FAIL: Pipeline stall/resume result = %h, expected = 40800000\n",1
                     , '#',32,vlSelfRef.tb_fp16_mac__DOT__d_o);
    }
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         346);
    VL_WRITEF_NX("Test 9: Back-to-back operations\n",0);
    vlSelfRef.tb_fp16_mac__DOT__a_i = 0x3c00U;
    vlSelfRef.tb_fp16_mac__DOT__b_i = 0x4000U;
    vlSelfRef.tb_fp16_mac__DOT__c_i = 0x40a00000U;
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 1U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         359);
    co_await vlSelfRef.__VdlySched.delay(1ULL, nullptr, 
                                         "output/testbench.sv", 
                                         360);
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    while ((1U & (~ ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                     >> 3U)))) {
        Vtb_fp16_mac___024root____VbeforeTrig_hf0a630d2__0(vlSelf, 
                                                           "@( tb_fp16_mac.uut.valid_q[3])");
        co_await vlSelfRef.__VtrigSched_hf0a630d2__0.trigger(1U, 
                                                             nullptr, 
                                                             "@( tb_fp16_mac.uut.valid_q[3])", 
                                                             "output/testbench.sv", 
                                                             363);
    }
    if ((0x40e00000U == vlSelfRef.tb_fp16_mac__DOT__d_o)) {
        VL_WRITEF_NX("PASS: First operation result = 7.0\n",0);
    } else {
        VL_WRITEF_NX("FAIL: First operation result = %h, expected = 40E00000\n",1
                     , '#',32,vlSelfRef.tb_fp16_mac__DOT__d_o);
    }
    while ((8U & (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q))) {
        Vtb_fp16_mac___024root____VbeforeTrig_h5faf2a49__0(vlSelf, 
                                                           "@( (~ tb_fp16_mac.uut.valid_q[3]))");
        co_await vlSelfRef.__VtrigSched_h5faf2a49__0.trigger(1U, 
                                                             nullptr, 
                                                             "@( (~ tb_fp16_mac.uut.valid_q[3]))", 
                                                             "output/testbench.sv", 
                                                             374);
    }
    vlSelfRef.tb_fp16_mac__DOT__a_i = 0x4000U;
    vlSelfRef.tb_fp16_mac__DOT__b_i = 0x4200U;
    vlSelfRef.tb_fp16_mac__DOT__c_i = 0x40800000U;
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 1U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         382);
    co_await vlSelfRef.__VdlySched.delay(1ULL, nullptr, 
                                         "output/testbench.sv", 
                                         383);
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    while ((1U & (~ ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                     >> 3U)))) {
        Vtb_fp16_mac___024root____VbeforeTrig_hf0a630d2__0(vlSelf, 
                                                           "@( tb_fp16_mac.uut.valid_q[3])");
        co_await vlSelfRef.__VtrigSched_hf0a630d2__0.trigger(1U, 
                                                             nullptr, 
                                                             "@( tb_fp16_mac.uut.valid_q[3])", 
                                                             "output/testbench.sv", 
                                                             386);
    }
    if ((0x41200000U == vlSelfRef.tb_fp16_mac__DOT__d_o)) {
        VL_WRITEF_NX("PASS: Second operation result = 10.0\n",0);
    } else {
        VL_WRITEF_NX("FAIL: Second operation result = %h, expected = 41200000\n",1
                     , '#',32,vlSelfRef.tb_fp16_mac__DOT__d_o);
    }
    vlSelfRef.tb_fp16_mac__DOT__valid_i = 0U;
    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(vlSelf, 
                                                       "@(posedge tb_fp16_mac.clk)");
    co_await vlSelfRef.__VtrigSched_h56ff3649__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_fp16_mac.clk)", 
                                                         "output/testbench.sv", 
                                                         395);
    VL_WRITEF_NX("All tests completed at time %t\n",2, 'T',-12
                 , '#',64,VL_TIME_UNITED_Q(1));
    VL_FINISH_MT("output/testbench.sv", 109, "");
    co_return;
}

void Vtb_fp16_mac___024root___eval_triggers_vec__act(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_triggers_vec__act\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vtrigprevexpr_h08083ddc__0;
    __Vtrigprevexpr_h08083ddc__0 = 0;
    // Body
    __Vtrigprevexpr_h08083ddc__0 = (1U & (~ ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                                             >> 3U)));
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    ((((IData)(__Vtrigprevexpr_h08083ddc__0) 
                                                       != (IData)(vlSelfRef.__Vtrigprevexpr_h08083ddc__1)) 
                                                      << 4U) 
                                                     | (((((1U 
                                                            & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                                                               >> 3U)) 
                                                           != (IData)(vlSelfRef.__Vtrigprevexpr_h74914845__0)) 
                                                          << 3U) 
                                                         | (vlSelfRef.__VdlySched.awaitingCurrentTime() 
                                                            << 2U)) 
                                                        | ((((~ (IData)(vlSelfRef.tb_fp16_mac__DOT__rst_n)) 
                                                             & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_fp16_mac__DOT__rst_n__0)) 
                                                            << 1U) 
                                                           | ((IData)(vlSelfRef.tb_fp16_mac__DOT__clk) 
                                                              & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_fp16_mac__DOT__clk__0))))))));
    vlSelfRef.__Vtrigprevexpr___TOP__tb_fp16_mac__DOT__clk__0 
        = vlSelfRef.tb_fp16_mac__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_fp16_mac__DOT__rst_n__0 
        = vlSelfRef.tb_fp16_mac__DOT__rst_n;
    vlSelfRef.__Vtrigprevexpr_h74914845__0 = (1U & 
                                              ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                                               >> 3U));
    vlSelfRef.__Vtrigprevexpr_h08083ddc__1 = __Vtrigprevexpr_h08083ddc__0;
}

bool Vtb_fp16_mac___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___trigger_anySet__act\n"); );
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

void Vtb_fp16_mac___024root___act_comb__TOP__0(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___act_comb__TOP__0\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
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
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_sub 
        = (IData)(((0U == (0x7c00U & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i))) 
                   & (0U != (0x000003ffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i)))));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_sub 
        = (IData)(((0U == (0x7c00U & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i))) 
                   & (0U != (0x000003ffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i)))));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_zero_f 
        = ((IData)((0U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i)))) 
           | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_sub));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_zero_f 
        = ((IData)((0U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i)))) 
           | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_sub));
}

void Vtb_fp16_mac___024root___eval_act(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_act\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((0x000000000000001dULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__c_is_sub 
            = (IData)(((0U == (0x7f800000U & vlSelfRef.tb_fp16_mac__DOT__c_i)) 
                       & (0U != (0x007fffffU & vlSelfRef.tb_fp16_mac__DOT__c_i))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_inf 
            = (IData)((0x7c00U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_inf 
            = (IData)((0x7c00U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__stall 
            = ((~ (IData)(vlSelfRef.tb_fp16_mac__DOT__ready_i)) 
               & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                  >> 3U));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_sub 
            = (IData)(((0U == (0x7c00U & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i))) 
                       & (0U != (0x000003ffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i)))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_sub 
            = (IData)(((0U == (0x7c00U & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i))) 
                       & (0U != (0x000003ffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i)))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_zero_f 
            = ((IData)((0U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i)))) 
               | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_sub));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_zero_f 
            = ((IData)((0U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i)))) 
               | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_sub));
    }
}

void Vtb_fp16_mac___024root___nba_sequent__TOP__0(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___nba_sequent__TOP__0\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.tb_fp16_mac__DOT__rst_n) {
        if ((1U & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__stall)))) {
            vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q 
                = ((0x0000000eU & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                                   << 1U)) | ((IData)(vlSelfRef.tb_fp16_mac__DOT__valid_i) 
                                              & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__stall))));
            vlSelfRef.tb_fp16_mac__DOT__invalid_o = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_invalid;
            vlSelfRef.tb_fp16_mac__DOT__overflow_o 
                = ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__result_overflow) 
                   & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_invalid)));
            vlSelfRef.tb_fp16_mac__DOT__d_o = ((((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sign) 
                                                 & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_invalid))) 
                                                << 0x0000001fU) 
                                               | (((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__packed_exp) 
                                                   << 0x00000017U) 
                                                  | vlSelfRef.tb_fp16_mac__DOT__uut__DOT__packed_mant));
            vlSelfRef.tb_fp16_mac__DOT__underflow_o 
                = (((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__result_underflow) 
                    & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_invalid))) 
                   & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_is_zero)));
        }
    } else {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q = 0U;
        vlSelfRef.tb_fp16_mac__DOT__invalid_o = 0U;
        vlSelfRef.tb_fp16_mac__DOT__overflow_o = 0U;
        vlSelfRef.tb_fp16_mac__DOT__d_o = 0U;
        vlSelfRef.tb_fp16_mac__DOT__underflow_o = 0U;
    }
}

void Vtb_fp16_mac___024root___nba_sequent__TOP__1(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___nba_sequent__TOP__1\n"); );
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
    if ((1U & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__stall)))) {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_a_mant 
            = ((((0U != (0x0000001fU & ((IData)(vlSelfRef.tb_fp16_mac__DOT__a_i) 
                                        >> 0x0000000aU))) 
                 << 0x0000000aU) | (0x000003ffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i))) 
               & (- (IData)((1U & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_sub))))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_b_mant 
            = ((((0U != (0x0000001fU & ((IData)(vlSelfRef.tb_fp16_mac__DOT__b_i) 
                                        >> 0x0000000aU))) 
                 << 0x0000000aU) | (0x000003ffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i))) 
               & (- (IData)((1U & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_sub))))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_frac 
            = ((0x00200000U & vlSelfRef.tb_fp16_mac__DOT__uut__DOT__raw_product)
                ? (0x03ffffe0U & (vlSelfRef.tb_fp16_mac__DOT__uut__DOT__raw_product 
                                  << 5U)) : (0x03ffffc0U 
                                             & (vlSelfRef.tb_fp16_mac__DOT__uut__DOT__raw_product 
                                                << 6U)));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sticky_in 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__shift_sticky;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_sign 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_c_sign;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_frac 
            = (vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_c_mant 
               << 3U);
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_b_inf 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_b_inf;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_a_inf 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_a_inf;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_c_inf 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_inf;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_sign 
            = ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_a_sign) 
               ^ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_b_sign));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_zero 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_c_zero;
    }
    tb_fp16_mac__DOT__uut__DOT__c_mag = ((0x04000000U 
                                          | vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_frac) 
                                         & (- (IData)(
                                                      (1U 
                                                       & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_zero))))));
    if ((1U & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__stall)))) {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_zero 
            = ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_a_zero) 
               | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_b_zero));
    }
    tb_fp16_mac__DOT__uut__DOT__prod_mag = ((0x04000000U 
                                             | vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_frac) 
                                            & (- (IData)(
                                                         (1U 
                                                          & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_zero))))));
    if ((1U & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__stall)))) {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sum_mag 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__sum_mag;
        tb_fp16_mac__DOT__uut__DOT__lead_zero = 0x1bU;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_exp 
            = ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__prod_ge_c)
                ? (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_exp)
                : (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_exp));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sign 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__sum_sign;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_is_zero 
            = (0U == vlSelfRef.tb_fp16_mac__DOT__uut__DOT__sum_mag);
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_invalid 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_invalid;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_c_sign 
            = (vlSelfRef.tb_fp16_mac__DOT__c_i >> 0x0000001fU);
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_c_mant 
            = (0x007fffffU & (vlSelfRef.tb_fp16_mac__DOT__c_i 
                              & (- (IData)((1U & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__c_is_sub)))))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_b_inf 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_b_inf;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_a_inf 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_a_inf;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_inf 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_c_inf;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_a_sign 
            = (1U & ((IData)(vlSelfRef.tb_fp16_mac__DOT__a_i) 
                     >> 0x0000000fU));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_b_sign 
            = (1U & ((IData)(vlSelfRef.tb_fp16_mac__DOT__b_i) 
                     >> 0x0000000fU));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_c_zero 
            = ((IData)((0U == (0x7fffffffU & vlSelfRef.tb_fp16_mac__DOT__c_i))) 
               | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__c_is_sub));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_a_zero 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_zero_f;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_b_zero 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_zero_f;
    } else {
        tb_fp16_mac__DOT__uut__DOT__lead_zero = 0x1bU;
    }
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
    if ((1U & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__stall)))) {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_exp 
            = (0x000003ffU & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_c_exp) 
                              - (IData)(0x007fU)));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_exp 
            = (0x000003ffU & (((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_a_exp) 
                               - (IData)(0x000fU)) 
                              + (((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_b_exp) 
                                  - (IData)(0x000fU)) 
                                 + (1U & (vlSelfRef.tb_fp16_mac__DOT__uut__DOT__raw_product 
                                          >> 0x00000015U)))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_invalid 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_invalid;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_b_inf 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_inf;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_a_inf 
            = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_inf;
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_c_inf 
            = (IData)((0x7f800000U == (0x7fffffffU 
                                       & vlSelfRef.tb_fp16_mac__DOT__c_i)));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_c_exp 
            = (0x000000ffU & ((vlSelfRef.tb_fp16_mac__DOT__c_i 
                               >> 0x00000017U) & (- (IData)(
                                                            (1U 
                                                             & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__c_is_sub)))))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_a_exp 
            = (0x0000001fU & (((IData)(vlSelfRef.tb_fp16_mac__DOT__a_i) 
                               >> 0x0000000aU) & (- (IData)(
                                                            (1U 
                                                             & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_sub)))))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_b_exp 
            = (0x0000001fU & (((IData)(vlSelfRef.tb_fp16_mac__DOT__b_i) 
                               >> 0x0000000aU) & (- (IData)(
                                                            (1U 
                                                             & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_sub)))))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_invalid 
            = ((IData)(((0x7c00U == (0x7c00U & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i))) 
                        & (0U != (0x000003ffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i))))) 
               | ((IData)(((0x7c00U == (0x7c00U & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i))) 
                           & (0U != (0x000003ffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i))))) 
                  | ((IData)(((0x7f800000U == (0x7f800000U 
                                               & vlSelfRef.tb_fp16_mac__DOT__c_i)) 
                              & (0U != (0x007fffffU 
                                        & vlSelfRef.tb_fp16_mac__DOT__c_i)))) 
                     | (((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_zero_f) 
                         & (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_inf)) 
                        | ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_zero_f) 
                           & (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_inf))))));
    }
    tb_fp16_mac__DOT__uut__DOT__special_inf = ((~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_invalid)) 
                                               & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_a_inf) 
                                                  | ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_b_inf) 
                                                     | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_c_inf))));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__raw_product 
        = (0x003fffffU & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_b_mant) 
                          * (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s1_a_mant)));
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
    tb_fp16_mac__DOT__uut__DOT__exp_diff = (0x000003ffU 
                                            & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_exp) 
                                               - (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_exp)));
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
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__prod_ge_c 
        = ((~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_prod_zero)) 
           & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s2_c_zero) 
              | VL_LTES_III(32, 0U, VL_EXTENDS_II(32,10, (IData)(tb_fp16_mac__DOT__uut__DOT__exp_diff)))));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__result_overflow 
        = ((~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_is_zero)) 
           & VL_LTES_III(11, 0x00ffU, (IData)(tb_fp16_mac__DOT__uut__DOT__final_exp_biased)));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__result_underflow 
        = ((~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_is_zero)) 
           & VL_GTES_III(11, 0U, (IData)(tb_fp16_mac__DOT__uut__DOT__final_exp_biased)));
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
    tb_fp16_mac__DOT__uut__DOT__clamped_shift = ((0x001bU 
                                                  < (IData)(tb_fp16_mac__DOT__uut__DOT__shift_amt))
                                                  ? 0x001bU
                                                  : (IData)(tb_fp16_mac__DOT__uut__DOT__shift_amt));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__shift_sticky 
        = ((0U != (IData)(tb_fp16_mac__DOT__uut__DOT__clamped_shift)) 
           & (0U != (tb_fp16_mac__DOT__uut__DOT__small_mag 
                     & VL_SHIFTR_III(27,27,10, 0x07ffffffU, 
                                     (0x000003ffU & 
                                      ((IData)(0x001bU) 
                                       - (IData)(tb_fp16_mac__DOT__uut__DOT__clamped_shift)))))));
    tb_fp16_mac__DOT__uut__DOT__small_shifted = VL_SHIFTR_III(27,27,10, tb_fp16_mac__DOT__uut__DOT__small_mag, (IData)(tb_fp16_mac__DOT__uut__DOT__clamped_shift));
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

void Vtb_fp16_mac___024root___nba_comb__TOP__0(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___nba_comb__TOP__0\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__c_is_sub 
        = (IData)(((0U == (0x7f800000U & vlSelfRef.tb_fp16_mac__DOT__c_i)) 
                   & (0U != (0x007fffffU & vlSelfRef.tb_fp16_mac__DOT__c_i))));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_sub 
        = (IData)(((0U == (0x7c00U & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i))) 
                   & (0U != (0x000003ffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i)))));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_sub 
        = (IData)(((0U == (0x7c00U & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i))) 
                   & (0U != (0x000003ffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i)))));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_inf 
        = (IData)((0x7c00U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i))));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_inf 
        = (IData)((0x7c00U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i))));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_zero_f 
        = ((IData)((0U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i)))) 
           | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_sub));
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_zero_f 
        = ((IData)((0U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i)))) 
           | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_sub));
}

void Vtb_fp16_mac___024root___nba_comb__TOP__1(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___nba_comb__TOP__1\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_fp16_mac__DOT__uut__DOT__stall = (
                                                   (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__ready_i)) 
                                                   & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                                                      >> 3U));
}

void Vtb_fp16_mac___024root___eval_nba(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_nba\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        if (vlSelfRef.tb_fp16_mac__DOT__rst_n) {
            if ((1U & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__stall)))) {
                vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q 
                    = ((0x0000000eU & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                                       << 1U)) | ((IData)(vlSelfRef.tb_fp16_mac__DOT__valid_i) 
                                                  & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__stall))));
                vlSelfRef.tb_fp16_mac__DOT__invalid_o 
                    = vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_invalid;
                vlSelfRef.tb_fp16_mac__DOT__overflow_o 
                    = ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__result_overflow) 
                       & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_invalid)));
                vlSelfRef.tb_fp16_mac__DOT__d_o = (
                                                   (((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_sign) 
                                                     & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_invalid))) 
                                                    << 0x0000001fU) 
                                                   | (((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__packed_exp) 
                                                       << 0x00000017U) 
                                                      | vlSelfRef.tb_fp16_mac__DOT__uut__DOT__packed_mant));
                vlSelfRef.tb_fp16_mac__DOT__underflow_o 
                    = (((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__result_underflow) 
                        & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_invalid))) 
                       & (~ (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__s3_is_zero)));
            }
        } else {
            vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q = 0U;
            vlSelfRef.tb_fp16_mac__DOT__invalid_o = 0U;
            vlSelfRef.tb_fp16_mac__DOT__overflow_o = 0U;
            vlSelfRef.tb_fp16_mac__DOT__d_o = 0U;
            vlSelfRef.tb_fp16_mac__DOT__underflow_o = 0U;
        }
    }
    if ((1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtb_fp16_mac___024root___nba_sequent__TOP__1(vlSelf);
    }
    if ((0x000000000000001dULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__c_is_sub 
            = (IData)(((0U == (0x7f800000U & vlSelfRef.tb_fp16_mac__DOT__c_i)) 
                       & (0U != (0x007fffffU & vlSelfRef.tb_fp16_mac__DOT__c_i))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_sub 
            = (IData)(((0U == (0x7c00U & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i))) 
                       & (0U != (0x000003ffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i)))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_sub 
            = (IData)(((0U == (0x7c00U & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i))) 
                       & (0U != (0x000003ffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i)))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_inf 
            = (IData)((0x7c00U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_inf 
            = (IData)((0x7c00U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i))));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_zero_f 
            = ((IData)((0U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__a_i)))) 
               | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__a_is_sub));
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_zero_f 
            = ((IData)((0U == (0x7fffU & (IData)(vlSelfRef.tb_fp16_mac__DOT__b_i)))) 
               | (IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__b_is_sub));
    }
    if ((0x000000000000001fULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.tb_fp16_mac__DOT__uut__DOT__stall 
            = ((~ (IData)(vlSelfRef.tb_fp16_mac__DOT__ready_i)) 
               & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                  >> 3U));
    }
}

void Vtb_fp16_mac___024root___timing_ready(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___timing_ready\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.__VtrigSched_h56ff3649__0.ready("@(posedge tb_fp16_mac.clk)");
    }
    if ((8ULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.__VtrigSched_hf0a630d2__0.ready("@( tb_fp16_mac.uut.valid_q[3])");
    }
    if ((0x0000000000000010ULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.__VtrigSched_h5faf2a49__0.ready("@( (~ tb_fp16_mac.uut.valid_q[3]))");
    }
}

void Vtb_fp16_mac___024root___timing_resume(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___timing_resume\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VtrigSched_h56ff3649__0.moveToResumeQueue(
                                                          "@(posedge tb_fp16_mac.clk)");
    vlSelfRef.__VtrigSched_hf0a630d2__0.moveToResumeQueue(
                                                          "@( tb_fp16_mac.uut.valid_q[3])");
    vlSelfRef.__VtrigSched_h5faf2a49__0.moveToResumeQueue(
                                                          "@( (~ tb_fp16_mac.uut.valid_q[3]))");
    vlSelfRef.__VtrigSched_h56ff3649__0.resume("@(posedge tb_fp16_mac.clk)");
    vlSelfRef.__VtrigSched_hf0a630d2__0.resume("@( tb_fp16_mac.uut.valid_q[3])");
    vlSelfRef.__VtrigSched_h5faf2a49__0.resume("@( (~ tb_fp16_mac.uut.valid_q[3]))");
    if ((4ULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.__VdlySched.resume();
    }
}

void Vtb_fp16_mac___024root___trigger_orInto__act_vec_vec(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___trigger_orInto__act_vec_vec\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((0U >= n));
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_fp16_mac___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

bool Vtb_fp16_mac___024root___eval_phase__act(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_phase__act\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VactExecute;
    // Body
    Vtb_fp16_mac___024root___eval_triggers_vec__act(vlSelf);
    Vtb_fp16_mac___024root___timing_ready(vlSelf);
    Vtb_fp16_mac___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VactTriggered, vlSelfRef.__VactTriggeredAcc);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_fp16_mac___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
    Vtb_fp16_mac___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    __VactExecute = Vtb_fp16_mac___024root___trigger_anySet__act(vlSelfRef.__VactTriggered);
    if (__VactExecute) {
        vlSelfRef.__VactTriggeredAcc.fill(0ULL);
        Vtb_fp16_mac___024root___timing_resume(vlSelf);
        Vtb_fp16_mac___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vtb_fp16_mac___024root___eval_phase__inact(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_phase__inact\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VinactExecute;
    // Body
    __VinactExecute = vlSelfRef.__VdlySched.awaitingZeroDelay();
    if (__VinactExecute) {
        VL_FATAL_MT("output/testbench.sv", 7, "", "ZERODLY: Design Verilated with '--no-sched-zero-delay', but #0 delay executed at runtime");
    }
    return (__VinactExecute);
}

void Vtb_fp16_mac___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtb_fp16_mac___024root___eval_phase__nba(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_phase__nba\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vtb_fp16_mac___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vtb_fp16_mac___024root___eval_nba(vlSelf);
        Vtb_fp16_mac___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vtb_fp16_mac___024root___eval(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtb_fp16_mac___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("output/testbench.sv", 7, "", "DIDNOTCONVERGE: NBA region did not converge after '--converge-limit' of 10000 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VinactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00002710U < vlSelfRef.__VinactIterCount)))) {
                VL_FATAL_MT("output/testbench.sv", 7, "", "DIDNOTCONVERGE: Inactive region did not converge after '--converge-limit' of 10000 tries");
            }
            vlSelfRef.__VinactIterCount = ((IData)(1U) 
                                           + vlSelfRef.__VinactIterCount);
            vlSelfRef.__VactIterCount = 0U;
            do {
                if (VL_UNLIKELY(((0x00002710U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                    Vtb_fp16_mac___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                    VL_FATAL_MT("output/testbench.sv", 7, "", "DIDNOTCONVERGE: Active region did not converge after '--converge-limit' of 10000 tries");
                }
                vlSelfRef.__VactIterCount = ((IData)(1U) 
                                             + vlSelfRef.__VactIterCount);
                vlSelfRef.__VactPhaseResult = Vtb_fp16_mac___024root___eval_phase__act(vlSelf);
            } while (vlSelfRef.__VactPhaseResult);
            vlSelfRef.__VinactPhaseResult = Vtb_fp16_mac___024root___eval_phase__inact(vlSelf);
        } while (vlSelfRef.__VinactPhaseResult);
        vlSelfRef.__VnbaPhaseResult = Vtb_fp16_mac___024root___eval_phase__nba(vlSelf);
    } while (vlSelfRef.__VnbaPhaseResult);
}

void Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0(Vtb_fp16_mac___024root* vlSelf, const char* __VeventDescription) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root____VbeforeTrig_h56ff3649__0\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    VlUnpacked<QData/*63:0*/, 1> __VTmp;
    // Body
    __VTmp[0U] = (QData)((IData)(((IData)(vlSelfRef.tb_fp16_mac__DOT__clk) 
                                  & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_fp16_mac__DOT__clk__0)))));
    vlSelfRef.__Vtrigprevexpr___TOP__tb_fp16_mac__DOT__clk__0 
        = vlSelfRef.tb_fp16_mac__DOT__clk;
    if ((1ULL & __VTmp[0U])) {
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h56ff3649__0.ready(__VeventDescription);
    }
    vlSelfRef.__VactTriggeredAcc[0U] = (vlSelfRef.__VactTriggeredAcc[0U] 
                                        | __VTmp[0U]);
}

void Vtb_fp16_mac___024root____VbeforeTrig_hf0a630d2__0(Vtb_fp16_mac___024root* vlSelf, const char* __VeventDescription) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root____VbeforeTrig_hf0a630d2__0\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    VlUnpacked<QData/*63:0*/, 1> __VTmp;
    // Body
    __VTmp[0U] = (QData)((IData)((((1U & ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                                          >> 3U)) != (IData)(vlSelfRef.__Vtrigprevexpr_h74914845__0)) 
                                  << 3U)));
    vlSelfRef.__Vtrigprevexpr_h74914845__0 = (1U & 
                                              ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                                               >> 3U));
    if ((8ULL & __VTmp[0U])) {
        vlSelfRef.__VtrigSched_hf0a630d2__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_hf0a630d2__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_hf0a630d2__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_hf0a630d2__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_hf0a630d2__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_hf0a630d2__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_hf0a630d2__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_hf0a630d2__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_hf0a630d2__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_hf0a630d2__0.ready(__VeventDescription);
    }
    vlSelfRef.__VactTriggeredAcc[0U] = (vlSelfRef.__VactTriggeredAcc[0U] 
                                        | __VTmp[0U]);
}

void Vtb_fp16_mac___024root____VbeforeTrig_h5faf2a49__0(Vtb_fp16_mac___024root* vlSelf, const char* __VeventDescription) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root____VbeforeTrig_h5faf2a49__0\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    VlUnpacked<QData/*63:0*/, 1> __VTmp;
    CData/*0:0*/ __Vtrigprevexpr_h08083ddc__0;
    __Vtrigprevexpr_h08083ddc__0 = 0;
    // Body
    __Vtrigprevexpr_h08083ddc__0 = (1U & (~ ((IData)(vlSelfRef.tb_fp16_mac__DOT__uut__DOT__valid_q) 
                                             >> 3U)));
    __VTmp[0U] = (QData)((IData)((((IData)(__Vtrigprevexpr_h08083ddc__0) 
                                   != (IData)(vlSelfRef.__Vtrigprevexpr_h08083ddc__1)) 
                                  << 4U)));
    vlSelfRef.__Vtrigprevexpr_h08083ddc__1 = __Vtrigprevexpr_h08083ddc__0;
    if ((0x0000000000000010ULL & __VTmp[0U])) {
        vlSelfRef.__VtrigSched_h5faf2a49__0.ready(__VeventDescription);
    }
    vlSelfRef.__VactTriggeredAcc[0U] = (vlSelfRef.__VactTriggeredAcc[0U] 
                                        | __VTmp[0U]);
}

#ifdef VL_DEBUG
void Vtb_fp16_mac___024root___eval_debug_assertions(Vtb_fp16_mac___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_fp16_mac___024root___eval_debug_assertions\n"); );
    Vtb_fp16_mac__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}
#endif  // VL_DEBUG
