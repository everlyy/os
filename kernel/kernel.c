#include <types.h>
#include <serial/serial.h>
#include <serial/debug.h>

__attribute__((section("kernel_entry"))) void kernel_main(void) {
	serial_init();

	debug_print_string("Hello, serial!\n");
	debug_print_dec(12345);
	debug_print_char('\n');
	debug_print_hex(0x1234ABCD);
	debug_print_char('\n');


	while(1);
}