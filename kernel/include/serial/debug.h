#pragma once

#include <types.h>

void debug_print_string(const char* str);
void debug_print_char(const char c);
void debug_print_hex(const uint32_t num);
void debug_print_dec(const int32_t num);
void debug_printf(const char* fmt, ...);