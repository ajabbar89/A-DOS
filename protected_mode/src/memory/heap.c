#include <memory/heap.h>
#include <error/error_types.h>
#include <memory/memory.h>
#include <config/config.h>

static bool heap_validate_alignment(void *address) {
	return (((uint32_t)address % PAGE_SIZE) == 0); 
}

/*
 *	@brief Function to create heap by validating the heap start address against alignment of
 *	page size and also zeroing out heap addresses and heap table entries for the entire heap
 *	size and heap table size respectively
 */
int heap_create(heap *_heap, void *start_address) {
	int ret = 0;
	if(!heap_validate_alignment(start_address)) {
		ret = INVALID_ARG;
		goto out;
	}
	//Zeroing out the heap locations
	memset(start_address,0,HEAP_SIZE);
	memset(_heap->heap_table,0,HEAP_TABLE_SIZE);
out:
	return ret;
}

size_t convert_bytes_to_blocks(size_t bytes) {
	size_t extra = ((bytes % PAGE_SIZE) == 0) ? 0 : 1;
	return (bytes/PAGE_SIZE) + extra;
}

/*
 *	@brief Returns the index of the start free block for the set of contiguous free blocks requested 
 *	from the heap table array
 */
uint32_t heap_get_start_block(heap *_heap,size_t blocks) {
	uint32_t start_block = 0;
	uint32_t block_count = 0;
	for(int i = 0; i < HEAP_TABLE_SIZE; i++) {
		if(_heap->heap_table[i].block_status != 0x0) {
			block_count = 0;
			start_block = i+1;
		}
		else
			block_count++;
		if(block_count == blocks)
			return start_block;
	}
	return -1;
}

/*
 *	@brief Return the address of memory from heap which is mapped to the start_block index 
 *	of the heap table array
 */
void *heap_block_to_address(heap *_heap,uint32_t start_block) {
	return _heap->base_address + ((uint32_t)start_block)*((uint32_t)PAGE_SIZE);
}


/*
 *	@brief Function will return the block offset inside the heap table array corresponding to the 
 *	start address in heap allocation specified.
 */
uint32_t heap_address_to_block(heap *_heap, void *address) {
	return ((uint32_t)address - (uint32_t)_heap->base_address)/PAGE_SIZE;
}

/*
 *	@brief Function to mark or allocate the blocks of memory in heap which is to be allocated 
 *	by filling up the table entry data corresponding to each block inside the heap table array
 */
void allocate_blocks(heap *_heap,uint32_t start_block, size_t blocks) {
	heap_table_entry *ptr;
	uint32_t end_block = start_block + blocks - 1;
	for(int i = start_block; i < (start_block + blocks); i++) {
		ptr = (heap_table_entry *)&_heap->heap_table[i];
		if(i == start_block && blocks == 1)
			*ptr = LONE_BLOCK;
		else if(i == start_block && blocks > 1)
			*ptr = FIRST_BLOCK;
		else if(i == end_block && blocks > 1)
			*ptr = LAST_BLOCK;
		else
			*ptr = INTERIM_BLOCK;
	}
}

/*
 *	@brief Function to deallocate blocks. Dellocation will be done from start block till end block by checking the 
 *	entry value
 */
void deallocate_blocks(heap *_heap, uint32_t start_block) {
	heap_table_entry *ptr = (heap_table_entry *)&_heap->heap_table[start_block];
	*ptr = 0x0;
	ptr++;
	while(*ptr != LONE_BLOCK && *ptr != FIRST_BLOCK && *ptr != 0x0 && (uint32_t *)ptr <= (uint32_t *)&(_heap->heap_table[HEAP_TABLE_SIZE-1]))
		*ptr++ = 0x0;
}


void *amalloc(heap *_heap,size_t bytes) {
	void *address = 0;
	size_t blocks = convert_bytes_to_blocks(bytes);
	uint32_t start_block = heap_get_start_block(_heap,blocks);
	if(start_block < 0)
		goto out;
	//Mark the blocks as taken or allocated
	allocate_blocks(_heap,start_block,blocks);
	address = heap_block_to_address(_heap,start_block);	
out:
	return address;
}

void afree(heap *_heap,void *ptr) {
	uint32_t start_block = heap_address_to_block(_heap,ptr);
	deallocate_blocks(_heap,start_block);
}
