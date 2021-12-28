#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <common.h>

#define TOTAL_INTERRUPTS 		512
#define KERNEL_CODE_SELECTOR 		0x8
#define KERNEL_DATA_SELECTOR 		0x10

#define VGA_WIDTH 			80
#define VGA_HEIGHT 			20
#define VIDEO_MEM_ADDR 			0xB8000

#define PAGE_SIZE			(4*1024)
#define HEAP_START 			0x1000000
#define HEAP_SIZE 			100*1024*1024 //MB expressed in bytes
#define HEAP_TABLE_SIZE			((HEAP_SIZE)/(PAGE_SIZE))
#define HEAP_TABLE_ADDRESS		0x00007E00

#define SECTOR_SIZE			512

#endif
