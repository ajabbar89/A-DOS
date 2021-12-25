#ifndef __HEAP_H__
#define __HEAP_H__

#include "common.h"
#include "config.h"

/*
 Each entry of table will have the below bitmapping.
Bits
	0-3 : Entry type
	4-5 : Reserved
	  6 : If the entry corresponds to the start of the block
	  7 : If the entry corresponds to memory which has blocks succeeding it in the allocation
Mapping to byte values,
	0xC1 : Block is taken and its the first block in allocation and there are more blocks succeeding it. //First block of allocation
	0x41 : Block is taken and its the first block in allocation and no more blocks succeeding it.//First & single block of allocation
	0x81 : Block is taken and its not first block in allocation with more blocks following it. //Intermediate block in allocation
	0x01 : Block is taken and its not first block nor are there are blocks after it. //Last block in allocation
	0x00 : Block is free
	0xX0 : Block is free, in case the upper bits were not cleared off when block was released

 */
//Bits 0-4 of heap table entry
#define HEAP_TABLE_BLOCK_TAKEN	0x1
#define HEAP_TABLE_BLOCK_FREE 	0x0		
//Bits 6-7 of heap table entry
#define SINGLE_BLOCK		0x3
#define START_BLOCK		0x1
#define INTERMEDIATE_BLOCK	0x2
#define END_BLOCK		0x0

#define LONE_BLOCK		0x41
#define LAST_BLOCK		0x01
#define INTERIM_BLOCK		0x81
#define FIRST_BLOCK		0xC1

typedef struct {
	uint8_t block_status:4;
	uint8_t reserved:2;
	uint8_t block_type:2; 
}heap_table_t;

typedef struct {
	void *base_address;
	heap_table_t *heap_table;	
}heap;

int heap_create(heap *_heap, void *start_address);
void *amalloc(heap *_heap,size_t bytes);
void afree(heap *_heap,void *ptr);
size_t convert_bytes_to_blocks(size_t bytes);

#endif
