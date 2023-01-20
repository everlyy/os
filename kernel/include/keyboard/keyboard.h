#pragma once

#include <types.h>

int32_t keyboard_init();
uint8_t keyboard_get_scancode();
uint8_t keyboard_scancode_to_ascii(uint8_t scancode);
