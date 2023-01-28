#pragma once

#include <types.h>

#define dbgprintf(...) _dbgprintf(__VA_ARGS__, "");
#define _dbgprintf(fmt, ...) debug_printf("[%s:%d %s] "fmt"%s", __FILE__, __LINE__, __func__, __VA_ARGS__)

void debug_print_string(const char* str);
void debug_print_char(const char c);
void debug_print_hex(const uint32_t num);
void debug_print_dec(const int32_t num);
void debug_printf(const char* fmt, ...);