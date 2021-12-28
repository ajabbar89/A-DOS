#include <io/io.h>
#include <memory/memory.h>

disk_t disk;//To represent the primary HDD

int disk_read_sector(uint32_t lba, int total, void *buf) {
	outb(0x1F6,(lba >> 24) | 0xE0);
	outb(0x1F2,total);
	outb(0x1F3,(unsigned char)(lba & 0xff));
	outb(0x1F4,(unsigned char)(lba >> 8));
	outb(0x1F5,(unsigned char)(lba >> 16));
	outb(0x1F7,0x20);
	
	unsigned short *ptr = (unsigned short *)buf;
	for(int i = 0; i < total; i++) {
		//Wait for the buffer to be ready
		char c = insb(0x1F7);
		while(!(c & 0x08)) {
			c = insb(0x1F7);
		}

		//Copy from HDD to memory
		for(int j = 0; j < 256; j++) {
			*ptr = insw(0x1F0);
			ptr++;
		}
	}
	return 0;
}


void disk_init() {
	disk.type = 0;//0 is supposed to represent the primary HDD
	disk.sector_size = SECTOR_SIZE;
}

disk_t *get_disk(int index) {
	if(index != 0)
		return 0;

	return &disk;
}

int disk_read_block(disk_t *idisk, uint32_t lba, int total, void *buf) {
	if(idisk != &disk)
		return -1;

	return disk_read_sector(lba,total,buf);
}
