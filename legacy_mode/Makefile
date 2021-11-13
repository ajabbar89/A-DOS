AS=nasm

.PHONY: simple_boot read_from_HDD clean

1secfile = boot
2secfile = read_from_HDD

all: simple_boot read_from_HDD

simple_boot: 
	$(AS) -f bin $(1secfile).asm -o $(1secfile).bin

read_from_HDD: 
	$(AS) -f bin $(2secfile).asm -o $(2secfile).bin
	dd if=message.txt >> $(2secfile).bin
	dd if=/dev/zero bs=512 count=1 >> $(2secfile).bin

clean:
	rm -f *.bin
	
