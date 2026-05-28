#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdbool.h>

struct menu_option {
    char key;
    const char *name;
};

char menu(const char *prompt, unsigned option_count,
          const struct menu_option *options);

bool menu_preset_continue(const char *prompt);

#endif
