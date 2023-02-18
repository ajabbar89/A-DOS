#include <memory/paging.h>
#include <kernel/kernel.h>
#include <config/config.h>

static uint32_t *current_directory = 0;
extern void page_directory_load(uint32_t *);

paging_chunk_4gb *create_page_directory(uint8_t flags) {
	uint32_t *directory = (uint32_t *)kmalloc(sizeof(uint32_t)*PAGE_DIRECTORY_ENTRIES);//Allocating 1024 page directory entries where each entry would have the address of the page table
	uint32_t offset = 0;
	for(int i = 0; i < PAGE_DIRECTORY_ENTRIES; i++) {
		uint32_t *entry = (uint32_t *)kmalloc(sizeof(uint32_t)*PAGE_TABLE_ENTRIES);//Allocating 1024 page table entries where each page table entry would map to start of a 4K address range
		for(int j = 0; j < PAGE_TABLE_ENTRIES; j++) {
			entry[j] = (offset + j*PAGE_SIZE) | flags;//Storing the address in terms of PAGE_SIZE increments inside each page table entry
		}
		offset += (PAGE_TABLE_ENTRIES * PAGE_SIZE);
		directory[i] = (uint32_t)entry | flags | PAGE_READ_WRITE;//Storing the address of a page table inside page directory
	}
	paging_chunk_4gb *chunk_4gb = (paging_chunk_4gb *)kmalloc(sizeof(paging_chunk_4gb));
	chunk_4gb->page_directory = directory;
	return chunk_4gb;
}

uint32_t *get_page_directory(paging_chunk_4gb *chunk_4gb) {
	return chunk_4gb->page_directory;
}

void page_directory_switch(uint32_t *page_directory) {
	page_directory_load(page_directory);
	current_directory = page_directory;
}

bool address_aligned(uint32_t *virtual_address) {
	return (*virtual_address % PAGE_SIZE) == 0;
}

int get_page_table_index(void *virtual_address, uint32_t *page_directory_index, uint32_t *page_table_index) {
	if(address_aligned(virtual_address)) {
	*page_directory_index = (uint32_t)virtual_address/(PAGE_SIZE*PAGE_DIRECTORY_ENTRIES);//Basically each directory entry will cover addresses over a range of 4096*1024, ie, each entry has addresses starting from base to base+4096*1024
	*page_table_index = ((uint32_t)virtual_address % (PAGE_SIZE*PAGE_DIRECTORY_ENTRIES))/PAGE_SIZE;
	}
	else 
		return -1;
	return 0;
}

int set_page_table_entry(void *virtual_address, uint32_t *page_directory, uint32_t physical_address) {
	uint32_t page_directory_index = 0;
	uint32_t page_table_index = 0;
	if(address_aligned(virtual_address)) {
		get_page_table_index(virtual_address,&page_directory_index,&page_table_index);
		uint32_t *page_table = (uint32_t *)(page_directory[page_directory_index] & 0xFFFFF000);//Retrieving the page table start location for the virtual address from the page directory
		page_table[page_table_index] = physical_address;//The physical_address here is the 'OR' of the physical address and the flags
	}
	else 
		return -1;
	return 0;
}
