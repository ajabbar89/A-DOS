#ifndef __PAGING_H__
#define __PAGING_H__

#include "config.h"
#include "common.h"

#define PAGE_TABLE_ENTRIES		1024U
#define PAGE_DIRECTORY_ENTRIES		1024U

#define PAGE_PRESENT			0b00000001
#define PAGE_READ_WRITE			0b00000010
#define PAGE_USER_SUPERVISOR		0b00000100
#define PAGE_WRITE_THROUGH		0b00001000
#define PAGE_CACHE_DISABLE		0b00010000
#define PAGE_ACCESSED			0b00100000
#define PAGE_DIRTY			0b01000000
#define PAGE_ATTRIBUTE			0b10000000

typedef struct {
	uint32_t present:1;
	uint32_t read_write:1;
	uint32_t user_supervisor:1;
	uint32_t page_write_through:1;
	uint32_t page_cache_disable:1;
	uint32_t accessed:1;
	uint32_t dirty:1;
	uint32_t page_attribute:1;
	uint32_t global:1;
	uint32_t available:3;
	uint32_t address:20;
}page_table_t;

typedef struct {
	uint32_t present:1;
	uint32_t read_write:1;
	uint32_t user_supervisor:1;
	uint32_t page_write_through:1;
	uint32_t page_cache_disable:1;
	uint32_t accessed:1;
	uint32_t dirty:1;
	uint32_t page_size:1;
	uint32_t available:4;
	uint32_t address:20;
}page_directory_t;

typedef struct {
	uint32_t *page_directory;
}paging_chunk_4gb;

paging_chunk_4gb *create_page_directory(uint8_t flags);
uint32_t *get_page_directory(paging_chunk_4gb *chunk_4gb);
void page_directory_switch(uint32_t *page_directory);
void enable_paging();

#endif
