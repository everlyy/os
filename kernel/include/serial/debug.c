#include <serial/debug.h>
#include <serial/serial.h>
#include <printf.h>
#include <stdarg.h>

void debug_printf(const char* fmt, ...) {
	va_list args;
	va_start(fmt, args);

	printf(&serial_write, fmt, args);

	va_end(args);
}