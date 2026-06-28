#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED

#include <stdbool.h>

struct memory_entry; // fwd decl

void memory_init(void);

bool memory_create_label(const char *name, unsigned name_len, void *ptr,
                         bool is_code);

bool memory_entry_is_code_label(struct memory_entry *memory_entry);
bool memory_entry_is_label(struct memory_entry *memory_entry);
bool memory_entry_is_section(struct memory_entry *memory_entry);
bool memory_entry_is_allocation(struct memory_entry *memory_entry);

struct memory_entry *memory_lookup_label(const char* name, unsigned name_len);
struct memory_entry *memory_rlookup_label(void *ptr);
struct memory_entry *memory_lookup_section(void *ptr);

void memory_print_name_from_label(struct memory_entry *memory_entry);
void *memory_addr_from_entry(struct memory_entry *memory_entry);

void memory_print_entries(void);

struct memory_entry *memory_get_user_section(void);
void memory_update_section_size(struct memory_entry *section_entry, unsigned new_size);
void *memory_get_section_address(struct memory_entry *section_entry);
unsigned memory_get_section_size(struct memory_entry *section_entry);

void *memory_sys_alloc(unsigned size);
void memory_sys_free(void *ptr);

#endif
