#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdbool.h>

struct menu_option {
    char key;
    const char *name;
};

enum menu_warning_fix_result {
    MENU_WF_PROCEED,
    MENU_WF_FIX,
    MENU_WF_QUIT,
};

char menu(const char *prompt, unsigned option_count,
          const struct menu_option *options, const char *submenu_keys);
char submenu(const char *prompt, unsigned option_count,
             const struct menu_option *options);

bool menu_preset_continue(const char *prompt, bool erase_on_continue);
enum menu_warning_fix_result menu_preset_warning_fix(const char *prompt);

#endif
