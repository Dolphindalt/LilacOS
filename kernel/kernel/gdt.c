#include <kernel/gdt.h>

#include <string.h>

#define gdt(n) gdt_entries[n]
#define idt(n) idt_entries[n]

extern void _gdt_flush(uint32_t); // flush.s
extern void _idt_flush(uint32_t);

static void gdt_init();
static void gdt_setgate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

static void idt_init();
static void idt_setgate(uint8_t, uint32_t, uint16_t, uint8_t);

gdt_entry_t gdt_entries[5];
gdt_ptr_t _gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t _idt_ptr;

void descriptor_tables_init()
{
    gdt_init();
    idt_init();
}

static void gdt_init()
{
    _gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    _gdt_ptr.base = (uint32_t)&gdt_entries;

    gdt_setgate(0, 0, 0, 0, 0); // NULL segment
    gdt_setgate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code
    gdt_setgate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data
    gdt_setgate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code
    gdt_setgate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data

    _gdt_flush((uint32_t)&_gdt_ptr);
}

static void gdt_setgate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt(num).base_low = (base & 0xFFFF);
    gdt(num).base_middle = (base >> 16) & 0xFF;
    gdt(num).base_high = (base >> 24) & 0xFF;

    gdt(num).limit_low = (limit & 0xFFFF);
    gdt(num).granularity = (limit >> 16) & 0x0F;

    gdt(num).granularity |= gran & 0xF0;
    gdt(num).access = access;
}

static void idt_init()
{
    _idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    _idt_ptr.base = (uint32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

    idt_setgate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr1, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr2, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr3, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr4, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr5, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr6, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr7, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr8, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr9, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr10, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr11, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr12, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr13, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr14, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr15, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr16, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr17, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr18, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr19, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr20, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr21, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr22, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr23, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr24, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr25, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr26, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr27, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr28, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr29, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr30, 0x08, 0x8E);
    idt_setgate(0, (uint32_t)isr31, 0x08, 0x8E);
}

static void idt_setgate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags)
{
    idt(num).base_low = base & 0xFFFF;
    idt(num).base_high = (base >> 16) & 0xFFFF;

    idt(num).selector = selector;
    idt(num).reserved = 0;
    // uncomment the or statement when in user mode
    idt(num).flags = flags; /* | 0x60 */ // level 3!
}