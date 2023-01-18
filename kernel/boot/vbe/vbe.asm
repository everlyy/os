; https://wiki.osdev.org/User:Omarrx024/VESA_Tutorial

; Sets a VESA mode
; Inputs
;  AX: width
;  BX: height
;  CL: bits per pixel
; Outputs:
;  CF: Set on error
;  vbe_status: 0 on success
;  vbe_screen structure
vbe_set_mode:
	mov [.width], ax
	mov [.height], bx
	mov [.bits_per_pixel], cl

	xor ax, ax
	mov es, ax

	; Get VBE BIOS info
	push es
	mov ax, 0x4F00
	mov di, vbe_info_block
	int 0x10
	pop es

	; Check for errors, AX is 0x4F on success
	cmp ax, 0x4F
	jne .get_bios_info_error

	mov ax, word [vbe_info_block.video_modes]
	mov [.offset], ax
	mov ax, word [vbe_info_block.video_modes + 2]
	mov [.segment], ax

	mov fs, ax
	mov si, [.offset]

	.find_mode:
		mov dx, [fs:si]
		add si, 2
		mov [.offset], si
		mov [.mode], dx

		; Check if we're at the end of the list
		cmp dx, 0xFFFF
		je .end_of_list_error

		; Get VBE mode info 
		push es
		mov ax, 0x4F01
		mov cx, [.mode]
		mov di, mode_info_block
		int 0x10
		pop es

		cmp ax, 0x4F
		jne .get_mode_info_error

		mov ax, [.width]
		cmp ax, [mode_info_block.width]
		jne .next_mode

		mov ax, [.height]
		cmp ax, [mode_info_block.height]
		jne .next_mode

		mov al, [.bits_per_pixel]
		cmp al, [mode_info_block.bits_per_pixel]
		jne .next_mode

		; Found the correct VBE mode, set it
		push es
		mov ax, 0x4F02
		mov bx, [.mode]
		or bx, 0x4000		; Enable linear framebuffer
		mov di, 0
		int 0x10
		pop es

		cmp ax, 0x4F
		jne .set_mode_error

		clc
		mov byte [vbe_status], 0
		ret

	.next_mode:
		mov ax, [.segment]
		mov fs, ax
		mov si, [.offset]
		jmp .find_mode

	.get_bios_info_error:
		mov byte [vbe_status], 1
		jmp .error

	.end_of_list_error:
		mov byte [vbe_status], 2
		jmp .error

	.get_mode_info_error:
		mov byte [vbe_status], 3
		jmp .error

	.set_mode_error:
		mov byte [vbe_status], 4
		jmp .error

	.error:
		stc
		ret

	.width: dw 0x0000	
	.height: dw 0x0000	
	.bits_per_pixel: db 0x00	
	.segment: dw 0x0000
	.offset: dw 0x0000
	.mode: dw 0x0000

vbe_status: db 0x00