#pragma once

#include <types.h>

int32_t serial_init();
void serial_write(const char c);
char serial_read();