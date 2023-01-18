#include <types.h>
#include <vbe/vbe.h>
#include "cat_image.h"

#define ASM __asm__ __volatile__

__attribute__((section("kernel_entry"))) void kernel_main(void) {
	for(uint32_t y = 0; y < image_h; y++) {
		for(uint32_t x = 0; x < image_w; x++) {
			vbe_set_pixel(x, y, image[y * image_w + x]);
		}
	}
	while(1);
}