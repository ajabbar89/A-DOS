#ifndef __IO_H__
#define __IO_H__

#include <common.h>


typedef struct {
	uint32_t type;
	uint32_t sector_size;
}disk_t;

unsigned char insb(unsigned short port); //To read a byte from port
unsigned short insw(unsigned short port); //To read a word from port
void outb(unsigned short port, unsigned char val); //To insert byte into port
void outw(unsigned short port, unsigned short val); //To insert word into port
void diskInit();
disk_t *getDisk(int index);
int diskReadBlock(disk_t *disk, uint32_t lba, int total, void *buf);

#endif
