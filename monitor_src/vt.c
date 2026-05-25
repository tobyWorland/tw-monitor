#include "vt.h"

#include "hardware.h"

void vt_clearline(void) {
    putstring("\e[2K");
}
