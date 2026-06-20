#include "scb.h"

#include "../io.h"
#include "../util.h"

#include <stdint.h>

volatile uint32_t *scb_aircr = (void*)0xE000ED0C; // Application Interrupt and Reset Control Register

#define AIRCR_VECTKEY     (0x05FA << 16)
#define AIRCR_SYSRESETREQ BIT(2)

// System control block fault registers
volatile uint32_t *scb_cfsr = (void*)0xE000ED28; // Configurable Fault Status Register
volatile uint32_t *scb_hfsr = (void*)0xE000ED2C; // Hard Fault Status Register
volatile uint32_t *scb_mmar = (void*)0xE000ED34; // Memory Management fault Address Register
volatile uint32_t *scb_bfar = (void*)0xE000ED38; // Bus Fault Address Register

#define HFSR_FORCED      BIT(30)
#define HFSR_VECTTBL     BIT(1)

#define UFSR_DIVZERO     BIT(25)
#define UFSR_UNALIGNED   BIT(24)
#define UFSR_NOCP        BIT(19)
#define UFSR_INVPC       BIT(18)
#define UFSR_INVSTATE    BIT(17)
#define UFSR_UNDEFINSTR  BIT(16)

#define BFSR_BFARVALID   BIT(15)
#define BFSR_LSPERR      BIT(13)
#define BFSR_STKERR      BIT(12)
#define BFSR_UNSTKERR    BIT(11)
#define BFSR_IMPRECISERR BIT(10)
#define BFSR_PRECISERR   BIT(9)
#define BFSR_IBUS        BIT(8)

#define MMFSR_MMARVALID  BIT(7)
#define MMFSR_MLSPERR    BIT(5)
#define MMFSR_MSTKERR    BIT(4)
#define MMFSR_MUNSTKERR  BIT(3)
#define MMFSR_DACCVIOL   BIT(1)
#define MMFSR_IACCVIOL   BIT(0)

void print_fault_state(void) {
    uint32_t hfsr = *scb_hfsr;
    uint32_t cfsr = *scb_cfsr;
    uint32_t mmar = *scb_mmar;
    uint32_t bfar = *scb_bfar;

    putstring("Hard:  ");
    if (hfsr == 0) {
        putstring(" <none>");
    } else {
        if (hfsr & HFSR_FORCED)  putstring(" FORCED");
        if (hfsr & HFSR_VECTTBL) putstring(" VECTTBL");
    }
    putnewline();

    putstring("Usage: ");
    if ((cfsr >> 16) == 0) {
        putstring(" <none>");
    } else {
        if (cfsr & UFSR_DIVZERO)    putstring(" DIVZERO");
        if (cfsr & UFSR_UNALIGNED)  putstring(" UNALIGNED");
        if (cfsr & UFSR_NOCP)       putstring(" NOCP");
        if (cfsr & UFSR_INVPC)      putstring(" INVPC");
        if (cfsr & UFSR_INVSTATE)   putstring(" INVSTATE");
        if (cfsr & UFSR_UNDEFINSTR) putstring(" UNDEFINSTR");
    }
    putnewline();

    putstring("Bus:   ");
    if ((cfsr & 0xFF00) == 0) {
        putstring(" <none>");
    } else {
        if (cfsr & BFSR_BFARVALID)   putstring(" BFARVALID");
        if (cfsr & BFSR_LSPERR)      putstring(" LSPERR");
        if (cfsr & BFSR_STKERR)      putstring(" STKERR");
        if (cfsr & BFSR_UNSTKERR)    putstring(" UNSTKERR");
        if (cfsr & BFSR_IMPRECISERR) putstring(" IMPRECISERR");
        if (cfsr & BFSR_PRECISERR)   putstring(" PRECISERR");
        if (cfsr & BFSR_IBUS)        putstring(" IBUS");
    }
    putnewline();

    putstring("Memory:");
    if ((cfsr & 0x00FF) == 0) {
        putstring(" <none>");
    } else {
        if (cfsr & MMFSR_MMARVALID) putstring(" MMARVALID");
        if (cfsr & MMFSR_MLSPERR)   putstring(" MLSPERR");
        if (cfsr & MMFSR_MSTKERR)   putstring(" MSTKERR");
        if (cfsr & MMFSR_MUNSTKERR) putstring(" MUNSTKERR");
        if (cfsr & MMFSR_DACCVIOL)  putstring(" DACCVIOL");
        if (cfsr & MMFSR_IACCVIOL)  putstring(" IACCVIOL");
    }
    putnewline();

    if (cfsr & MMFSR_MMARVALID) {
        putstring("MMAR:   ");
        puthexword(mmar);
        putnewline();
    }

    if (cfsr & BFSR_BFARVALID) {
        putstring("BFAR:   ");
        puthexword(bfar);
        putnewline();
    }
}

void clear_fault_state(void) {
    // Fault registers are sticky and the only way to clear fault bits
    // is to write 1s to those bits
    // So writing the registers to themselves should clear all set faults
    *scb_hfsr = *scb_hfsr;
    *scb_cfsr = *scb_cfsr;
}

void scb_sys_reset_request(void) {
    *scb_aircr = AIRCR_VECTKEY | AIRCR_SYSRESETREQ;

    // Recommended by ARMv7m Architecture reference manual
    // B1.5.16 Reset Management
    __asm("dsb\n"
          "b .\n");
    __builtin_unreachable();
}
