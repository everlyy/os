#pragma once

#include <types.h>
#include <graphics/font.h>

void graphics_clear_screen(const uint32_t color);
void graphics_draw_bitmap(const uint32_t* bitmap, const uint32_t x, const uint32_t y, const uint32_t w, const uint32_t h);
void graphics_draw_character(const uint8_t* character, const uint8_t char_width, const uint8_t char_height, const uint32_t x, const uint32_t y, const uint32_t color);
void graphics_clear_character(const uint8_t char_width, const uint8_t char_height, const uint32_t x, const uint32_t y, const uint32_t color);