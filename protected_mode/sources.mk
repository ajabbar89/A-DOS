#Directories
BLR_SRC_DIR = ./src/boot
KERNEL_SRC_DIR = ./src/kernel
IDT_SRC_DIR = ./src/idt
MEMORY_SRC_DIR = ./src/memory
IO_SRC_DIR = ./src/io
BIN_DIR = ./bin
BUILD_DIR = ./build
BLD_TOOLS_DIR = ./cross/bin

#asm sources
BLR_SRC = $(BLR_SRC_DIR)/boot.asm
KERNEL_ASM_SRC = $(KERNEL_SRC_DIR)/kernel.asm
IDT_ASM_SRC = $(IDT_SRC_DIR)/idt.asm
IO_ASM_SRC = $(IO_SRC_DIR)/io.asm

#C sources
KERNEL_C_SRC = $(KERNEL_SRC_DIR)/kernel.c
IDT_C_SRC = $(IDT_SRC_DIR)/idt.c
MEMORY_C_SRC = $(MEMORY_SRC_DIR)/memory.c

#Summary of source files
ASM_SRCS = $(KERNEL_ASM_SRC) $(IDT_ASM_SRC)  $(IO_ASM_SRC) #kernel.asm's object file needs to be linked first in binary so that the program entry label _start will be placed at the kernel memory address start. This variable ASM_SRCS is directly passed to the linker while creating the binary
C_SRCS = $(KERNEL_C_SRC) $(IDT_C_SRC) $(MEMORY_C_SRC)

#Output binaries
BLR_OUT = $(BIN_DIR)/boot.bin
KERNEL_OUT = $(BIN_DIR)/kernel.bin
OS_OUT = $(BIN_DIR)/os.bin

