#include <types.h>
#include <serial/serial.h>
#include <vbe/vbe.h>
#include <stdlib.h>

__attribute__((section("kernel_entry"))) void kernel_main(void) {
	serial_init();

	srand(435345);

	while(1) {
		for(uint32_t y = 0; y < vbe_mode_info->height; y++) {
			for(uint32_t x = 0; x < vbe_mode_info->width; x++) {
				uint8_t r = rand() % 0xFF;
				uint8_t g = rand() % 0xFF;
				uint8_t b = rand() % 0xFF;
				vbe_set_pixel(x, y, (r << 16) | (g << 8) | b);
			}
		}
	}
}