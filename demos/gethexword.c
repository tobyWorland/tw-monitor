#include <stdio.h>
#include <termios.h>

void gethexword(); // TODO:

int main() {
    struct termios old, settings;
    if (tcgetattr(0, &old) == -1) {
        perror("tcgetattr");
        return 1;
    }

    cfmakeraw(&settings);
    if (tcsetattr(0, TCSANOW, &settings) == -1) {
        perror("tcsetattr");
        return 1;
    }

    gethexword();

    if (tcsetattr(0, TCSANOW, &old) == -1) {
        perror("tcsetattr");
        return 1;
    }

    return 0;
}
