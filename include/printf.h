#pragma once

#include <types.h>

typedef void (*putc_func)(char);

void printf(const putc_func putc, const char* fmt, uint32_t* arg_ptr);