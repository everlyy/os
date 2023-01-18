; EVFS Filetable
; TODO: Create this automatically when compiling

filetable_headers:
	db "evfs"	; Signature
	dw 0x0000	; Checksum
	dw 0x0004	; Amount of entries
	dw 0x00FF	; Disk size in sectors

	; Padding
	times 32-($-filetable_headers) db 0

; Entries
bootsector_entry:
	dw 0x0001		; Starting sector
	dw 0x0001		; Size in sectors
	db 0x04			; Name length
	db 0x00			; Is free space
	times 10 db 0	; Unused
	.name:
		db "boot"
		times 16-($-.name) db 0

filetable_entry:
	dw 0x0002		; Starting sector
	dw 0x0002		; Size in sectors
	db 0x09			; Name length
	db 0x00			; Is free space
	times 10 db 0	; Unused
	.name:
		db "filetable"
		times 16-($-.name) db 0

secondstage_entry:
	dw 0x0004		; Starting sector
	dw 0x0004		; Size in sectors
	db 0x0B			; Name length
	db 0x00			; Is free space
	times 10 db 0	; Unused
	.name:
		db "secondstage"
		times 16-($-.name) db 0

kernel_entry:
	dw 0x0008		; Starting sector
	dw 0x000F		; Size in sectors
	db 0x06			; Name length
	db 0x00			; Is free space
	times 10 db 0	; Unused
	.name:
		db "kernel"
		times 16-($-.name) db 0

times 1024-($-$$) db 0