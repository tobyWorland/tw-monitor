#include "monitor_enter.h"

#include "../assert.h"
#include "../char.h"
#include "../io.h"
#include "../menu.h"
#include "../util.h"
#include "../vt.h"
#include "../thumb_asm.h"

#include <stdint.h>

#ifndef HOST

struct enter_state {
    void *addr;
    uint32_t current;
    unsigned digit_idx;
    unsigned col;

    const enum enter_type ent_type;
    const unsigned digit_width;
    const bool extra_space_in_number;
    const bool ent_type_is_instruction;
};

static void load_current(struct enter_state *state) {
    switch (state->ent_type) {
    case ET_BYTE:
        *(uint8_t*)&state->current = *(uint8_t*)state->addr;
        break;
    case ET_HWORD:
        *(uint16_t*)&state->current = *(uint16_t*)state->addr;
        break;
    case ET_WORD:
        *(uint32_t*)&state->current = *(uint32_t*)state->addr;
        break;
    default:
        ASSERT_NOT_REACHED();
    }
};

static void store_current(struct enter_state *state) {
    switch (state->ent_type) {
    case ET_BYTE:
        *(uint8_t*)state->addr = *(uint8_t*)&state->current;
        break;
    case ET_HWORD:
        *(uint16_t*)state->addr = *(uint16_t*)&state->current;
        break;
    case ET_WORD:
        *(uint32_t*)state->addr = *(uint32_t*)&state->current;
        break;
    default:
        ASSERT_NOT_REACHED();
    }
};

static inline unsigned enter_get_digits_from_type(enum enter_type ent_type) {
    unsigned width_in_bytes = (unsigned)ent_type;
    return width_in_bytes * 2;
}

static inline bool handle_input(struct enter_state *state) {
    char c = getchar();
    uint8_t digit;

    if (c == '\r') {
        return false; // Quit
    } else if (c == '\b') {
        if (state->digit_idx == 0) {
            // Go back a byte
            if (state->col > 0) {
                state->col--;
                state->addr -= (unsigned)state->ent_type;

                vt_blank_last_n_chars(state->digit_width + 1 + !!state->extra_space_in_number);

                load_current(state);
                state->digit_idx = 0;
            } else {
                // TODO: handle going back a row
            }
        } else {
            // Reload byte from memory
            load_current(state);
            state->digit_idx = 0;
        }
    } else if (c == ' ') {
        // Leave entry as is and enter the next one by setting the idx as if we
        // had entered enough digits to enter the next entry
        state->digit_idx = state->digit_width;
    } else if ((digit = char_to_digit(c)) != 0xFF) {
        // Add new digit to current
        state->current <<= 4;
        state->current |= digit;
        state->digit_idx++;
    }

    return true;
}

static bool next_column(struct enter_state *state) {
    if (state->ent_type_is_instruction) {
        return (state->col++ > 0) || !thumb_is_wide_instruction(state->current);
    } else {
        // If hit max column, we want to move to the next row
        // 16 bytes
        // 8  half words
        // 4  words
        return ++state->col == 32/state->digit_width;
    }
}

static void _monitor_enter(struct enter_state *state) {
    load_current(state);

    puthexword((uint32_t)state->addr);

    while (1) {
        // Print current entry
        putchar(' ');
        puthexnumber(state->digit_width, state->current);

        if (state->digit_idx >= state->digit_width) {
            // Store current to memory
            store_current(state);

            // Needs to happen before next entry for the ET_INSTRUCTION wide check
            bool column_overflow = next_column(state);

            // Move to next entry
            state->addr += (unsigned)state->ent_type;
            load_current(state);
            state->digit_idx = 0;

            if (column_overflow) {
                // Column overflowed, now on next row so print address
                putnewline();
                puthexword((uint32_t)state->addr);
                state->col = 0;
            }

            // Print next entry
            putchar(' ');
            puthexnumber(state->digit_width, state->current);
        }

        if (!handle_input(state)) {
            break;
        }

        // Erase the current entry from the screen so we can reprint it with the new digit
        // +1 for the space
        vt_blank_last_n_chars(state->digit_width + 1 + !!state->extra_space_in_number);
    }

    // Store our changes to the current entry before we exit
    store_current(state);
    putnewline();
}

enum enter_type enter_ent_type_submenu(void) {
    static const struct menu_option ent_options[] = {
        {'b', "Bytes"       },
        {'h', "Half Words"  },
        {'w', "Words"       },
        {'i', "Instructions"},
    };
    char opt = submenu("Type? ", ARR_LEN(ent_options), ent_options);
    switch (opt) {
    case 'b':
        return ET_BYTE;
    case 'h':
        return ET_HWORD;
    case 'w':
        return ET_WORD;
    case 'i':
        return ET_INSTRUCTION;
    default:
        ASSERT_NOT_REACHED();
    }
}

void monitor_enter(void *addr, enum enter_type ent_type) {
    const bool is_instruction_ent = ent_type == ET_INSTRUCTION;

    if (is_instruction_ent) {
        ent_type = ET_HWORD;
    }

    const unsigned digit_width = enter_get_digits_from_type(ent_type);
    struct enter_state state = (struct enter_state){
        .addr = addr,
        .ent_type = ent_type,
        .digit_width = digit_width,
        .extra_space_in_number = digit_width > 4,
        .ent_type_is_instruction = is_instruction_ent,
    };

    _monitor_enter(&state);
}

#endif
