#ifndef INTERRUPTS_H
#define INTERRUPTS_H 1

#include <kernel/gdt.h>

#define PIC_MASTER_COMMAND_PORT 0x20
#define PIC_MASTER_DATA_PORT 0x21
#define PIC_SLAVE_COMMAND_PORT 0xA0
#define PIC_SLAVE_DATA_PORT 0xA1

struct GateDescriptor {
    uint16_t handler_addr_low;
    uint16_t gdt_segment;
    uint8_t reserved;
    uint8_t access;
    uint16_t handler_addr_high;
} __attribute__((packed));

struct idtptr {
    uint16_t size;
    uint32_t base;
} __attribute__((packed));

static uint16_t hardware_interrupt_offset;

/*
    Sets interrupts in the interrupt descriptor table, idt.
    Should be called after gdt_init().
*/
void interrupts_init(uint16_t hardware_offset);

/*
    Set an entry in the idt table.
*/
void set_idt_entry(uint8_t interruptNum, uint16_t offset, void (*handler)(), uint8_t access, uint8_t type);

void interrupts_activate();
void interrupts_deactivate();

uint32_t handle_interrupt(uint8_t num, uint32_t esp);

void ignore_interrupt_request();
void handle_interrupt_request0x00();
void handle_interrupt_request0x01();
void handle_interrupt_request0x02();
void handle_interrupt_request0x03();
void handle_interrupt_request0x04();
void handle_interrupt_request0x05();
void handle_interrupt_request0x06();
void handle_interrupt_request0x07();
void handle_interrupt_request0x08();
void handle_interrupt_request0x09();
void handle_interrupt_request0x0A();
void handle_interrupt_request0x0B();
void handle_interrupt_request0x0C();
void handle_interrupt_request0x0D();
void handle_interrupt_request0x0E();
void handle_interrupt_request0x0F();
void handle_interrupt_request0x31();

void handle_exception0x00();
void handle_exception0x01();
void handle_exception0x02();
void handle_exception0x03();
void handle_exception0x04();
void handle_exception0x05();
void handle_exception0x06();
void handle_exception0x07();
void handle_exception0x08();
void handle_exception0x09();
void handle_exception0x0A();
void handle_exception0x0B();
void handle_exception0x0C();
void handle_exception0x0D();
void handle_exception0x0E();
void handle_exception0x0F();
void handle_exception0x10();
void handle_exception0x11();
void handle_exception0x12();
void handle_exception0x13();

#endif