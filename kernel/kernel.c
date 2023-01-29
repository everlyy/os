#include <types.h>
#include <serial/serial.h>
#include <serial/debug.h>
#include <evfs/evfs.h>
#include <graphics/font.h>
#include <graphics/graphics.h>
#include <terminal/terminal.h>
#include <keyboard/keyboard.h>
#include <string.h>
#include <addresses.h>
#include "image.h"

void panic() {
	graphics_clear_screen(0xFF0000);
	while(1);
}

void load_font_from_file(font_t* font, uint8_t* font_buffer, const char* name) {
	filetable_entry_t* font_entry = NULL;
	int32_t status = evfs_get_entry_by_name(strlen(name), name, &font_entry);
	if(status < EVFS_STATUS_SUCCESS) {
		dbgprintf("Couldn't get font entry: %d\n", status);
		panic();
	}

	status = evfs_read_file(font_entry, font_buffer);
	if(status < EVFS_STATUS_SUCCESS) {
		dbgprintf("Couldn't read font entry: %d\n", status);
		panic();
	}

	font->character_width = 8;
	font->character_height = 16;
	font->characters = font_buffer;

	dbgprintf("Loaded font. start=%d size=%d buffer=0x%x\n", font_entry->starting_sector, font_entry->size_in_sectors, (uint32_t)font_buffer);
}

__attribute__((section("kernel_entry"))) void kernel_main(void) {
	serial_init();
	keyboard_init();

	int32_t status = evfs_verify_filetable();
	if(status < EVFS_STATUS_SUCCESS)
		dbgprintf("Couldn't verify filetable\n");

	dbgprintf("FILETABLE DUMP:\n");
	for(uint16_t i = 0; i < filetable->number_of_entries; i++) {
		filetable_entry_t* entry = &filetable->entries[i];
		if(entry->is_free_space)
			continue;

		char name[MAX_NAME_LENGTH + 1];
		memset(name, ' ', MAX_NAME_LENGTH);
		memcpy(name, entry->name, entry->name_length);
		name[MAX_NAME_LENGTH] = '\0';

		dbgprintf("  %s start=%d size=%d\n", name, entry->starting_sector, entry->size_in_sectors);
	}

	font_t font = { 0 };
	load_font_from_file(&font, (uint8_t*)FONT_ADDRESS, "font");
	terminal_init(&font, 0x0E0E0E, 0xFFFFFF);

	terminal_puts("abcdefghijklmnopqrstuvwxyz\n");
	terminal_puts("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
	terminal_puts("0123456789\n");
	terminal_puts("!@#$%^&*()-=_+[]{};:'\",<.>/?|\\\n");

	terminal_printf("Hello, world!\nformat tests:\n  char: %c\n  string: %s\n  decimal: %d\n  hex: %x\n  percent: %%\n", 'a', "hi", 12345, 0xABCD1234);

	char buffer[128];

	while(1) {
		terminal_printf("$ ");
		uint32_t length = terminal_wait_for_command(buffer, 128);
		terminal_printf("entered %d characters: %s\n", length, buffer);
	}
}