#include "vt.h"

#include "io.h"
#include "string.h"

void vt_clearscreen(void) {
    putstring("\e[2J\e[H");
}

void vt_clearline(void) {
    putstring("\e[2K\r");
}

void vt_blank_last_n_chars(unsigned n) {
    for (unsigned i = 0; i < n; i++)
        putchar('\b');
    for (unsigned i = 0; i < n; i++)
        putchar(' ');
    for (unsigned i = 0; i < n; i++)
        putchar('\b');
}

void vt_upline(void) {
    putstring("\e[A");
}

// 0 represents an error in getting the position
// The x and y starts from 1 NOT 0
struct position vt_get_cursor_pos(void) {
    struct position result;

    putstring("\e[6n");

    if (!(getchar() == '\e' && getchar() == '[')) {
        return (struct position){0, 0};
    }

    const char *response = io_gettill('R', false);
    char *endptr;
    result.y = strtou(response, &endptr, 10);
    if (*endptr != ';') {
        return (struct position){0, 0};
    }
    result.x = strtou(endptr + 1, &endptr, 10);
    if (*endptr != '\0') {
        return (struct position){0, 0};
    }

    return result;
}

void vt_set_cursor_pos(struct position position) {
    io_printf("\e[%u;%uH", position.y, position.x);
}

void vt_go_end_position(void) {
    vt_set_cursor_pos((struct position){999, 999});
}

struct size vt_get_size(void) {
    struct position orginal_pos = vt_get_cursor_pos();
    struct position max_pos;

    // Go to the end position and get the position reported by the terminal
    vt_go_end_position();
    max_pos = vt_get_cursor_pos();

    // Restore old position
    vt_set_cursor_pos(orginal_pos);

    // X, Y is our size as it is from 1 and NOT 0
    return (struct size){max_pos.x, max_pos.y};
}

void vt_scroll_down(unsigned n) {
    struct position oldpos = vt_get_cursor_pos();

    // Go to end of screen and use line feeds to scroll
    vt_go_end_position();
    for (unsigned i = 0; i < n; i++) {
        putchar('\n');
    }

    // Work out where the cursor should go back to
    if (oldpos.y <= n) {
        // Old position got scrolled off, go back to first line
        oldpos.y = 1;
    } else {
        oldpos.y -= n;
    }
    vt_set_cursor_pos(oldpos);
}

void vt_set_attribute(unsigned attr) {
    io_printf("\e[%um", attr);
};
