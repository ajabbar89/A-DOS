#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <common.h>
#include <config/config.h>

void terminal_init(void);
void terminal_putchar(int,int,char,char);
void terminal_writechar(char,char);
void kernel_main();
void print(const char *);
void kheap_init();
void *kmalloc(size_t bytes);
void kfree(void *ptr);
uint16_t char_video_memory(char,char);

#endif
