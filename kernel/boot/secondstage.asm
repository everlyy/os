org 0x7E00
bits 16

secondstage_entry:
	xor ax, ax
	mov es, ax

	; Set VESA video mode
	; Currently hardcoded to be 1920x1080 with 32 bits per pixel
	mov ax, 1280
	mov bx, 720
	mov cl, 32
	call vbe_set_mode
	jc vbe_set_failed

	; Load GDT
	cli
	xor ax, ax
	mov ds, ax
	lgdt [gdt_description]

	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp 0x08:protected_mode_entry

vbe_set_failed:
	mov si, .message
	call print_string
	
	mov al, byte [vbe_status]
	add al, '0'
	mov ah, 0x0E
	int 0x10

	cli
	hlt
	.message: db "FATAL: Failed to set VBE mode. Status=", 0x00

%include "print_string.asm"
%include "vbe/vbe.asm"

; Everything after this is 32-bit
bits 32
protected_mode_entry:
	; Set up data segment descriptor
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; Stack pointer
	mov esp, 0x90000

	; Copy VBE mode info to 0x9000
	mov esi, mode_info_block
	mov edi, 0x9000
	mov ecx, 64		; Mode info block is 256 bytes, we're copying 32bit values so 256 / 4 = 64
	rep movsd

	; Jump to kernel
	jmp 0x08:0x50000

gdt_start:
	dq 0x00000000	; First descriptor is required to be NULL

	.code:
		dw 0xFFFF		; Segment limit 1
		dw 0x0000		; Segment base 1
		db 0x00			; Segment base 2
		db 0b10011010	; Access byte
		db 0b11001111	; Flags and segment limit 2
		db 0x00			; Segment base 3
	.data:
		dw 0xFFFF		; Segment limit 1
		dw 0x0000		; Segment base 1
		db 0x00			; Segment base 2
		db 0b10010010	; Access byte
		db 0b11001111	; Flags and segment limit 2
		db 0x00			; Segment base 3
gdt_end:

gdt_description:
	dw (gdt_end - gdt_start)
	dd gdt_start

%include "vbe/vbe_structures.asm"

; Pad with 0x00 to align with sector size (512 bytes)
times 2048-($-$$) db 0x00