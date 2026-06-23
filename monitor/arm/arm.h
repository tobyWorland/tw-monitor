#ifndef ARM_H_INCLUDED
#define ARM_H_INCLUDED

#include <stdbool.h>

bool arm_address_has_thumb_intwrk_bit(void *address);
void *arm_address_set_thumb_intwrk_bit(void *old_address, bool set);

#endif
