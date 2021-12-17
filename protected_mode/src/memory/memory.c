#include "memory.h"

void *memset(void *ptr, int num, size_t size) {
	uint8_t *p = (uint8_t *)ptr;
	for(int i = 0; i < size; i++)
		p[i] = num;
	return p;
}

size_t strlen(const char *string) {
	size_t sz = 0;
	while(*string++ != '\0')
		sz++;
	return sz;
}

