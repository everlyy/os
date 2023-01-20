#include <graphics/graphics.h>
#include <vbe/vbe.h>

void graphics_clear_screen(const uint32_t color) {
	// Probably the least efficient way to clear the screen, but it works :-)
	for(uint32_t y = 0; y < vbe_mode_info->height; y++) {
		for(uint32_t x = 0; x < vbe_mode_info->width; x++) {
			vbe_set_pixel(x, y, color);
		}
	}
}

void graphics_draw_character(const uint8_t* character, const uint8_t char_width, const uint8_t char_height, const uint32_t x, const uint32_t y, const uint32_t color) {
	for(uint8_t fy = 0; fy < char_height; fy++) {
		for(uint8_t fx = 0; fx < char_width; fx++) {
			uint8_t draw_pixel = (character[fy] >> fx) & 1;

			if(!draw_pixel)
				continue;

			uint32_t drawx = x + (char_width - fx);
			uint32_t drawy = y + fy;

			if(drawx >= vbe_mode_info->width || drawy >= vbe_mode_info->height)
				continue;

			vbe_set_pixel(drawx, drawy, color);
		}
	}
}

void graphics_draw_bitmap(const uint32_t* bitmap, const uint32_t x, const uint32_t y, const uint32_t w, const uint32_t h) {
	for(uint32_t bx = 0; bx < w; bx++) {
		for(uint32_t by = 0; by < h; by++) {
			uint32_t drawx = x + bx;
			uint32_t drawy = y + by;

			if(drawx >= vbe_mode_info->width || drawy >= vbe_mode_info->height)
				continue;

			uint32_t color = bitmap[by * w + bx];
			vbe_set_pixel(drawx, drawy, color);
		}
	}
}

void graphics_clear_character(const uint8_t char_width, const uint8_t char_height, const uint32_t x, const uint32_t y, const uint32_t color) {
	for(uint8_t fy = 0; fy < char_height; fy++) {
		for(uint8_t fx = 0; fx < char_width; fx++) {
			uint32_t drawx = x + (char_width - fx);
			uint32_t drawy = y + fy;

			if(drawx >= vbe_mode_info->width || drawy >= vbe_mode_info->height)
				continue;

			vbe_set_pixel(drawx, drawy, color);
		}
	}
}