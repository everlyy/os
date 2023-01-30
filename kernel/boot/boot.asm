org 0x7C00
bits 16

%define FILETABLE_SECTOR 2
%define FILETABLE_SIZE 2
%define FILETABLE_ADDRESS 0x2000

%define FILETABLE_ENTRY_SIZE 32
%define FILETABLE_ENTRY_HEADER_SIZE 16
%define FILETABLE_HEADER_SIZE 32

boot_entry:
	mov byte [drive_number], dl

	mov cl, FILETABLE_SECTOR
	mov al, FILETABLE_SIZE
	xor bx, bx
	mov es, bx
	mov bx, FILETABLE_ADDRESS
	call read_disk

	; Make sure that filetable was read correctly
	call verify_filetable
	jc invalid_filetable

read_filetable_entries:
	; Read secondstage bootloader
	mov si, secondstage_filename
	mov dl, byte [secondstage_filename.length]
	call find_entry_by_name
	mov cl, [di+0]	
	mov al, [di+2]
	xor bx, bx
	mov es, bx
	mov bx, 0x7E00
	call read_disk

	; Read kernel
	mov si, kernel_filename
	mov dl, byte [kernel_filename.length]
	call find_entry_by_name
	mov cl, [di+0]
	mov al, [di+2]
	mov bx, 0x5000
	mov es, bx
	xor bx, bx
	call read_disk

	jmp 0x0000:0x7E00

read_disk:
	mov ah, 0x02
	mov ch, 0x00
	mov dh, 0x00
	mov dl, [drive_number]
	int 0x13
	jc read_error
	cmp ah, 0
	jne read_error 
	ret

read_error:
	mov si, .message
	call print_string
	jmp boot_failed
	.message: db "FATAL: read disk failed", 0x00

boot_failed:
	mov si, .message
	call print_string
	cli
	hlt
	.message: db 0x0A, 0x0D, "Failed to boot", 0x00

verify_filetable:
	pusha

	; First 4 bytes of filetable shoudl be "evfs"
	mov cx, 4
	mov di, filetable_signature
	mov si, FILETABLE_ADDRESS
	repe cmpsb
	jne .invalid

	call verify_filetable_checksum
	jc .invalid
	jmp .return

	.invalid:
		stc
		jmp .return

	.return:
		popa
		ret

verify_filetable_checksum:
	clc

	call calculate_filetable_checksum		; DI = Calculated checksum
	mov dx, word [FILETABLE_ADDRESS + 4]	; DX = Checksum in filetable
	cmp dx, di
	jne .invalid_checksum
	jmp .return

	.invalid_checksum:
		stc

	.return:
		ret

; Calculate checksum of filetable in memory
; Returns:
;  DI: Checksum
calculate_filetable_checksum:
	mov si, FILETABLE_ADDRESS

	mov ax, [si + 6]	; Save number of entries in filetable to AX

	; Clear some registers
	xor di, di
	xor bx, bx
	xor cx, cx

	mov dx, FILETABLE_ENTRY_SIZE
	mul dx							; AX = entries count * sizeof entry

	add si, FILETABLE_HEADER_SIZE	; SI now points to start of entries list

	.loop:
		cmp bx, ax	; Check if we reached end of entries
		jge .return

		; This just adds whatever is at (si + bx) to di,
		; it's the 'checksum' for my filetable
		mov cl, byte [si + bx]
		add di, cx

		inc bx
		jmp .loop

	.return:
		ret

invalid_filetable:
	mov si, .message
	call print_string

	; Ask user if they want to continue even though verification failed.
	mov ah, 0x00
	int 0x16

	mov ah, 0xE
	int 0x10

	cmp al, 'Y'
	je read_filetable_entries
	jmp boot_failed
	.message: db "Failed to verify filetable. Proceed? (Y/n)", 0x0A, 0x0D, "# ", 0x00

%include "print_string.asm"

; Reads the filetable and finds an entry by name
; Input:
;  SI: pointer to filename
;  DL: length of filename
;
; Returns:
;  DI: Filetable entry
find_entry_by_name:
	; Make DI point at name of first entry
	mov di, FILETABLE_ADDRESS
	add di, FILETABLE_HEADER_SIZE
	add di, FILETABLE_ENTRY_HEADER_SIZE

	.loop:
		; Check if is_free_space is set (deleted entry) and skip
		cmp byte [di - 11], 0x00
		jne .continue

		; This is really just a memcmp in assembly
		; CL is the amount of bytes to compare
		; the 2 registers on the stack are the 2 memory regions to compare
		mov cl, dl
		push di
		push si
		repe cmpsb
		je .return
		pop si
		pop di
		
		.continue:
			add di, FILETABLE_ENTRY_SIZE	; Skip to next entry
			jmp .loop

	.return:
		pop si
		pop di
		sub di, FILETABLE_ENTRY_HEADER_SIZE	; Point DI at start of entry
		ret

kernel_filename: db "kernel"
	.length: db $ - kernel_filename

secondstage_filename: db "secondstage"
	.length: db $ - secondstage_filename

drive_number: db 0x00
filetable_signature: db "evfs"

times 510-($-$$) db 0x00
dw 0xAA55