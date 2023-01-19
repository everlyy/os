#pragma once

#include <types.h>

struct font_s {
	uint8_t character_width;
	uint8_t character_height;
	uint8_t* characters;
};
typedef struct font_s font_t;