#include "idt.h"
#include "memory.h"
#include "kernel.h"
#include "io.h"

struct idt_desc idt_descriptors[TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;


extern void idt_load(void *ptr);
extern void generic_irq_handler_wrapper(); //Wrapper for handling IRQs for which interrupt handler is not programmed. Else there will be a processor fault exception
extern void irq21_handler_wrapper(); //Wrapper for IRQ21 handler function so that there is an iret present while handling the interrupt. 
/*
 * @brief Meant for those IRQs for which a handler is not programmed to avoid processor faults due to no handler present.
 */
void generic_irq_handler() {
	outb(0x20,0x20);//Simply acknowledging the interrupt by passing 0x20 to port 0x20 which is the Master PIC
}

/*
 * @brief interrupt handler for interrupt 0 which shall be the divide by 0 interrupt
 */ 
void idt_zero() {
	print("Divide by zero Error\n");
}

/*
 * @brief Interrupt handler for IRQ21. IRQ20 is the first system peripheral interrupt mapping to the timer. So, IRQ21 will be the keyboard interrupt
 */
void irq21_handler() {
	print("Key press\n");
	outb(0x20,0x20);
}

void idt_set(int interrupt_number, void (*address)()) {
	struct idt_desc *desc = &idt_descriptors[interrupt_number];
	desc->offset_1 = (uint32_t)address & 0xffff;
	desc->selector = KERNEL_CODE_SELECTOR;
        desc->zero = 0;
	desc->type_attr = 0xEE;
	desc->offset_2 = (uint32_t)address >> 16;
}

void idt_init() {
	memset(idt_descriptors,0,sizeof(idt_descriptors));
	idtr_descriptor.limit = sizeof(idt_descriptors) - 1;	  
	idtr_descriptor.base = (uint32_t)idt_descriptors;

	for(int i = 0; i < TOTAL_INTERRUPTS; i++)
		idt_set(i,generic_irq_handler_wrapper);

	idt_set(0,idt_zero);
	idt_set(0x21,irq21_handler_wrapper);

	//Loading the IDT desriptor table into IDTR register after creating
	//all the handlers
	idt_load(&idtr_descriptor);
}
