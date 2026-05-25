#include <assert.h>
#include <stdio.h>

void putstring(const char *string) {
	printf("%s", string);
}
void putnewline() {
	puts("");
}

void fake_assert(bool flag) {
	assert(flag);
}
