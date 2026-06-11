#include "terminal.h"

#include "vt.h"

// TOOD: Implement
void terminal_init(void) {

}

void terminal_clearscreen() {
    vt_clearscreen();
}

void terminal_clearline() {
    vt_clearline();
}

struct size terminal_get_size(void) {
    return vt_get_size();
}

struct position terminal_get_position(void) {
    return vt_get_cursor_pos();
}

void terminal_set_position(struct position position) {
    vt_set_cursor_pos(position);
}

void terminal_set_position_xy(unsigned x, unsigned y) {
    terminal_set_position((struct position){x, y});
}

void terminal_scroll_down(unsigned n) {
    vt_scroll_down(n);
}

void terminal_erase_last_chars(unsigned n) {
    vt_blank_last_n_chars(n);
}

void terminal_set_video_reverse(bool reverse) {
    vt_set_attribute(reverse ? VT_ATTR_REV : VT_ATTR_NONE);
}
