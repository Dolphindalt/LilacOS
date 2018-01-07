#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <kernel/isr.h>

#define KHEAP_START 0xC0000000
#define KHEAP_INITIAL_SIZE 0x100000
#define HEAP_INDEX_SIZE 0x20000
#define HEAP_MAGIC 0x123890AB
#define HEAP_MIN_SIZE 0x70000

typedef struct page {
    uint32_t present : 1; // page in memory
    uint32_t rw : 1; // read only, read write if set
    uint32_t user : 1; // supervisor level only if clear
    uint32_t accessed : 1; // accessed since last refresh?
    uint32_t dirty : 1; // written to since last refrsh?
    uint32_t unused : 7; // reserved or unused
    uint32_t frame : 20; // frame address shift 12 right
} page_t;

typedef struct page_table {
    page_t pages[1024];
} page_table_t;

typedef struct page_directory {
    page_table_t *tables[1024];
    uint32_t tables_physical[1024];
    uint32_t physical_address;
} page_directory_t;

void paging_init();
void paging_switchdirectory(page_directory_t *n);
page_t *paging_getpage(uint32_t address, int make, page_directory_t *dir);
void paging_pagefault(registers_t *regs);
void alloc_frame(page_t *page, int is_kernel, int is_writeable);
void free_frame(page_t *page);

#endif