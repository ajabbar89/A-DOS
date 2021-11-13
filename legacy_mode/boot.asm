;	@file	boot.asm
;
;	@brief This shall be the bootloader in Real Mode for our OS
;	The assembly code is written in Intel/NASM style or dialect
;	which can be assembled using "NASM" assembler

;In Real Mode, BIOS starts executing code from bootloader from address 0x7C00

ORG 0x0
BITS 16 ;Telling the assembler to assemble instructions into 16bit code

_entry:
	jmp short _start_after_bpb  ;bpb = BIOS parameter block. BIOS's typically expect the bootloader to have this set of data before the actual BLR code
	nop

times 33 db 0x0

_start_after_bpb:
	jmp 0x7C0:_start

_start:
	cli ;clearing all interrupts before initing the segment registers
	mov ax,0x7C0
	mov ds,ax ;Ensuring the data segment has the right start address so that lodsb will fetch the right data 'message' from memory
	mov es,ax
	mov ax,00
	mov ss,ax
	mov sp,0x7C00
	sti ;reenabling interrupts

	mov si,message
	mov bx,0
	call _print
	jmp $ ;Loop continuously at this line

_print:
	lodsb
	cmp al,0x0
	je _done
	call _print_char
	jmp _print
_done:
	ret

_print_char:	
	mov ah,0x0E
	int 0x10 ;This interrupt will call the BIOS as BIOS would have setup this interrupt for use.
	;The above snippet is basically forcing 'A' to be output onto terminal or video memory. Here is some documentation. http://www.ctyme.com/intr/int.htm
	ret

message: db 'Hello World!',0

;Looking for 0x55AA signature while searching for a FAT filesystem to be loaded into memory by bootloader	
times 510-($-$$) db 0 ;This says ensure that our code has 510 bytes of data. So, the code written above, if it doesn't
		      ;consume 510 bytes, this filling up command will ensure that 510 bytes of data gets filled
		      ;and if not filled, it shall be filled with 0x00
dw 0xAA55
	
