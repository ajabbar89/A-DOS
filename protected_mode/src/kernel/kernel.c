#include "kernel.h"
#include "idt.h"
#include "io.h"
#include "memory.h"

extern void _problem(); //To link the _problem lable defined in kernel.asm to validate interrupt implementation
extern void _setup_PIC(); //To setup the programmable interrupt controller mapping of system peripheral interrupts
extern void enable_interrupts();
extern void disable_interrupts();

uint16_t *video_mem = NULL;
int terminal_row,terminal_col;
/*
 * 	@brief function which will generate the ASCII/colour combination
 * 	encoding which needs to be placed in video memory location 0xB8000
 * 	onwards in text mode to display chars on screen.
 *
 * 	2 bytes will be used up for ASCII/colour combination. Byte0 for the
 * 	ASCII value of character, byte1 for the colour
 */	
uint16_t char_video_memory(char c, char colour) {
	return (uint16_t)(colour << 8) | (uint16_t)(c); //Due to little endian mode of x86 processor, when using uint16_t, the MSB specified in variable will go into higher address.
}

/*
 * 	@brief Function to clear the screen by writing space or ' ' to 
 * 	all the pixels. The width and height are predefined using MACROS.
 * 	The function also sets the video_mem pointer to point to the video
 * 	memory address 0xB8000
 */ 
void terminal_init() {
	video_mem = (uint16_t *)(VIDEO_MEM_ADDR);
	terminal_row = 0;
	terminal_col = 0;
	for(int row = 0; row < VGA_HEIGHT; row++) {
		for(int col = 0; col < VGA_WIDTH; col++) 
			terminal_putchar(row,col,' ',0);
	}
}
/*
 * 	@brief Helper function to write ASCII/colour combination to video memory
 */ 
void terminal_putchar(int row, int col, char c, char colour) {
	video_mem[(row*VGA_WIDTH) + col] = char_video_memory(c,colour);
}

/*
 * 	@brief Helper function to calculate screen cursor position and pass the required char to terminal_putchar routine
 */ 
void terminal_writechar(char c, char colour) {
	if(c == '\n') {
		terminal_row += 1;
		terminal_col = 0;
		return;
	}
	terminal_putchar(terminal_row,terminal_col,c,colour);
	terminal_col += 1;
	if(terminal_col == VGA_WIDTH) {
		terminal_col = 0;
		terminal_row += 1;
		if(terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}


/*
 * 	@brief Function to print a string on terminal
 */ 
void print(const char *str) {
	size_t len = strlen(str);
	size_t i = 0;
	while(i < len)
		terminal_writechar(str[i++],15);
}


void kernel_main() {
/*	char *video_mem = (char *)(0xB8000);//Address in memory mapped to video. Data placed here will be printed on screen in text mode
	video_mem[0] = 'A';
	video_mem[1] = '1'; //The 2nd byte is for color
*/
	terminal_init();
	print("Hello World!\n");
	print("Anwar loves Senia");

	kheap_init();

	disable_interrupts();

	//Initialize Interrupt Descriptor Table
	idt_init();
	
	//Setup the Programmable interrupt controller
	_setup_PIC();

	enable_interrupts();

	//Validating Interrupt implementation
	//_problem();
	
	//outb(0x60,0xff); //This is to test if in/out implementation works. Test it out using GDB and breaking at this line and then checking the EDX and EAX register contents which will be filled with these arguments.
	
	void *ptr = kmalloc(50);
	void *ptr1 = kmalloc(5000);
	kfree(ptr);
	kfree(ptr1);
	void *ptr2 = kmalloc(10000);
	kfree(ptr2);
}
