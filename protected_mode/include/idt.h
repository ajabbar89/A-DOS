/*
 * 	@brief Header file for Interrupt Descriptor Table
 */ 

#ifndef __IDT_H__
#define __IDT_H__

#include "common.h"
#include "config.h"
#include "kernel.h"

struct idt_desc {
	uint16_t offset_1; //offset bits 0-15
	uint16_t selector; //Selector thats in GDT
	uint8_t zero; //Unused
	uint8_t type_attr; //Descriptor type and attributes
	uint16_t offset_2; //offset bits 16-31
} __attribute__((packed));

struct idtr_desc {
	uint16_t limit; //Size of descriptor table - 1
	uint32_t base; //Base address of the start of the interrupt descriptor table
} __attribute__((packed)); //Location of the IDT is kept inside a register inside the CPU known as the IDTR register which can be loaded/stored using lidt/sidt instructions.

void idt_init();
void idt_set(int,void (*)());
void idt_zero();

#endif

