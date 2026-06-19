#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "util.h"

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

// Multiple results can be returned by ORing them together
enum menu_number_callback_result { // After on_option callback exits...
    MNCR_QUIT_MENU    = BIT(0), // Quit menu (always happens last)
    MNCR_CLR_REPROMPT = BIT(1), // Clear line and reprint the prompt with number
    MNCR_PRINT_OPT    = BIT(2), // Print the option
};

// Menu combined with numeric input
// on_option handles caller's options taking a pointer to the current
// number, and the char of the option pressed.
int menu_number(const char *prompt, int init_number, unsigned option_count,
                const struct menu_option *options,
                enum menu_number_callback_result (*on_option)(int *, char c));

bool menu_preset_continue(const char *prompt, bool erase_on_continue);
enum menu_warning_fix_result menu_preset_warning_fix(const char *prompt);
unsigned menu_preset_register(const char *prompt);
intptr_t menu_preset_relative_label(const char *prompt, void *relative_from, bool is_code);
uint16_t menu_preset_register_list(const char *prompt);
enum thumb_width_specifier menu_preset_instruction_width_menu(enum thumb_width_specifier current);
enum thumb_condition menu_preset_instruction_set_condition_menu(void);

struct absolute_address_result {
    void *address;
    struct memory_entry *section;
};
struct absolute_address_result
menu_preset_absolute_address(const char *prompt, void *default_address,
                             bool label_drop_interwork_bit,
                             bool accept_sections);

void menu_print_missing_action_message(void);

#endif
