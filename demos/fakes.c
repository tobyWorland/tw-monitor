#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

void putstring(const char *string) {
    printf("%s", string);
}

void putnewline() {
    printf("\r\n");
}

char fake_getchar() {
    char c = getchar();
    if (c == 127) { // Host will send 127 for backspace where as target will get
                    // 8 (\b)
        c = '\b';
    }
    return c;
}

void fake_assert(bool flag) {
    assert(flag);
}
