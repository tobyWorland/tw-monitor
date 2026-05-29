#include "thumb_asm.h"

#include "assert.h"
#include "io.h"

bool thumb_is_wide_instruction(uint16_t ins) {
    // ARMv7-M Architecture Reference Manual
    // A5.3 32bit Thumb instruction Encoding
    return ((ins >> 13) == 7) && ((ins >> 11) != 0);
}

void thumb_print_register(unsigned reg) {
    assert(reg <= 15);
    if (reg < 13) {
        putchar('R');
        putbyte(reg); // TODO: Should print in base 10
    } else {
        const char *name;
        switch (reg) {
        case 13:
            name = "SP";
            break;
        case 14:
            name = "LR";
            break;
        case 15:
            name = "PC";
            break;
        default:
            ASSERT_NOT_REACHED();
        }
        putstring(name);
    }
}

// NOTE: Temporary
void thumb_print_disassembled_instruction(const uint16_t *insptr) {
    putchar(' ');
    if (thumb_is_wide_instruction(*insptr)) {
        uint16_t ins1 = insptr[0];
        uint16_t ins2 = insptr[1];

        if (ins1 == 0xF3AF && ins2 == 0x8000) {
            putstring("NOP.W");
            return;
        }
    } else {
        uint16_t ins = *insptr;

        if ((ins >> 8) == 0xBE) {
            putstring("BKPT ");
            putbyte(ins & 0xFF); // BKPT's immediate
            return;
        } else if (ins == 0xBF00) {
            putstring("NOP");
            return;
        } else if ((ins & ~(0xf << 3)) == 0x4700) {
            putstring("BX ");
            thumb_print_register((ins >> 3) & 0xf); // Rm
            return;
        }
    }
    putstring("UNKNOWN");
}
