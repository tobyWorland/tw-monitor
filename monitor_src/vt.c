#include "vt.h"

#include "io.h"

void vt_clearline(void) {
    putstring("\e[2K");
}
