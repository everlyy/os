#include <terminal/terminal.h>
#include <vbe/vbe.h>
#include <graphics/graphics.h>
#include <graphics/font.h>
#include <stdbool.h>

static uint16_t cursor_x;
static uint16_t cursor_y;
static uint16_t term_width;
static uint16_t term_height;
static uint32_t background_color;
static uint32_t foreground_color;
static font_t* term_font;

static const char* characters = "0123456789abcdef";

void terminal_init(font_t* font, const uint32_t bg, const uint32_t fg) {
	cursor_x = 0;
	cursor_y = 0;
	background_color = bg;
	foreground_color = fg;
	term_font = font;
	term_width = vbe_mode_info->width / term_font->character_width;
	term_height = vbe_mode_info->height / term_font->character_height;

	graphics_clear_screen(background_color);
}

void terminal_putc(const char c) {
	if(c == '\n') {
		cursor_x = 0;
		cursor_y++;
	} else if(c == '\r') {
		cursor_x = 0;
	} else if(c == '\b') {
		// FIXME: This currently doesn't support backspacing up, fix that
		//  Probably a better thing is to keep a buffer with all the characters and draw that, instead of just drawing the characters
		if(cursor_x != 0)
			cursor_x--;
		graphics_clear_character(term_font, cursor_x * term_font->character_width, cursor_y * term_font->character_height, background_color);
	} else {
		graphics_draw_character(term_font, cursor_x * term_font->character_width, cursor_y * term_font->character_height, foreground_color, c);
		cursor_x++;
	}

	if(cursor_x >= term_width) {
		cursor_x = 0;
		cursor_y++;
	}

	if(cursor_y >= term_height) {
		// FIXME: Scroll the screen instead of starting from 0
		graphics_clear_screen(background_color);
		cursor_x = 0;
		cursor_y = 0;
	}
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