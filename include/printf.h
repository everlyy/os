#pragma once

#include <types.h>
#include <stdarg.h>

typedef void (*putc_func)(char);

void printf(const putc_func putc, const char* fmt, va_list args);