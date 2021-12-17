ORG 0x7C00
BITS 16

_entry:
	jmp short _start_after_bpb_read
	nop

times 33 db 0x0

_start_after_bpb_read:
	jmp 0x0:_start_16

_start_16:
	cli
	mov ax,0x0
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov sp,0x7C00
	sti

_enter_protected:
	cli
	lgdt[gdt_descriptor]
	mov eax,cr0
	or eax,1
	mov cr0,eax
	jmp CODE_SEG:_load32


gdt_start:

gdt_null:
	dd 0x0
	dd 0x0

gdt_code:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 0x9a
	db 11001111b
	db 0x0	

gdt_data:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 0x92
	db 11001111b
	db 0x0

gdt_end:

gdt_descriptor:
	dw gdt_end-gdt_start-1
	dd gdt_start

CODE_SEG equ gdt_code-gdt_start
DATA_SEG equ gdt_data-gdt_start

; Driver in assembly to read from Disk. We cannot use INT 0x13 as BIOS interrupts are not 
; available in 32bit mode
[BITS 32]
_load32:
	mov eax,1
	mov ecx,100 ;Reading 100 sectors from HDD
	mov edi,0x100000 ;The memory location where kernel must be loaded	
	call ata_lba_read
	jmp CODE_SEG:0x100000

ata_lba_read:
	mov ebx,eax ;Backup the LBA
	; Send the highest 8 bits of the LBA to the HDD controller
	shr eax,24
	or eax,0xE0 ;Selects the master drive
	mov dx,0x1F6 ;0x1F6 is the port to which the LBA bits are to be written to
	out dx,al
	;Finished sending the highest 8 bits of the LBA

	;Send the total sectors to read
	mov eax,ecx
	mov dx,0x1F2
	out dx,al 
	;Finished sending the total sectors to read

	;Send more bits of the LBA
	mov eax,ebx ; Restore the backup LBA
	mov dx,0x1F3
	out dx,al
	;Finished sending more bits of the LBA

	;Send more bits of the LBA	
	mov dx,0x1F4
	mov eax,ebx ;Restore the backup LBA
	shr eax,8
	out dx,al
	;Finished sending more bits of the LBA

	;Send upper 16 bits of the LBA
	mov dx,0x1F5
	mov eax,ebx ;Restore the backup LBA
	shr eax,16
	out dx,al
	;Finished sending the upper 16 bits of the LBA

	mov dx,0x1F7
	mov al,0x20
	out dx,al

	;Read all sectors into memory
.next_sector:
	push ecx ;ecx will get decremented here representing pending num of sectors to read into memory

;Checking if we need to read
.try_again:
	mov dx,0x1F7
	in al,dx
	test al,8
	jz .try_again

; We need to read 256 words at a time
	mov ecx,256 ;256 words equals 1 sector or 512bytes
	mov dx,0x1F0
	rep insw
	pop ecx
	loop .next_sector
	;End of reading sectors into memory
	ret
	
	
	

fat_signature:
	times 510-($-$$) db 0x0
	dw 0xAA55

