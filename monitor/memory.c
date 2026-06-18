#include "memory.h"

#include "arm.h"
#include "assert.h"
#include "io.h"
#include "string.h"

#include <stddef.h>

#define MAX_NAME_LEN 8

#ifndef HOST

// Both defined in linker script
extern uint64_t stack_ceiling;
extern unsigned user_start;

// Memory entries grow from the stack ceiling down to the lower
// addresses of memory. So the next entry will be before the current.

// TODO: Make define?
// TODO: Type could be tagged on to the most significant bits of the pointer?
enum entry_type {
    ET_SECTION,
    ET_LABEL_CODE,
    ET_LABEL_DATA,
};

struct memory_entry {
    enum entry_type type;
    void *addr;
    union {
        struct {
            unsigned size;
        } section;
        struct {
            uint8_t name_len;
            // NOTE: name is **NOT** null terminated
            char name[MAX_NAME_LEN];
        } label;
    };
};

static struct memory_entry *s_next_free_memory_entry = NULL;

static struct memory_entry *get_first_memory_entry(void) {
    void *ptr = &stack_ceiling;
    return ptr - sizeof(struct memory_entry);
}

static struct memory_entry *get_next_memory_entry(struct memory_entry *entry) {
    entry--;

    if (entry == s_next_free_memory_entry) {
        return NULL;
    } else {
        return entry;
    }
}

static struct memory_entry *create_memory_entry() {
    struct memory_entry *result = s_next_free_memory_entry;
    s_next_free_memory_entry = result - 1;
    return result;
}

void memory_init(void) {
    s_next_free_memory_entry = get_first_memory_entry();

    struct memory_entry *entry = create_memory_entry();
    *entry = (struct memory_entry) {
        .type = ET_SECTION,
        .addr = &user_start,
        .section = {
            .size = 0
        }
    };
}

bool memory_create_label(const char *name, unsigned name_len, void *ptr,
                         bool is_code) {
    // Check if name_len is valid
    if ((name_len == 0) || (name_len > MAX_NAME_LEN)) {
        return false;
    }

    // Check if label already exists
    if (memory_lookup_label(name, name_len)) {
        return false;
    }

    // Create new_label
    struct memory_entry *new_label = create_memory_entry();
    *new_label = (struct memory_entry) {
        .type = is_code ? ET_LABEL_CODE : ET_LABEL_DATA,
        .addr = ptr,
    };

    // Write name
    new_label->label.name_len = name_len;
    memcpy(new_label->label.name, name, name_len);

    // Set interwork bit if it's a code label for absolute addressing
    if (is_code) {
        new_label->addr = arm_address_set_thumb_intwrk_bit(new_label->addr, true);
    }

    return true;
}

bool memory_entry_is_code_label(struct memory_entry *memory_entry) {
    return memory_entry->type == ET_LABEL_CODE;
}

bool memory_entry_is_label(struct memory_entry *memory_entry) {
    switch (memory_entry->type) {
    case ET_LABEL_CODE:
    case ET_LABEL_DATA:
        return true;
    default:
        return false;
    }
}

bool memory_entry_is_section(struct memory_entry *memory_entry) {
    return memory_entry->type == ET_SECTION;
}

struct memory_entry *memory_lookup_label(const char* name, unsigned name_len) {
    struct memory_entry *current = get_first_memory_entry();

    for (; current; current = get_next_memory_entry(current)) {
        if (memory_entry_is_label(current) &&
            current->label.name_len == name_len &&
            !memcmp(current->label.name, name, name_len)) {
            return current;
        }
    }

    return current; // NULL
}

struct memory_entry *memory_rlookup_label(void *ptr) {
    struct memory_entry *current = get_first_memory_entry();

    for (; current; current = get_next_memory_entry(current)) {
        if (!memory_entry_is_label(current)) {
            continue;
        }

        if (memory_entry_is_code_label(current)) {
            // If code label, allow ptr to match having the interwork bit set or not
            void *intwrk_bit_ptr = arm_address_set_thumb_intwrk_bit(current->addr, true);
            void *wo_intwrk_bit_ptr = arm_address_set_thumb_intwrk_bit(current->addr, false);
            if ((intwrk_bit_ptr == ptr) || (wo_intwrk_bit_ptr == ptr)) {
                return current;
            }
        } else {
            if (current->addr == ptr) {
                return current;
            }
        }
    }

    return current; // NULL
}

struct memory_entry *memory_lookup_section(void *ptr) {
    struct memory_entry *current = get_first_memory_entry();

    for (; current; current = get_next_memory_entry(current)) {
        if (!memory_entry_is_section(current)) {
            continue;
        }

        void *sec_start = current->addr;
        void *sec_end = sec_start + current->section.size;

        if ((sec_start <= ptr) && (ptr <= sec_end)) { // sec_start <= ptr <= sec_end
            return current;
        }
    }

    return current; // NULL
}

void memory_print_name_from_label(struct memory_entry *memory_entry) {
    assert(memory_entry_is_label(memory_entry));
    putnstring(memory_entry->label.name, memory_entry->label.name_len);
}

void *memory_addr_from_entry(struct memory_entry *memory_entry) {
    return memory_entry->addr;
}

void memory_print_entries(void) {
    struct memory_entry *current = get_first_memory_entry();

    for (; current; current = get_next_memory_entry(current)) {
        if (memory_entry_is_section(current)) {
            io_printf("SECTION @ %x SIZE %u\r\n", current->addr, current->section.size);
        } else if (memory_entry_is_label(current)) {
            if (memory_entry_is_code_label(current)) {
                // TODO: Need to implement %.*s
                io_printf("CLABEL  @ %x NAME ", current->addr);
                putnstring(current->label.name, current->label.name_len);
                putnewline();
            } else {
                // TODO: Need to implement %.*s
                io_printf("DLABEL  @ %x NAME ", current->addr);
                putnstring(current->label.name, current->label.name_len);
                putnewline();
            }
        }
    }
}

#endif
