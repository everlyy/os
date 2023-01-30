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

void printf(const putc_func putc, const char* fmt, uint32_t* arg_ptr) {
	while(*fmt) {
		if(*fmt == '%') {
			fmt++;
			char c = *fmt;
			switch(c) {
			case 'c':
				putc(*(char*)arg_ptr);
				arg_ptr++;
				break;

			case 's':
				puts(putc, *(char**)arg_ptr);
				arg_ptr++;
				break;

			case 'd':
				print_number(putc, *(int32_t*)arg_ptr, 10, true);
				arg_ptr++;
				break;

			case 'x':
				print_number(putc, *(uint32_t*)arg_ptr, 16, false);
				arg_ptr++;
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