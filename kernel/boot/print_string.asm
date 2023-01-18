; Print null-terminated string in SI
print_string:
	push si
	push ax
	mov ah, 0x0E

	.loop:
		cmp byte [si], 0x00
		je .return

		mov al, [si]
		int 0x10

		inc si
		jmp .loop

	.return:
		pop ax
		pop si
		ret