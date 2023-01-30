#include <serial/debug.h>
#include <serial/serial.h>
#include <stdbool.h>
#include <printf.h>

void debug_printf(const char* fmt, ...) {
	uint32_t* arg_ptr = (uint32_t*)&fmt;
	arg_ptr++;

	printf(&serial_write, fmt, arg_ptr);
}