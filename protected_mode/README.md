# Protected Mode
In protected boot mode for x86 processors, 32-bit ISA is used. So, effectively 4GB of memory address space available.
In addition other features like memory protection is available compared to legacy 16-bit mode. In this project,
we enter protected mode, by forcing system to boot in legacy mode(non-UEFI) BIOS option. This will force the 
processor to start out with legacy 16-bit execution, but the bootloader we have written in the project located at 
***./src/boot*** will force a 32-bit or protected mode transition. The bootloader handles the loading of the kernel 
code from disk into memory.

But in protected mode, BIOS interrupts like INT 0x13(to read from HDD) are unavailable. So, to read from HDD, we need to 
write our own code. This code can be written in C or .asm. We have driver written in .asm for this inside boot.asm. This driver 
is written to the kernel code into predefined memory location. Right now kernel start is assumed as 0x100000 location.

## Compilation
For the .asm files the project uses the "nasm" assembler since the assembly language format used is of nasm format or dialect.
For the .c files, a cross compiler is used. The cross compiler and the associated build tools are taken from GCC official 
code and placed in ***./cross*** folder. A shell script ***setup_cross_compiler.sh*** is present inside the folder does the setup 
of building the code for the build tools. The script needs couple of edits including the path of the ***cross*** folder housing the 
toolchain, the GNU binutils path which will be used to setup the build tools and the GCC compiler path. Before running the shell script though, the system needs to have all the needed dependencies installed specified here, https://wiki.osdev.org/GCC_Cross-Compiler . Then, the binutils or utilities needed for the build process needs to be downloaded from here https://ftp.gnu.org/gnu/binutils/. The GCC compiler can be obtained from here https://ftp.gnu.org/gnu/gcc/. And after editing the shell script with the right paths, it can be run to setup the build system.

The respective binaries needed for compiling and linking are then specified for building the code inside the ***Makefile***.

To compile the project run ***$make all***

The raw kernel binary, inclusive of the kernel source code and a basic kernel startup code kernel.asm recognized by linker script 
as the start of the kernel binary ***kernel.bin*** is located inside ***./bin*** folder. 

The final operating system binary is ***OS.bin*** which is the ***boot.bin*** + ***kernel.bin*** with the kernel.bin appended
to the boot.bin file.
Intermediate object files ****.o*** is stored inside the ***./build*** folder.


### Testing
Use qemu to test the kernel code.
***$qemu-system-i386 -hda ./bin/os.bin***

### GDB usage for debugging

During dev, the correctness of registers can be checked using gdb by linking qemu with gdb as a remote target for gdb,
***$target remote | qemu-system-i386 -hda ./bin/os.bin -S -gdb stdio***

To see the registers
***$info registers***

The symbols need to be loaded explicitly in gdb
***$add-symbol-file ./build/kernel_final.o 0x100000*** Here 0x100000 is the start address of the kernel
The reason to do this step instead of directly running binary os.bin on gdb is because the binary cannot be executed 
on our host system. It won't work. Instead we execute it using qemu as we are simulating system boot into os.bin. Hence, we need 
to add-symbol-file separately and then hook up gdb with remote target of qemu.

For .asm code, we could get the layout of gdb to be showing disassembled code, by ***$layout asm***

Pressing continue after hooking qemu on gdb remotely, will do program execution. So, any breakpoints needed can be set before 
trying "continue" inside gdb.

### Boot flow
The ./src/boot/boot.asm is the BLR code which sets up the protected mode settings. The kernel will be present from 2nd sector onwards.
The bootloader code flow starts out by ensuring that BPB data is provided to the BIOS and then it provides $jmp instruction
to BIOS to jump to the loader code offseted from segment location 0x7C0 as $jmp ***_start:0x7C0***. This section of BLR will be in 
16 bit mode. To enter into protected or 32-bit mode, the GDT needs to be loaded and after that the 16bit code will jump to
CODE segment in 32-bit mode. In 32bit mode Code segment, there is an assembly driver for loading the disk contents(kernel) into
memory location starting 0x100000. After the load, jmp is done to 0x100000, where the kernel code will be loacted. The kernel.asm file 
will have the label ***_start*** which will be the entry point of the machine code that will be generated as kernel machine code. 
This is specified in the linker.ld linker script. Further the kernel.asm will call kernel_main which is function inside kernel C 
source code and execution will convene.


### Things to consider in linking
linker.ld will be the linker script file responsible for defining the layout of the kernel code in memory. The kernel code 
is expected to start out from memory location 0x10000. The linker.ld has an .asm section defined so that
the asm code will be separated from C code in memory so as not to mess up the C-code's alignment in memory. This .asm section 
is therefore placed the last in the "SECTION" layout of the linker script. But a key point to be noted is that while 
linking the files needed for the kernel.bin, the kernel_final.o must be linked first so that it will get loaded into memory at 
0x100000 because section wise, .asm files will be at the last in binary. But in the kernel.asm, since we have not specified any section 
explicitly it will be treated as .text. So when its getting linked with other C code to generate kernel.bin, we must ensure
that its the first one to be linked so that at 0x100000 when the driver in boot.asm which reads from disk to load kernel to memory,
will in fact load kernel.asm related code first.

### Programmable Interrupt Controller(PIC)
The PIC allows us to map the system supported IRQs to IRQ#(these are not system exceptions but interrupts from basic peripherals). 
As an example, the first IRQ for processor is a timer interrupt. Typically 8-15# is meant for system exceptions in protected mode. 
In the project we will start IRQ# from IRQ20. So, IRQ20 is effectively the timer interrupt and IRQ21 will be the keyboard interrupt. 
For this, the PIC needs to be made aware that the IRQs start from 20. This is done in asm code in kernel.asm file before kernel_main 
call.
The setup of PIC is done in kernel_main after loading the IDTR with Interrupt descriptor table(effectively the function pointers to 
exception and interrupt handlers)

### Heap implementation
The heap memory shall start at 0x1000000. 100MB is the target heap size. A heap mapping table will be created which will therefore take 
up 100\*1024\*1024/4096 = 25600 bytes. The 4096 is the 4K block size of a heap entry.
Each entry of table will have the below bitmapping.
Bits
	0-3 : Entry type
	4-5 : Reserved
	  6 : If the entry corresponds to the start of the block
	  7 : If the entry corresponds to memory which has blocks succeeding it in the allocation
Mapping to byte values,
	0xC1 : Block is taken and its the first block in allocation and there are more blocks succeeding it. //First block of allocation
	0x41 : Block is taken and its the first block in allocation and no more blocks succeeding it.//First & single block of allocation
	0x81 : Block is taken and its not first block in allocation with more blocks following it. //Intermediate block in allocation
	0x01 : Block is taken and its not first block nor are there are blocks after it. //Last block in allocation
	0x00 : Block is free
	0xX0 : Block is free, in case the upper bits were not cleared off when block was released


