#pragma once

#include <types.h>

int32_t memcmp(const void* s1, const void* s2, uint32_t n);
void* memcpy(void* dest, const void* src, uint32_t n);
uint32_t strlen(const char* str);