#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <common.h>

size_t strlen(const char *);
void *memset(void *,int,size_t);
size_t strnlen(const char *, int);
int todigit(char);
bool isdigit(char);
int memcmp(void *, void *, int);

#endif
