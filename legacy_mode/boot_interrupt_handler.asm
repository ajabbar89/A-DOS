ORG 0x0
BITS 16

_entry:
	jmp short _start_after_bpb
	nop

times 33 db 0x0

_start_after_bpb:
	jmp 0x7C0:_start

handler_zero:
	mov bx,0x0	
	mov ah,0x0E
	mov al,'Z'
	int 0x10
	iret

handler_one:
	mov bx,0x0
	mov ah,0x0E
	mov al,'O'
	int 0x10
	iret

_start:
	cli
	mov ax,0x7C0
	mov ds,ax
	mov es,ax
	xor ax,ax
	mov ss,ax
	mov sp,0x7C00
	sti

	mov word[ss:0x0],handler_zero
	mov word[ss:0x2],0x7C0

	mov word[ss:0x4],handler_one
	mov word[ss:0x6],0x7C0

	xor ax,ax
	div ax ; Exception -> divide by zero. Will invoke INT 0x0 which will pick the vector from 0x0-0x3 for handling it

	int 0x1

	mov bx,0x0
	mov si,message
	call _print
	jmp $

_print:
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

message: db 'Hello World!',0x0

times 510-($-$$) db 0x0
dw 0xAA55
