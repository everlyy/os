#pragma once

#include <types.h>

#define dbgprintf(...) _dbgprintf(__VA_ARGS__, "");
#define _dbgprintf(fmt, ...) debug_printf("[%s:%d %s] "fmt"%s", __FILE__, __LINE__, __func__, __VA_ARGS__)

void debug_printf(const char* fmt, ...);