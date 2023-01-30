#include <printf.h>
#include <types.h>

static const char* characters = "0123456789abcdef";

static void print_number(const putc_func putc, const int32_t num, const uint8_t base, const bool sign) {
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
		putc(buffer[i]);
}

static void puts(putc_func putc, const char* str) {
	while(*str) {
		putc(*str);
		str++;
	}
}

void printf(const putc_func putc, const char* fmt, va_list args) {
	// TODO: Use a write() function and use a buffer to print instead of just printing everything character by character

	while(*fmt) {
		if(*fmt == '%') {
			fmt++;
			char c = *fmt;
			switch(c) {
			case 'c':
				putc(va_arg(args, char));;
				break;

			case 's':
				puts(putc, va_arg(args, char*));
				break;

			case 'd':
				print_number(putc, va_arg(args, int32_t), 10, true);
				break;

			case 'x':
				print_number(putc, va_arg(args, uint32_t), 16, false);
				break;

			case '%':
				putc('%');
				break;

			default:
				putc('%');
				putc(c);
				break;
			}
		} else {
			putc(*fmt);
		}
		fmt++;
	}
}