#include <serial/debug.h>
#include <serial/serial.h>
#include <stdbool.h>

static const char* characters = "0123456789abcdef";

static void print_number(const int32_t num, const uint8_t base, const bool sign) {
	char buffer[16] = { 0 };
	bool negative = false;
	int32_t i = 0;
	uint32_t n = num;

	if(sign && num < 0) {
		negative = true;
		n = -num;
	}

	do {
		buffer[i++] = characters[n % base];
	} while((n /= base) > 0);

	if(negative)
		buffer[i++] = '-';

	while(--i >= 0)
		serial_write(buffer[i]);
}

void debug_print_string(const char* str) {
	while(*str) {
		serial_write(*str++);
	}
}

void debug_print_char(const char c) {
	serial_write(c);
}

void debug_print_hex(const uint32_t num) {
	print_number(num, 16, false);
}

void debug_print_dec(const int32_t num) {
	print_number(num, 10, true);
}

void debug_printf(const char* fmt, ...) {
	uint32_t* arg_ptr = (uint32_t*)&fmt;
	arg_ptr++;

	while(*fmt) {
		if(*fmt == '%') {
			fmt++;
			char c = *fmt;
			switch(c) {
			case 'c':
				serial_write(*(char*)arg_ptr);
				arg_ptr++;
				break;

			case 's':
				debug_print_string(*(char**)arg_ptr);
				arg_ptr++;
				break;

			case 'd':
				print_number(*(int32_t*)arg_ptr, 10, true);
				arg_ptr++;
				break;

			case 'x':
				print_number(*(uint32_t*)arg_ptr, 16, false);
				arg_ptr++;
				break;

			case '%':
				serial_write('%');
				break;

			default:
				serial_write('%');
				serial_write(c);
				break;
			}
		} else {
			serial_write(*fmt);
		}
		fmt++;
	}
}