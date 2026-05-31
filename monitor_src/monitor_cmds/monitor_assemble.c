#include "monitor_assemble.h"

#include "../char.h"
#include "../io.h"
#include "../menu.h"
#include "../util.h"
#include "../thumb_asm.h"

#include <stddef.h>

static void assemble_and_show_result(void **paddr, const struct thumb_instruction *instruction) {
    enum thumb_assemble_result result = thumb_assemble(*paddr, instruction);
    switch (result) {
    case AR_NARROW_SUCCESS:
        *paddr += sizeof(thumb_t);
        break;
    case AR_WIDE_SUCCESS:
        *paddr += sizeof(thumb_t)*2;
        break;
    default:
        putstring("Failed to assemble ");
        putbyte(result);
        putnewline();
        break;
    }
}

void monitor_assemble(void *addr) {
    static const struct menu_option assemble_options[] = {
        {'b', "BX"},
        {'n', "NOP"},
        {'N', "NOP.W"},
        {'x', "BKPT"},
        {CTRL('p'), "Print Assembly"},
        {CTRL('q'), "Quit"},
    };
    bool quit = false;
    void *starting_addr = addr;

    while (!quit) {
        char opt = menu("ASM> ", ARR_LEN(assemble_options), assemble_options, NULL);
        switch (opt) {
        case 'x': {
            // TODO: Should have a way of getting a byte or arbitrary width number
            uint32_t immediate = gethexword(0);
            struct thumb_instruction instruction = {};
            instruction.mnemonic = TM_BKPT;
            // TODO: Should be able to use the helper
            instruction.operands[0].type = OT_IMMEDIATE;
            instruction.operands[0].imm = immediate;
            assemble_and_show_result(&addr, &instruction);
        }
            break;
        case CTRL('q'):
            quit = true;
            break;
        default:
            putstring("Error: Missing action\r\n");
            break;
        }
    }
}
