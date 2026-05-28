#include "demo_support.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

struct termios old, settings;

static void restore_term() {
    if (tcsetattr(0, TCSANOW, &old) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}

void demo_make_term_raw() {
    if (tcgetattr(0, &old) == -1) {
        perror("tcgetattr");
        exit(1);
    }

    if (atexit(restore_term) != 0) {
        fprintf(stderr, "Failed to atexit()\n");
        restore_term();
        exit(1);
    }

    cfmakeraw(&settings);
    if (tcsetattr(0, TCSANOW, &settings) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}
