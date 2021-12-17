section .asm

global idt_load
idt_load:
	push ebp
	mov ebp,esp
	
	mov ebx,[ebp + 8] ;The +8 is to receive the first argument passed to the function in the C code
	lidt [ebx]
	pop ebp
	ret
	
