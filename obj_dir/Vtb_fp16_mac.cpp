// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_fp16_mac__pch.h"

//============================================================
// Constructors

Vtb_fp16_mac::Vtb_fp16_mac(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_fp16_mac__Syms(contextp(), _vcname__, this)}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtb_fp16_mac::Vtb_fp16_mac(const char* _vcname__)
    : Vtb_fp16_mac(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_fp16_mac::~Vtb_fp16_mac() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_fp16_mac___024root___eval_debug_assertions(Vtb_fp16_mac___024root* vlSelf);
#endif  // VL_DEBUG
void Vtb_fp16_mac___024root___eval_static(Vtb_fp16_mac___024root* vlSelf);
void Vtb_fp16_mac___024root___eval_initial(Vtb_fp16_mac___024root* vlSelf);
void Vtb_fp16_mac___024root___eval_settle(Vtb_fp16_mac___024root* vlSelf);
void Vtb_fp16_mac___024root___eval(Vtb_fp16_mac___024root* vlSelf);

void Vtb_fp16_mac::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_fp16_mac::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_fp16_mac___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtb_fp16_mac___024root___eval_static(&(vlSymsp->TOP));
        Vtb_fp16_mac___024root___eval_initial(&(vlSymsp->TOP));
        Vtb_fp16_mac___024root___eval_settle(&(vlSymsp->TOP));
        vlSymsp->__Vm_didInit = true;
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtb_fp16_mac___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtb_fp16_mac::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty() && !contextp()->gotFinish(); }

uint64_t Vtb_fp16_mac::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vtb_fp16_mac::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtb_fp16_mac___024root___eval_final(Vtb_fp16_mac___024root* vlSelf);

VL_ATTR_COLD void Vtb_fp16_mac::final() {
    contextp()->executingFinal(true);
    Vtb_fp16_mac___024root___eval_final(&(vlSymsp->TOP));
    contextp()->executingFinal(false);
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_fp16_mac::hierName() const { return vlSymsp->name(); }
const char* Vtb_fp16_mac::modelName() const { return "Vtb_fp16_mac"; }
unsigned Vtb_fp16_mac::threads() const { return 1; }
void Vtb_fp16_mac::prepareClone() const { contextp()->prepareClone(); }
void Vtb_fp16_mac::atClone() const {
    contextp()->threadPoolpOnClone();
}
