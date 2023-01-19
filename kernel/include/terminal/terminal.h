#pragma once

#include <types.h>
#include <graphics/font.h>

void terminal_init(font_t* font, const uint32_t bg, const uint32_t fg);
void terminal_putc(const char c);
void terminal_puts(const char* str);
void terminal_printf(const char* fmt, ...);