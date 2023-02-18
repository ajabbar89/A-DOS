#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <common.h>

int strlen(const char *);
void *memset(void *,int,size_t);
int strnlen(const char *, int);
int charToDigit(char);
bool isDigit(char);
int memcmp(void *, void *, int);

#endif
