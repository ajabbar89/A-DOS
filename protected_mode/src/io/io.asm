section .asm

global insb
global insw
global outb
global outw

insb:
	push ebp
	mov ebp,esp
	xor eax,eax
	mov edx,[ebp + 8]
	in al,dx ;the byte is copied into al which is part of eax which will be the return value for this asm function
	pop ebp
	ret
insw:
	push ebp
	mov ebp,esp
	xor eax,eax
	mov edx,[ebp + 8]
	in ax,dx
	pop ebp
	ret

outb:
	push ebp
	mov ebp,esp
	mov eax,[ebp+12]
	mov edx,[ebp+8]
	out dx,al
	pop ebp
	ret
outw:
	push ebp
	mov ebp,esp
	mov eax,[ebp + 12]
	mov edx,[ebp + 8]
	out dx,ax
	pop ebp
	ret
