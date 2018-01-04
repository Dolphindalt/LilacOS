#include <kernel/interrupts.h>

#include <kernel/port.h>
#include <stdio.h>

#define IDT_DESC_PRESENT 0x80

struct GateDescriptor idt[256]; // literal idt representation

void interrupts_init(uint16_t hardware_offset)
{
    hardware_interrupt_offset = hardware_offset;

    uint32_t code_segment = gdt_get_segment(1);
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for(uint16_t i = 0; i < 256; i++)
        set_idt_entry(i, code_segment, &ignore_interrupt_request, 0, IDT_INTERRUPT_GATE);

    set_idt_entry(0x00, code_segment, &handle_exception0x00, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x01, code_segment, &handle_exception0x01, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x02, code_segment, &handle_exception0x02, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x03, code_segment, &handle_exception0x03, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x04, code_segment, &handle_exception0x04, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x05, code_segment, &handle_exception0x05, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x06, code_segment, &handle_exception0x06, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x07, code_segment, &handle_exception0x07, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x08, code_segment, &handle_exception0x08, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x09, code_segment, &handle_exception0x09, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x0A, code_segment, &handle_exception0x0A, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x0B, code_segment, &handle_exception0x0B, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x0C, code_segment, &handle_exception0x0C, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x0D, code_segment, &handle_exception0x0D, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x0E, code_segment, &handle_exception0x0E, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x0F, code_segment, &handle_exception0x0F, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x10, code_segment, &handle_exception0x10, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x11, code_segment, &handle_exception0x11, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x12, code_segment, &handle_exception0x12, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x13, code_segment, &handle_exception0x13, 0, IDT_INTERRUPT_GATE);

    set_idt_entry(hardware_offset + 0x00, code_segment, &handle_interrupt_request0x00, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x01, code_segment, &handle_interrupt_request0x01, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x02, code_segment, &handle_interrupt_request0x02, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x03, code_segment, &handle_interrupt_request0x03, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x04, code_segment, &handle_interrupt_request0x04, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x05, code_segment, &handle_interrupt_request0x05, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x06, code_segment, &handle_interrupt_request0x06, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x07, code_segment, &handle_interrupt_request0x07, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x08, code_segment, &handle_interrupt_request0x08, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x09, code_segment, &handle_interrupt_request0x09, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x0A, code_segment, &handle_interrupt_request0x0A, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x0B, code_segment, &handle_interrupt_request0x0B, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x0C, code_segment, &handle_interrupt_request0x0C, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x0D, code_segment, &handle_interrupt_request0x0D, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x0E, code_segment, &handle_interrupt_request0x0E, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(hardware_offset + 0x0F, code_segment, &handle_interrupt_request0x0F, 0, IDT_INTERRUPT_GATE);

    port8slow_write(0x11, PIC_MASTER_COMMAND_PORT); // accept interrupts
    port8slow_write(0x11, PIC_SLAVE_COMMAND_PORT);
    
    port8slow_write(hardware_offset, PIC_MASTER_DATA_PORT);
    port8slow_write(hardware_offset + 8, PIC_SLAVE_DATA_PORT);

    port8slow_write(0x04, PIC_MASTER_DATA_PORT); // master
    port8slow_write(0x02, PIC_SLAVE_DATA_PORT); // slave

    port8slow_write(0x01, PIC_MASTER_DATA_PORT);
    port8slow_write(0x01, PIC_SLAVE_DATA_PORT); 

    port8slow_write(0x00, PIC_MASTER_DATA_PORT);
    port8slow_write(0x00, PIC_SLAVE_DATA_PORT); 

    struct idtptr _ip;
    _ip.size = 256 * sizeof(struct GateDescriptor) - 1;
    _ip.base = (address)&idt;
    asm volatile("lidt %0" : : "m"(_ip));
}

void set_idt_entry(uint8_t interruptNum, uint16_t offset, void (*handler)(), uint8_t access, uint8_t type)
{
    idt[interruptNum].handler_addr_low = ((uint32_t)handler) & 0xFFFF;
    idt[interruptNum].handler_addr_high = (((uint32_t)handler) >> 16) & 0xFFFF;
    idt[interruptNum].gdt_segment = offset;
    idt[interruptNum].access = IDT_DESC_PRESENT | type | ((access & 3) << 5);
    idt[interruptNum].reserved = 0;
}

void interrupts_activate()
{
    asm("sti"); // start interrupts
}

void interrupts_deactivate()
{
    asm("cli");
}

uint32_t handle_interrupt(uint8_t num, uint32_t esp)
{
    printf("INTERRUPT\n");
    return esp;
}

void ignore_interrupt_request();
void handle_interrupt_request0x00();
void handle_interrupt_request0x01();