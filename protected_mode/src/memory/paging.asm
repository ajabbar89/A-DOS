section .asm

global page_directory_load
global enable_paging

page_directory_load:
	push ebp
	mov ebp,esp
	mov eax,[ebp + 8]
	mov cr3,eax
	pop ebp
	ret

enable_paging:
	push ebp
	mov ebp,esp
	mov eax,cr0
	or eax,0x80000000
	mov cr0,eax
	pop ebp
	ret
