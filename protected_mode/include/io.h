#ifndef __IO_H__
#define __IO_H__

#include "common.h"
#include "config.h"

unsigned char insb(unsigned short port); //To read a byte from port
unsigned short insw(unsigned short port); //To read a word from port
void outb(unsigned short port, unsigned char val); //To insert byte into port
void outw(unsigned short port, unsigned short val); //To insert word into port
int disk_read_sector(int lba, int total, void *buf);

#endif
