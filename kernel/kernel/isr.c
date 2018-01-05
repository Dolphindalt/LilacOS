#include <kernel/isr.h>

#include <kernel/port.h>
#include <stdio.h>

isr_t interrupt_handlers[256];

void trap(registers_t *regs)
{
    if(regs->interrupt_number >= IRQ_0)
    {
        if(regs->interrupt_number >= 40)
        {
            outb(0xA0, 0x20);
            io_wait();
        }
        outb(0x20, 0x20);
        io_wait();
    }
    if(interrupt_handlers[regs->interrupt_number])
    {
        isr_t handler = interrupt_handlers[regs->interrupt_number];
        handler(regs);
    }
}

void interrupts_init()
{
    for(int i = 0; i <= 0x1f; i++)
        register_interrupt_handler(i, do_nothing);
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

void do_nothing(registers_t * regs)
{
	printf("Detected interrupt: %u\n", regs->interrupt_number);
}