#ifndef GDT_H
#define GDT_H

#include <stdint.h>

/* PIC Commands */
#define PIC1_CMD        0x20
#define PIC1_DATA       0x21
#define PIC2_CMD        0xA0
#define PIC2_DATA       0xA1
#define PIC_READ_IRR    0x0a	/* OCW3 irq ready next CMD read */
#define PIC_READ_ISR    0x0b	/* OCW3 irq service next CMD read */
#define ICW1_ICW4	0x01	/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02	/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04	/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08	/* Level triggered (edge) mode */
#define ICW1_INIT	0x10	/* Initialization - required! */

#define ICW4_8086	0x01	/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02	/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08	/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C	/* Buffered mode/master */
#define ICW4_SFNM	0x10	/* Special fully nested (not) */

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t reserved;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));
typedef struct idt_entry idt_entry_t;

struct descriptor_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
typedef struct descriptor_ptr descriptor_ptr_t;

void descriptor_tables_init();

#endif