// Verilated -*- SystemC -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VSimpleDMVectorCache_H_
#define _VSimpleDMVectorCache_H_

#include "systemc.h"
#include "verilated_sc.h"
#include "verilated.h"
class VSimpleDMVectorCache__Syms;

//----------

SC_MODULE(VSimpleDMVectorCache) {
  public:
    // CELLS
    // Public to allow access to /*verilator_public*/ items;
    // otherwise the application code can consider these internals.
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    sc_in<bool>	clk;
    sc_in<bool>	reset;
    sc_out<bool>	io_readReq_ready;
    sc_in<bool>	io_readReq_valid;
    sc_in<bool>	io_readResp_ready;
    sc_out<bool>	io_readResp_valid;
    sc_in<bool>	io_memReq_ready;
    sc_out<bool>	io_memReq_valid;
    sc_out<bool>	io_memResp_ready;
    sc_in<bool>	io_memResp_valid;
    sc_out<bool>	io_cacheActive;
    sc_in<uint32_t>	io_readReq_bits;
    sc_out<uint32_t>	io_memReq_bits;
    sc_out<uint32_t>	io_readCount;
    sc_out<uint32_t>	io_missCount;
    sc_out<vluint64_t>	io_readResp_bits;
    sc_in<vluint64_t>	io_memResp_bits;
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    VL_SIG8(v__DOT__T2,0,0);
    VL_SIG8(v__DOT__T9,0,0);
    VL_SIG8(v__DOT__state,1,0);
    VL_SIG8(v__DOT__T25,0,0);
    VL_SIG8(v__DOT__T28,0,0);
    VL_SIG8(v__DOT__T33,0,0);
    VL_SIG8(v__DOT__enableWriteOutputReg,0,0);
    //char	__VpadToAlign79[1];
    VL_SIG16(v__DOT__currentReqEntry,11,0);
    VL_SIG16(v__DOT__initCtr,12,0);
    VL_SIG16(v__DOT__bramReadAddrReg,12,0);
    //char	__VpadToAlign86[2];
    VL_SIG(v__DOT__missCount,31,0);
    VL_SIG(v__DOT__readCount,31,0);
    VL_SIG64(v__DOT__T36,63,0);
    VL_SIG16(v__DOT__tagStorage[8192],11,0);
    VL_SIG64(v__DOT__cacheLines[8192],63,0);
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    VL_SIG8(__Vcellinp__v__clk,0,0);
    VL_SIG8(__Vcellinp__v__io_memResp_valid,0,0);
    VL_SIG8(__Vcellinp__v__io_memReq_ready,0,0);
    VL_SIG8(__Vcellinp__v__io_readResp_ready,0,0);
    VL_SIG8(__Vcellinp__v__io_readReq_valid,0,0);
    VL_SIG8(__Vcellinp__v__reset,0,0);
    VL_SIG8(__Vclklast__TOP____Vcellinp__v__clk,0,0);
    //char	__VpadToAlign82035[1];
    VL_SIG(__Vcellinp__v__io_readReq_bits,23,0);
    VL_SIG64(__Vcellinp__v__io_memResp_bits,63,0);
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    //char	__VpadToAlign82052[4];
    VSimpleDMVectorCache__Syms*	__VlSymsp;		// Symbol table
    
    // PARAMETERS
    // Parameters marked /*verilator public*/ for use by application code
    
    // CONSTRUCTORS
  private:
    VSimpleDMVectorCache& operator= (const VSimpleDMVectorCache&);	///< Copying not allowed
    VSimpleDMVectorCache(const VSimpleDMVectorCache&);	///< Copying not allowed
  public:
    SC_CTOR(VSimpleDMVectorCache);
    virtual ~VSimpleDMVectorCache();
    
    // USER METHODS
    
    // API METHODS
  private:
    void eval();
  public:
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(VSimpleDMVectorCache__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(VSimpleDMVectorCache__Syms* symsp, bool first);
  private:
    static IData	_change_request(VSimpleDMVectorCache__Syms* __restrict vlSymsp);
  public:
    static void	_combo__TOP__1(VSimpleDMVectorCache__Syms* __restrict vlSymsp);
    static void	_combo__TOP__4(VSimpleDMVectorCache__Syms* __restrict vlSymsp);
    static void	_combo__TOP__6(VSimpleDMVectorCache__Syms* __restrict vlSymsp);
    static void	_combo__TOP__9(VSimpleDMVectorCache__Syms* __restrict vlSymsp);
    static void	_eval(VSimpleDMVectorCache__Syms* __restrict vlSymsp);
    static void	_eval_initial(VSimpleDMVectorCache__Syms* __restrict vlSymsp);
    static void	_eval_settle(VSimpleDMVectorCache__Syms* __restrict vlSymsp);
    static void	_sequent__TOP__3(VSimpleDMVectorCache__Syms* __restrict vlSymsp);
    static void	_sequent__TOP__7(VSimpleDMVectorCache__Syms* __restrict vlSymsp);
    static void	_settle__TOP__10(VSimpleDMVectorCache__Syms* __restrict vlSymsp);
    static void	_settle__TOP__5(VSimpleDMVectorCache__Syms* __restrict vlSymsp);
    static void	_settle__TOP__8(VSimpleDMVectorCache__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(128);

#endif  /*guard*/
