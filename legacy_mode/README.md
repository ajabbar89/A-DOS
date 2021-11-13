# Prerequisites

1.Install NASM assembler(for assembling the bootloader code)
2.Install qemu-system-x86 emulator(for validating the bootloader)
3.Install bless hex editor(for validating the contents of the binary)


# Implementation
Legacy mode operation of x86 processor is basically when processor boots in 16bit mode. Once system is turned ON, 
the BIOS searches for a bootloader from a disk or other boot sources. Here, we intend to use a disk.(A USB formatted 
 as raw drive). But we make sure the disk mimics a FAT32 drive by ensuring the first sector has the signature 
"0x55 0xAA" located at byte offset 511,512.

The bootloader code is written in nasm assembly.

## Files
- **boot.asm** -> A simple bootloader of 512 bytes which will boot system into legacy mode and print "Hello World" to console.
- **read_from_HDD.asm** -> A bootloader which will read the second sector stored in the disk(USB drive) and print it to console.
The second sector will contain the text inside message.txt. So, any content inside that file will go into disk and bootloader
will display it on console.
- **boot_interrupt_handler.asm** -> A simple bootloader which will print "ZOHello World" on console where 'Z' is printed
due to a divide by zero exception artificially generated and 'O' is printed due to interrupt INT 0x1 exercised with a handler 
for it written inside the .asm file. In short, this file does some basic implementation of interrupt handlers.

## Execution
Using "NASM", assemble the required .asm file it into machine code, $Eg: nasm -f bin boot.asm -o boot.bin

A Makefile is present which can be used for the above.

In case if the requirement is to get a bootloader based on boot.asm, provide command **$make simple_boot**. For the bootloader based on 
read_from_HDD.asm, provide command **$make read_from_HDD**. While assembling read_from_HDD.asm using ***make***, it will also place the contents of message.txt and the original 1sector bootloader binary into a single file.

## Results
$make simple_boot => generates boot.bin
$make read_from_HDD => generates read_from_HDD.bin

Flash the required binary into a disk drive, a USB drive for simplicity. Ensure a format of USB is done so that it doesn't have a filesystem on it.

On Linux, to write to USB drive, one can do $sudo dd if=boot.bin of=/dev/sdc

Here sdc is the file handler for the USB drive in a Linux system. The file handler can vary for different systems.


## Testing
The bootloader can be validated in an emulator like "qemu".
To load the bootloader in qemu, run $qemu-system-x86_64 -hda boot.bin 
"hda" here refers to HDD

In order to test it out on real system, connect the disk or USB drive with the binary burned into its first sector. While booting the x86 system, ensure that boot mode is set to "Legacy" and not "UEFI".
