#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

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

// Menu combined with numeric input
// on_option handles caller's options taking a pointer to the current
// number, and the char of the option pressed. Returning true if it
// should print the option name out, or false to silently continue on
// after on_option has been called.
int menu_number(const char *prompt, int init_number, unsigned option_count,
                const struct menu_option *options, bool (*on_option)(int *, char c));

bool menu_preset_continue(const char *prompt, bool erase_on_continue);
enum menu_warning_fix_result menu_preset_warning_fix(const char *prompt);
unsigned menu_preset_register(const char *prompt);
int32_t menu_preset_label(const char *prompt, void *relative_from);

#endif
