#ifndef GDT_H
#define GDT_H 1

#include <stdint.h>

/*
    Representation of a segment in a segment register.
    Attribute packed stops the compiler from optmizing the memory.
*/
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

/*
    The limit is the max amount of bytes -1 to be taken up by the GDT.
*/
struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

/*
    Invoked in boot.S to reload segment registers.
*/
extern void _gdt_flush();

/*
    Set the data in a segment register to be flushed to the GDT table.
*/
void gdt_set_segment(int n, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran);

/*
    Intended to be called by kernel_main. Sets up GDT table and segment registers.
*/
void gdt_init();

#endif