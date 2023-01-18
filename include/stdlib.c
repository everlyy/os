#include <stdlib.h>

static uint32_t next = 1;

void srand(uint32_t seed) {
	next = seed;
}

int32_t rand() {
	next = 1103515245 * next + 12345;
	return next % RAND_MAX;
}