#pragma once

#include <iostream>
#include <cstdlib>
#include <cxxabi.h>
// #if _LIBCPP_VERSION && __has_include("libunwind.h")
#include "libunwind.h"
// #endif
#include <csignal>
#include <assert.h>

namespace TestDemo
{
struct BacktraceState
{
    const ucontext_t * signal_ucontext;
    size_t address_count = 0;
    static const size_t address_count_max = 30;
    uintptr_t addresses[address_count_max] = {};

    BacktraceState(const ucontext_t* ucontext) : signal_ucontext(ucontext) {};

    bool AddAddress(uintptr_t ip) {
        // No more space in the storage. Fail.
        if (address_count >= address_count_max)
            return false;

        // Reset the Thumb bit, if it is set.
        const uintptr_t thumb_bit = 1;
        ip &= ~thumb_bit;

        // Ignore null addresses.
        if (ip == 0)
            return true;

        // Finally add the address to the storage.
        addresses[address_count++] = ip;
        return true;
    }
};


void printStackTrace(BacktraceState * state)
{
    assert(state);

    // Initialize unw_context and unw_cursor.
    unw_context_t unw_context = {};
    unw_getcontext(&unw_context);
    unw_cursor_t  unw_cursor = {};
    unw_init_local(&unw_cursor, &unw_context);

    // Get more contexts.
    const ucontext_t* signal_ucontext = state->signal_ucontext;
    assert(signal_ucontext);
    const sigcontext* signal_mcontext = &(signal_ucontext->uc_mcontext);
    assert(signal_mcontext);

    // Set registers.
    // unw_set_reg(&unw_cursor, UNW_ARM_R0, signal_mcontext->arm_r0);
    // unw_set_reg(&unw_cursor, UNW_ARM_R1, signal_mcontext->arm_r1);
    // unw_set_reg(&unw_cursor, UNW_ARM_R2, signal_mcontext->arm_r2);
    // unw_set_reg(&unw_cursor, UNW_ARM_R3, signal_mcontext->arm_r3);
    // unw_set_reg(&unw_cursor, UNW_ARM_R4, signal_mcontext->arm_r4);
    // unw_set_reg(&unw_cursor, UNW_ARM_R5, signal_mcontext->arm_r5);
    // unw_set_reg(&unw_cursor, UNW_ARM_R6, signal_mcontext->arm_r6);
    // unw_set_reg(&unw_cursor, UNW_ARM_R7, signal_mcontext->arm_r7);
    // unw_set_reg(&unw_cursor, UNW_ARM_R8, signal_mcontext->arm_r8);
    // unw_set_reg(&unw_cursor, UNW_ARM_R9, signal_mcontext->arm_r9);
    // unw_set_reg(&unw_cursor, UNW_ARM_R10, signal_mcontext->arm_r10);
    // unw_set_reg(&unw_cursor, UNW_ARM_R11, signal_mcontext->arm_fp);
    // unw_set_reg(&unw_cursor, UNW_ARM_R12, signal_mcontext->arm_ip);
    // unw_set_reg(&unw_cursor, UNW_ARM_R13, signal_mcontext->arm_sp);
    // unw_set_reg(&unw_cursor, UNW_ARM_R14, signal_mcontext->arm_lr);
    // unw_set_reg(&unw_cursor, UNW_ARM_R15, signal_mcontext->arm_pc);

    // unw_set_reg(&unw_cursor, UNW_REG_IP, signal_mcontext->arm_pc);
    // unw_set_reg(&unw_cursor, UNW_REG_SP, signal_mcontext->arm_sp);

    // unw_step() does not return the first IP.
    // state->AddAddress(signal_mcontext->arm_pc);

    // Unwind frames one by one, going up the frame stack.
    while (unw_step(&unw_cursor) > 0) {
        unw_word_t ip = 0;
        unw_get_reg(&unw_cursor, UNW_REG_IP, &ip);

        bool ok = state->AddAddress(ip);
        if (!ok)
            break;
    }
}

void singalHandler(int sigunm, void* ucontext)
{
    const ucontext_t* signal_ucontext = (const ucontext_t*)ucontext;
    assert(signal_ucontext);

    BacktraceState backtrace_state(signal_ucontext);
    printStackTrace(&backtrace_state);

    exit(sigunm);
}

void registerSignalHandler()
{
    for (int sig = 1; sig < NSIG; ++sig)
    {
        signal(sig, singalHandler);
    }
}
}