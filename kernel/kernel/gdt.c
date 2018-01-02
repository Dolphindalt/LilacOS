#include <kernel/gdt.h>

#include <kernel/port.h>

struct gdt_entry gdt[4]; // literal gdt
struct gdt_ptr _gp;

void gdt_set_segment(int n, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran)
{
    gdt[n].base_low = (base & 0xFFFF);
    gdt[n].base_middle = (base >> 16) & 0xFF;
    gdt[n].base_high = (base >> 24) & 0xFF;
    gdt[n].limit_low = (limit & 0xFFFF);
    gdt[n].granularity = ((limit >> 16) & 0x0F);
    gdt[n].granularity |= (gran & 0xF0);
    gdt[n].access = access;
}

void gdt_init()
{
    _gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
    _gp.base = (address)&gdt;

    gdt_set_segment(0, 0, 0, 0, 0); // NULL
    gdt_set_segment(0, 0, 0, 0, 0); // UNUSED
    gdt_set_segment(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // CODE
    gdt_set_segment(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // DATA

    _gdt_flush();
}