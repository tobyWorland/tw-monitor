#ifndef VT_H_INCLUDED
#define VT_H_INCLUDED

#include "util.h"

#define VT_ATTR_NONE 0
#define VT_ATTR_REV  7

void vt_clearscreen(void);
void vt_clearline(void);
void vt_blank_last_n_chars(unsigned n);
void vt_upline(void);
struct position vt_get_cursor_pos(void);
void vt_set_cursor_pos(struct position position);
void vt_go_end_position(void);
struct size vt_get_size(void);
void vt_scroll_down(unsigned n);
void vt_set_attribute(unsigned attr);

#endif
