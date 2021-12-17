#include "kernel.h"

void *memset(void *ptr, int num, size_t size) {
	uint8_t *p = (uint8_t *)ptr;
	for(int i = 0; i < size; i++)
		p[i] = num;
	return p;
}
