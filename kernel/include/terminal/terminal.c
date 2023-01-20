#include <terminal/terminal.h>
#include <vbe/vbe.h>
#include <graphics/graphics.h>
#include <graphics/font.h>
#include <stdbool.h>
#include <string.h>
#include <addresses.h>
#include <terminal/cursor.h>
#include <keyboard/keyboard.h>

static char* terminal_screen = (char*)TERMINAL_SCREEN_ADDRESS;
static uint16_t cursor_x;
static uint16_t cursor_y;
static uint16_t term_width;
static uint16_t term_height;
static uint32_t background_color;
static uint32_t foreground_color;
static font_t* term_font;
static const char* characters = "0123456789abcdef";

static void draw_character(const char c, const uint32_t x, const uint32_t y) {
	uint8_t* character = &term_font->characters[c * term_font->character_height];
	graphics_draw_character(character, term_font->character_width, term_font->character_height, x * term_font->character_width, y * term_font->character_height, foreground_color);
}

static void put_character(const char c) {
	terminal_screen[cursor_y * term_width + cursor_x] = c;
	graphics_clear_character(term_font->character_width, term_font->character_height, cursor_x * term_font->character_width, cursor_y * term_font->character_height, background_color);
	draw_character(c, cursor_x, cursor_y);
}

static void redraw_screen() {
	graphics_clear_screen(background_color);

	for(uint32_t y = 0; y < term_height; y++) {
		for(uint32_t x = 0; x < term_width; x++) {
			draw_character(terminal_screen[y * term_width + x], x, y);
		}
	}
}

static void scroll() {
	// This should be memmove, but I don't know how to implement memmove without something like malloc :/
	memcpy(terminal_screen, &terminal_screen[term_width], term_width * (term_height - 1));
	memset(&terminal_screen[(term_height - 1) * term_width], 0, term_width);
	redraw_screen();
}

void terminal_init(font_t* font, const uint32_t bg, const uint32_t fg) {
	cursor_x = 0;
	cursor_y = 0;
	background_color = bg;
	foreground_color = fg;
	term_font = font;
	term_width = vbe_mode_info->width / term_font->character_width;
	term_height = vbe_mode_info->height / term_font->character_height;

	memset(terminal_screen, 0, term_width * term_height);
	redraw_screen();
}

void terminal_putc(const char c) {
	graphics_clear_character(cursor_width, cursor_height, cursor_x * term_font->character_width, cursor_y * term_font->character_height, background_color);

	if(c == '\n') {
		cursor_x = 0;
		cursor_y++;
	} else if(c == '\r') {
		cursor_x = 0;
	} else if(c == '\b') {
		// FIXME: Don't go to end of previous line, but actually find where characters start
		if(cursor_x == 0) {
			cursor_x = term_width - 1;
			if(cursor_y != 0)
				cursor_y--;
		} else {
			cursor_x--;
		}
		put_character(0);
	} else {
		put_character(c);
		cursor_x++;
	}

	if(cursor_x >= term_width) {
		cursor_x = 0;
		cursor_y++;
	}

	if(cursor_y >= term_height) {
		scroll();
		cursor_y--;
	}

	graphics_draw_character(cursor, cursor_width, cursor_height, cursor_x * term_font->character_width, cursor_y * term_font->character_height, foreground_color);
}

void terminal_puts(const char* str) {
	while(*str) {
		terminal_putc(*str);
		str++;
	}
}

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
		terminal_putc(buffer[i]);
}

void terminal_printf(const char* fmt, ...) {
	// FIXME: Don't duplicate code for terminal_printf and debug_printf
	uint32_t* arg_ptr = (uint32_t*)&fmt;
	arg_ptr++;

	while(*fmt) {
		if(*fmt == '%') {
			fmt++;
			char c = *fmt;
			switch(c) {
			case 'c':
				terminal_putc(*(char*)arg_ptr);
				arg_ptr++;
				break;

			case 's':
				terminal_puts(*(char**)arg_ptr);
				arg_ptr++;
				break;

			case 'd':
				print_number(*(int32_t*)arg_ptr, 10, true);
				arg_ptr++;
				break;

			case 'x':
				print_number(*(uint32_t*)arg_ptr, 16, false);
				arg_ptr++;
				break;

			case '%':
				terminal_putc('%');
				break;

			default:
				terminal_putc('%');
				terminal_putc(c);
				break;
			}
		} else {
			terminal_putc(*fmt);
		}
		fmt++;
	}
}

uint32_t terminal_wait_for_command(char* input_buffer, uint32_t input_buffer_length) {
	uint32_t user_input_length = 0;

	while(1) {
		uint8_t scancode = keyboard_get_scancode();
		char c = keyboard_scancode_to_ascii(scancode);

		if(!c)
			continue;

		if(c == '\n') {
			terminal_putc(c);
			break;
		}

		// Handle backspace where user removes their input
		if(c == '\b' && user_input_length > 0) {
			user_input_length--;
			terminal_putc(c);
			continue;
		}

		// This backspace would delete the prompt characters or other things
		// that weren't typed by the user
		if(c == '\b')
			continue;

		terminal_putc(c);
		input_buffer[user_input_length++] = c;

		if(user_input_length >= input_buffer_length - 1)
			break;
	}

	input_buffer[user_input_length] = '\0';
	return user_input_length;
}