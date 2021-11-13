ORG 0x0
BITS 16 ; For NASM or the assembler to assemble the code into machine code for 16bit system

_entry:
	jmp short _start_after_bpb
	nop

times 33 db 0x0

_start_after_bpb:
	jmp 0x7C0:_start

_start:
	cli
	mov ax,0x7C0
	mov ds,ax
	mov es,ax
	xor ax,ax
	mov ss,ax
	mov sp,0x7C00
	sti

	mov ah,2 ; READ SECTOR COMMAND with INT 0x13
	mov al,1 ; ONE SECTOR TO READ
	mov ch,0 ; Cylinder low eight bits
	mov cl,2 ; Read sector 2
	mov dh,0 ; Head number
	mov bx,buffer
	int 0x13
	jc _error
	
	mov si,buffer
	call _print
	jmp $

_error:
	mov si,error_message
	call _print
	jmp $

_print:
	mov bx,0x0
	lodsb
	cmp al,0x0
	je _done
	call _printchar
	jmp _print

_done:
	ret

_printchar:
	mov ah,0x0E
	int 0x10
	ret

error_message: db 'Unable to read sector',0x0

times 510-($-$$) db 0x0
dw 0xAA55

buffer:			; The memory location into which the sector from HDD shall be loaded
