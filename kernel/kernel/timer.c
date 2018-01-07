#include <kernel/timer.h>

#include <kernel/isr.h>
#include <kernel/port.h>

uint32_t tick = 0;

static void timer_callback(registers_t *reg)
{
    tick++;
}

void timer_init(uint32_t freq)
{
    register_interrupt_handler(IRQ_0, &timer_callback);

    // input clock / freq
    uint32_t divisor = 1193180 / freq;

    outb(0x43, 0x36);

    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

    outb(0x40, l);
    outb(0x40, h);
}