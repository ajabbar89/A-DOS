#include "heap.h"
#include "common.h"
#include "kernel.h"
#include "error_types.h"

heap *kernel_heap;

void kheap_init() {
	kernel_heap->base_address = (void *)HEAP_START;

	kernel_heap->heap_table = (heap_table_t *)HEAP_TABLE_ADDRESS;	
	int ret = heap_create(kernel_heap,kernel_heap->base_address);
	if(ret == INVALID_ARG) 
		print("Failed to create heap. Unaligned memory location provided as start address of heap\n");
		
}

void *kmalloc(size_t bytes) {
	return amalloc(kernel_heap,bytes);
}

void kfree(void *ptr) {
	afree(kernel_heap,ptr);
}
