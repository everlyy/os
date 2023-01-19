#include <types.h>
#include <serial/serial.h>
#include <serial/debug.h>
#include <evfs/evfs.h>
#include <graphics/font.h>
#include <graphics/graphics.h>
#include <vbe/vbe.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

#define FONT_ADDRESS 0x10000

void panic() {
	graphics_clear_screen(0xFF0000);
	while(1);
}

__attribute__((section("kernel_entry"))) void kernel_main(void) {
	serial_init();

	graphics_clear_screen(0x0E0E0E);

	const char* font_name = "font";
	uint8_t* font_buffer = (uint8_t*)FONT_ADDRESS;

	filetable_entry_t* font_entry = NULL;
	int32_t status = evfs_get_entry_by_name(strlen(font_name), font_name, &font_entry);
	if(status < EVFS_STATUS_SUCCESS) {
		debug_printf("Couldn't get font entry: %d\n", status);
		panic();
	}

	status = evfs_read_file(font_entry, font_buffer);
	if(status < EVFS_STATUS_SUCCESS) {
		debug_printf("Couldn't read font entry: %d\n", status);
		panic();
	}

	font_t font = {
		.character_width = 8,
		.character_height = 16,
		.characters = font_buffer
	};

	debug_printf("Loaded font. start=%d size=%d buffer=0x%x\n", font_entry->starting_sector, font_entry->size_in_sectors, (uint32_t)font_buffer);
	graphics_draw_bitmap(image, 0, 0, image_w, image_h);
	graphics_draw_character(&font, 0, 32, 0xFFFFFF, 'W');
	graphics_draw_character(&font, 8, 32, 0xFFFFFF, 'T');
	graphics_draw_character(&font, 16, 32, 0xFFFFFF, 'F');

	while(1);
}