#include <kernel/paging.h>

#include <kernel/paging.h>
#include <kernel/isr.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void set_frame(uint32_t frame_address);
static void clear_frame(uint32_t frame_address);
static uint32_t first_frame();

uint32_t *frames;
uint32_t nframes;

extern heap_t *kheap;
extern uint32_t placement_address;

page_directory_t *kernel_directory = NULL, *current_directory = NULL;

void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
    if(page->frame)
        return;
    uint32_t idx = first_frame();
    if(idx == (uint32_t)-1)
        panic("No free frames!");
    set_frame(idx * 0x1000);
    page->present = 1;
    page->rw = is_writeable ? 1 : 0;
    page->user = is_kernel ? 0 : 1;
    page->frame = idx;
}

void free_frame(page_t *page)
{
    uint32_t frame;
    if(!(frame = page->frame)) // page has no frame
        return;
    else
    {
        clear_frame(frame);
        page->frame = 0x0;
    }
}

static void set_frame(uint32_t frame_address)
{
    uint32_t frame = frame_address / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

static void clear_frame(uint32_t frame_address)
{
    uint32_t frame = frame_address/0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

static uint32_t first_frame()
{
    uint32_t i;
    for(i = 0; i < INDEX_FROM_BIT(nframes); i++)
    {
        if(frames[i] != 0xFFFFFFFF) // nothing available
        {
            uint32_t j;
            for(j = 0; j < 32; j++)
            {
                uint32_t to_test = 0x1 << j;
                if(!(frames[i] & to_test))
                    return i * 4 * 8 + j;
            }
        }
    }
    return 0;
}

void paging_init()
{
    uint32_t mem_end_page = 0x1000000; // we only have 16MiB
    nframes = mem_end_page / 0x1000;
    frames = malloc_a(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));

    kernel_directory = malloc_a(sizeof(*kernel_directory));
    memset(kernel_directory, 0, sizeof(*kernel_directory));
    current_directory = kernel_directory;

    size_t i;
    for(i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
        paging_getpage(i, 1, kernel_directory);

    i = 0;
    while(i < 0x1000000)
    {
        alloc_frame(paging_getpage(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }

    for(i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
        alloc_frame(paging_getpage(i, 1, kernel_directory), 0, 0);

    register_interrupt_handler(14, paging_pagefault);
    paging_switchdirectory(kernel_directory);
    kheap = create_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
}

void paging_switchdirectory(page_directory_t *dir)
{
    current_directory = dir;
	asm volatile ("mov %0, %%cr3"::"r" (&dir->tables_physical));
	uint32_t cr0;
	asm volatile ("mov %%cr0, %0":"=r" (cr0));
	cr0 |= 0x80000000;
    asm volatile ("mov %0, %%cr0"::"r" (cr0));
}

page_t *paging_getpage(uint32_t address, int make, page_directory_t *dir)
{
    uint32_t table_idx;
	address /= 0x1000;	/* turn address into index */
	/* find the page table containing this address */
	table_idx = address / 1024;
	if (dir->tables[table_idx]) {
		return &dir->tables[table_idx]->pages[address % 1024];
	} else if (make) {
		uint32_t tmp;
		dir->tables[table_idx] = malloc_ap(sizeof(page_table_t), &tmp);
		memset(dir->tables[table_idx], 0, sizeof(page_table_t));
		dir->tables_physical[table_idx] = tmp | 0x7;	/*present, rw, user */
		return &dir->tables[table_idx]->pages[address % 1024];
	}
    return 0;
}

void paging_pagefault(registers_t *regs)
{
    uint32_t fault_address;
    asm volatile("mov %%cr2, %0" : "=r" (fault_address));
    int present   = !(regs->error_code & 0x1);// Page not present
    int rw = regs->error_code & 0x2;           // Write operation?
    int us = regs->error_code & 0x4;           // Processor was in user-mode?
    int reserved = regs->error_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = regs->error_code & 0x10;          // Caused by an instruction fetch?

    printf("page fault detected: ( ");
    if(present) printf("present ");
    if(rw) printf("read-only ");
    if(us) printf("usermode ");
    if(reserved) printf("reserved ");
    if(id) printf("fetch ");
    printf(") at %d\n", fault_address);
    printf("error %s : %u\n", __FILE__, __LINE__);
}