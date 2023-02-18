#include <memory/memory.h>

void *memset(void *ptr, int num, size_t size) {
	uint8_t *p = (uint8_t *)ptr;
	for(int i = 0; i < size; i++)
		p[i] = num;
	return p;
}

int strlen(const char *string) {
	size_t sz = 0;
	while(*string++ != '\0')
		sz++;
	return sz;
}

int strnlen(const char *string, int length) {
	size_t stringLength = 0;
	for (int i = 0; i < length; i++) {
		if(string[i] == '\0')
			break;
		stringLength++;
	}
	return stringLength;
}

int charToDigit(char c) {
	return c-0x30;  // To return the digit value for ASCII representation of a char.
}

bool isDigit(char c) {
	return ((c >= 0x30) && (c <= 0x39)); 
}

int memcmp(void *src, void *pattern, int length) {
	for(int i = 0; i < length; i++) {
		if (((char *)src)[i] != ((char *)pattern)[i])
			return -1;
	}
	return 0;
}
