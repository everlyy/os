#pragma once

#include <types.h>

typedef uint32_t* va_list;

#define va_start(arg, list) do { list = (uint32_t*)&arg; list++; } while(0)
#define va_arg(list, type) (*(type*)list++)
#define va_end(list) (list) = NULL