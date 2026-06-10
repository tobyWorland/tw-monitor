#include "memory.h"

#include "arm.h"
#include "assert.h"
#include "io.h"
#include "string.h"

#include <stddef.h>

// NOTE: Update linker script if changing struct
struct memory_metadata {
    struct memory_entry *first_memory_entry;
    struct memory_entry *last_memory_entry;
};

// Both defined in linker script
extern struct memory_metadata g_memory_meta;
extern unsigned user_start;

// TODO: Make define?
// TODO: Type could be tagged on to the most significant bits of the pointer?
enum entry_type {
    ET_SECTION    = 0,
    ET_LABEL_CODE = 1,
    ET_LABEL_DATA = 2,
};

struct memory_entry {
    enum entry_type type;
    void *addr;
    union {
        struct {
            unsigned size;
        } section;
        struct {
            unsigned name_len;
            // NOTE: name is **NOT** null terminated
            char name[];
        } label;
    };
};

static unsigned memory_sizeof_entry(struct memory_entry *memory_entry) {
    unsigned size = sizeof(struct memory_entry);
    switch (memory_entry->type) {
    case ET_SECTION:
        break;
    case ET_LABEL_CODE:
    case ET_LABEL_DATA:
        size += memory_entry->label.name_len;
        break;
    default:
        assert(false);
    }
    return size;
}

// Check from the last memory entry to the first
static struct memory_entry *memory_get_next_entry(struct memory_entry *memory_entry) {
    if (memory_entry == g_memory_meta.first_memory_entry) {
        // Was the first entry so no more entries exist after this
        return NULL;
    } else {
        return (void *)memory_entry + memory_sizeof_entry(memory_entry);
    }
}

void memory_init(void) {
    struct memory_entry *first = (void*)&g_memory_meta - sizeof(struct memory_entry);

    first->type = ET_SECTION;
    first->addr = &user_start;
    first->section.size = 0;

    g_memory_meta.first_memory_entry = first;
    g_memory_meta.last_memory_entry = first;
}

bool memory_create_label(const char *name, unsigned name_len, void *ptr,
                         bool is_code) {
    if (memory_lookup_label(name, name_len)) {
        return false;
    }

    struct memory_entry *new_label = g_memory_meta.last_memory_entry;
    new_label = (void*)new_label - sizeof(struct memory_entry) - name_len;

    new_label->type = is_code ? ET_LABEL_CODE : ET_LABEL_DATA;
    new_label->addr = ptr;
    new_label->label.name_len = name_len;
    memcpy(new_label->label.name, name, name_len);

    g_memory_meta.last_memory_entry = new_label;

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
    struct memory_entry *current = g_memory_meta.last_memory_entry;

    for (; current; current = memory_get_next_entry(current)) {
        if (memory_entry_is_label(current) &&
            current->label.name_len == name_len &&
            !memcmp(current->label.name, name, name_len)) {
            return current;
        }
    }

    return current; // NULL
}

struct memory_entry *memory_rlookup_label(void *ptr) {
    struct memory_entry *current = g_memory_meta.last_memory_entry;

    for (; current; current = memory_get_next_entry(current)) {
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
    struct memory_entry *current = g_memory_meta.last_memory_entry;

    for (; current; current = memory_get_next_entry(current)) {
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

void memory_print_entries(void) {
    struct memory_entry *current = g_memory_meta.last_memory_entry;

    for (; current; current = memory_get_next_entry(current)) {
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
