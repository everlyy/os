#include <vbe/vbe.h>

void vbe_set_pixel(uint16_t x, uint16_t y, uint32_t color) {
	uint32_t pixel = y * vbe_mode_info->pitch + (x * (vbe_mode_info->bits_per_pixel / 8)) + vbe_mode_info->physical_base_pointer;
	*(uint32_t*)pixel = color;
}