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
		serial_write(*str);
		str++;
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