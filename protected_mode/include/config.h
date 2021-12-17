#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "common.h"

#define TOTAL_INTERRUPTS 512
#define KERNEL_CODE_SELECTOR 0x8
#define KERNEL_DATA_SELECTOR 0x10

#define VGA_WIDTH 80
#define VGA_HEIGHT 20
#define VIDEO_MEM_ADDR 0xB8000

#endif
