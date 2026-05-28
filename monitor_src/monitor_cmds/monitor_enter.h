#ifndef MONITOR_ENTER_H_INCLUDED
#define MONITOR_ENTER_H_INCLUDED

enum enter_type {
    ET_BYTE = 1,
    ET_HWORD = 2,
    ET_WORD = 4,
    ET_INSTRUCTION
};

void monitor_enter(void *addr, enum enter_type ent_type);
enum enter_type enter_ent_type_submenu(void);

#endif
