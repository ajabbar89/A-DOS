section .asm

global idt_load
global generic_irq_handler_wrapper
global irq21_handler_wrapper
global enable_interrupts
global disable_interrupts
global _problem


extern irq21_handler
extern generic_irq_handler

enable_interrupts:
	sti
	ret

disable_interrupts:
	cli
	ret

idt_load:
	push ebp
	mov ebp,esp
	
	mov ebx,[ebp + 8] ;The +8 is to receive the first argument passed to the function in the C code
	lidt [ebx]
	pop ebp
	ret

generic_irq_handler_wrapper:
	cli
	pushad
	call generic_irq_handler
	popad
	sti
	iret

irq21_handler_wrapper:
	cli
	pushad
	call irq21_handler
	popad
	sti
	iret	

_problem:
	mov ax,0
	div ax ;to force divide by zero to validate Interrupt table

