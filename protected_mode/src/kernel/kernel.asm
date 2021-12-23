[BITS 32]

global _start
global _problem
global _setup_PIC

extern kernel_main

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
        mov ax,DATA_SEG
        mov ds,ax
        mov es,ax
        mov fs,ax
        mov gs,ax
        mov ss,ax
        mov ebp,0x00200000
        mov esp,ebp

        ;Setting A20 line
        in al,0x92
        or al,2
        out 0x92,al     

        call kernel_main
	jmp $


_setup_PIC:
	;Mapping the Programmable Interrupt controller to start IRQs from 20
	mov al, 00010001b
	out 0x20,al ; Master PIC's port is 0x20

	mov al,0x20 ; Interrupt 0x20 is where IRQs must start, IRQs here referring to system peripheral interrupts
	out 0x21,al

	mov al,00000001b
	out 0x21,al
	;End of PIC mapping

	ret


_problem:
	mov ax,0
	div ax ;to force divide by zero to validate Interrupt table

times 512-($-$$) db 0x0 ; To ensure that the asm code is aligned to 512 bytes to not to mess up any C code's alignment
	                ; This asm file will be treated as .text section by the linker script while linking since we
			; have not labelled with any section label. Also, we want this code to be the one loaded at 
			; 0x100000 location in memory. So, if other C code is present which makes up the final kernel.bin 
			; along with this .asm, the other C code will only be linked after this but their alignment 
			; should not be messed up, hence padding with 0x0 data upto 512bytes.
