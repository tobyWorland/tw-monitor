#include <assert.h>
#include <stdio.h>

void putstring(const char *string) {
	printf("%s", string);
}
void putnewline() {
	printf("\r\n");
}

void fake_assert(bool flag) {
	assert(flag);
}
