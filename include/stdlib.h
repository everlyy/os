#pragma once

#include <types.h>

#define RAND_MAX 0x7FFF

void srand(uint32_t seed);
int32_t rand();