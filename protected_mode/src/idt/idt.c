#include "idt.h"

struct idt_desc idt_descriptors[TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;


extern void idt_load(void *ptr);

/*
 * @brief interrupt handler for interrupt 0 which shall be the divide by 0 interrupt
 */ 
void idt_zero() {
	print("Divide by zero Error\n");
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

	idt_set(0,idt_zero);

	//Loading the IDT desriptor table into IDTR register after creating
	//all the handlers
	idt_load(&idtr_descriptor);
}
