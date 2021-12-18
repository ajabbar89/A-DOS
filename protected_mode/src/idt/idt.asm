section .asm

global idt_load
global generic_irq_handler_wrapper
global irq21_handler_wrapper

extern irq21_handler
extern generic_irq_handler

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
