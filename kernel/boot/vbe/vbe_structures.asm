vbe_info_block:
	.signature: db "VBE2"
	.version: dw 0x0000
	.oem_string_pointer: dd 0x00000000
	.capabilities: dd 0x00000000
	.video_modes: dd 0x00000000
	.video_memory: dw 0x0000
	.oem_software_revision: dw 0x0000
	.oem_vendor_name_pointer: dd 0x00000000
	.oem_product_name_pointer: dd 0x00000000
	.oem_product_revision_pointer: dd 0x00000000
	.reserved0: times 222 db 0x00
	.oem_data: times 256 db 0x00

mode_info_block:
	; Mandatory for all VBE revisions
	.mode_attributes: dw 0x0000
	.window_a: db 0x00
	.window_b: db 0x00
	.granularity: dw 0x0000
	.window_size: dw 0x0000
	.a_segment: dw 0x0000
	.b_segment: dw 0x0000
	.window_function_pointer: dd 0x00000000
	.pitch: dw 0x0000

	; Mandatory for VBE 1.2 and up
	.width: dw 0x0000
	.height: dw 0x0000
	.w_char: db 0x00
	.h_char: db 0x00
	.number_of_planes: db 0x00
	.bits_per_pixel: db 0x00
	.number_of_banks: db 0x00
	.memory_model: db 0x00
	.bank_size: db 0x00
	.number_of_image_pages: db 0x00
	.reserved1: db 0x00

	; Direct color fields
	.red_mask_size: db 0x00
	.red_field_position: db 0x00
	.green_mask_size: db 0x00
	.green_field_position: db 0x00
	.blue_mask_size: db 0x00
	.blue_field_position: db 0x00
	.reserved_mask_size: db 0x00
	.reserved_field_position: db 0x00
	.direct_color_mode_info: db 0x00

	; Mandatory for VBE 2.0 and up
	.physical_base_pointer: dd 0x00000000
	.reserved2: dd 0x00000000
	.reserved3: dw 0x0000

	; Mandatory for VBE 3.0 and up
	.linear_bytes_per_scanline: dw 0x0000
	.bank_number_of_image_pages: db 0x00
	.linear_number_of_image_pages: db 0x00
	.linear_red_mask_size: db 0x00
	.linear_red_field_position: db 0x00
	.linear_green_mask_size: db 0x00
	.linear_green_field_position: db 0x00
	.linear_blue_mask_size: db 0x00
	.linear_blue_field_position: db 0x00
	.linear_reserved_mask_size: db 0x00
	.linear_reserved_field_position: db 0x00
	.max_pixel_clock: dd 0x00000000

	.reserved4: times 190 db 0x00