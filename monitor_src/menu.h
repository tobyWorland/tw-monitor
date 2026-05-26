#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

struct menu_option {
    char key;
    const char *name;
};

char menu(const char *prompt, unsigned option_count, const struct menu_option *options);

#endif
