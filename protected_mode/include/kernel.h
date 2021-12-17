#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "common.h"
#include "idt.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 20
#define VIDEO_MEM_ADDR 0xB8000

void terminal_init(void);
uint16_t char_video_memory(char,char);
void terminal_putchar(int,int,char,char);
void terminal_writechar(char,char);
size_t strlen(const char *);
void print(const char *);
void *memset(void *,int,size_t);
void kernel_main();

#endif
