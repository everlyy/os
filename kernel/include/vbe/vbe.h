#pragma once

#include <types.h>
#include <addresses.h>

struct vbe_mode_info_s {
	uint16_t mode_attributes;
	uint8_t window_a;
	uint8_t window_b;
	uint16_t granularity;
	uint16_t window_size;
	uint16_t a_segment;
	uint16_t b_segment;
	uint32_t window_function_pointer;
	uint16_t pitch;

	uint16_t width;
	uint16_t height;
	uint8_t w_char;
	uint8_t h_char;
	uint8_t number_of_planes;
	uint8_t bits_per_pixel;
	uint8_t number_of_banks;
	uint8_t memory_model;
	uint8_t bank_size;
	uint8_t number_of_image_pages;
	uint8_t reserved1;

	uint8_t red_mask_size;
	uint8_t red_field_position;
	uint8_t green_mask_size;
	uint8_t green_field_position;
	uint8_t blue_mask_size;
	uint8_t blue_field_position;
	uint8_t reserved_mask_size;
	uint8_t reserved_field_position;
	uint8_t direct_color_mode_info;

	uint32_t physical_base_pointer;
	uint32_t reserved2;
	uint16_t reserved3;

	uint16_t linear_bytes_per_scanline;
	uint8_t bank_number_of_image_pages;
	uint8_t linear_number_of_image_pages;
	uint8_t linear_red_mask_size;
	uint8_t linear_red_field_position;
	uint8_t linear_green_mask_size;
	uint8_t linear_green_field_position;
	uint8_t linear_blue_mask_size;
	uint8_t linear_blue_field_position;
	uint8_t linear_reserved_mask_size;
	uint8_t linear_reserved_field_position;
	uint32_t max_pixel_clock;

	uint8_t reserved4[190];
};
typedef struct vbe_mode_info_s vbe_mode_info_t;

vbe_mode_info_t* vbe_mode_info = (vbe_mode_info_t*)VBE_MODE_INFO_ADDRESS;

void vbe_set_pixel(uint16_t x, uint16_t y, uint32_t color);