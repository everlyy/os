#pragma once

#include <types.h>
#include <graphics/font.h>

void terminal_init(font_t* font, const uint32_t bg, const uint32_t fg);
void terminal_putc(const char c);
void terminal_puts(const char* str);
void terminal_printf(const char* fmt, ...);
uint32_t terminal_wait_for_command(char* input_buffer, uint32_t input_buffer_length);