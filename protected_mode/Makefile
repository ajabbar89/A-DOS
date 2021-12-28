include sources.mk


#Compilation parameters
INCLUDES = -I./include 
CC = $(BLD_TOOLS_DIR)/i686-elf-gcc
LD = $(BLD_TOOLS_DIR)/i686-elf-ld
AS = nasm
CFLAGS = -Wall -Werror -O0 -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -std=gnu99\
	 -finline-functions -Wno-unused-function -fno-builtin -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Iinc

#Object files
ASM_OBJS = $(ASM_SRCS:.asm=.asm.o)
%.asm.o : %.asm
	$(AS) -f elf -g $< -o $@

C_OBJS = $(C_SRCS:.c=.o)
%.o : %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@ 


ASM_OBJS_BUILD: $(ASM_OBJS)
	mv $(KERNEL_SRC_DIR)/*.asm.o $(IDT_SRC_DIR)/*.asm.o $(IO_SRC_DIR)/*.asm.o  $(BUILD_DIR)

C_OBJS_BUILD: $(C_OBJS)
	mv $(KERNEL_SRC_DIR)/*.o $(IDT_SRC_DIR)/*.o $(MEMORY_SRC_DIR)/*.o $(BUILD_DIR) 

KERNEL_OBJS: ASM_OBJS_BUILD C_OBJS_BUILD

KERNEL_BUILD_FILES = $(BUILD_DIR)/kernel.asm.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/idt.o $(BUILD_DIR)/memory.o $(BUILD_DIR)/idt.asm.o $(BUILD_DIR)/io.asm.o $(BUILD_DIR)/heap.o $(BUILD_DIR)/kheap.o $(BUILD_DIR)/paging.o $(BUILD_DIR)/paging.asm.o


#Final build targets
all: BLR kernel_compile kernel_link os ctags 

BLR:
	$(AS) -f bin $(BLR_SRC) -o $(BLR_OUT)

kernel_compile: KERNEL_OBJS

kernel_link:
	$(LD) -g -relocatable $(KERNEL_BUILD_FILES) -o $(BUILD_DIR)/kernel_final.o
	$(CC) $(CFLAGS) -T ./src/linker.ld -o $(KERNEL_OUT) $(BUILD_DIR)/kernel_final.o

os:
	dd if=$(BLR_OUT) > $(OS_OUT)
	dd if=$(KERNEL_OUT) >> $(OS_OUT)
	dd if=/dev/zero bs=512 count=100 >> $(OS_OUT)

ctags:
	find . -path "./src/*" -or -path "./include/*" > ctags_files_list
	ctags -L ctags_files_list

clean:
	rm -f ./bin/*.bin ./build/*.o ./src/*.asm.o ./src/idt/*.asm.o ./memory/*o ./src/*.o ctags_files_list tags
