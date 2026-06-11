#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED

#include "util.h"

#include <stdbool.h>

void terminal_init(void);

void terminal_clearscreen();
void terminal_clearline();
struct size terminal_get_size(void);
struct position terminal_get_position(void);
void terminal_set_position(struct position position);
void terminal_set_position_xy(unsigned x, unsigned y);

void terminal_scroll_down(unsigned n);
void terminal_erase_last_chars(unsigned n);

void terminal_set_video_reverse(bool reverse);

#endif
