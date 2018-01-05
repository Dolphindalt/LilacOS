#include <kernel/gdt.h>

#include <kernel/port.h>
#include <kernel/isr.h>
#include <string.h>

extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);

static void gdt_init();
static void gdt_setgate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

static void idt_init();
static void idt_setgate(uint8_t, uint32_t, uint16_t, uint8_t);

gdt_entry_t gdt_entries[5];
descriptor_ptr_t _gdt_ptr;
idt_entry_t idt_entries[256];
descriptor_ptr_t _idt_ptr;

extern uint32_t vectors[];

void descriptor_tables_init()
{
    gdt_init();
    idt_init();
	memset(&interrupt_handlers, 0, sizeof(isr_t) * 256);
}

static void gdt_init()
{
	_gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
	_gdt_ptr.base = (uint32_t) & gdt_entries;
	gdt_setgate(0, 0, 0, 0, 0);
	gdt_setgate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);	/* code segment */
	gdt_setgate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);	/* data segment */
	gdt_setgate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);	/* user mode code segment */
	gdt_setgate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);	/* user mode data segment */

	gdt_flush((uint32_t) & _gdt_ptr);
}

static void gdt_setgate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
	gdt_entries[num].base_low = base & 0xFFFF;
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low = limit & 0xFFFF;
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;
	gdt_entries[num].granularity |= granularity & 0xF0;
	gdt_entries[num].access = access;
}

void PIC_remap(int master_offset, int slave_offset);

static void idt_init()
{
	int i;
	_idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
	_idt_ptr.base = (uint32_t) & idt_entries;

	memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

	PIC_remap(0x20, 0x28);

	for (i = 0; i < 256; i++)
		idt_setgate(i, vectors[i], 0x08, 0x8E);
	/*system call handler should get set here... */
	idt_flush((uint32_t) & _idt_ptr);
}

static void idt_setgate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags)
{
	idt_entries[num].base_low = base & 0xFFFF;
	idt_entries[num].base_high = (base >> 16) & 0xFFFF;

	idt_entries[num].selector = selector;
	idt_entries[num].reserved = 0;
	idt_entries[num].flags = flags /* @todo "| 0x60" for user mode */ ;
}

void PIC_remap(int master_offset, int slave_offset)
{
    uint8_t a1, a2; // masks
	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);
	outb(PIC1_CMD, ICW1_INIT + ICW1_ICW4); // init
	io_wait();
	outb(PIC2_CMD, ICW1_INIT + ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, master_offset); // who is who
	io_wait();
	outb(PIC2_DATA, slave_offset);
	io_wait();
	outb(PIC1_DATA, 4); // slave loc
	io_wait();
	outb(PIC2_DATA, 2); // slave identity
	io_wait();
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}