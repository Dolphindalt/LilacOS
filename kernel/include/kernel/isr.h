#ifndef ISR_H
#define ISR_H

#include <stdint.h>

typedef enum {
	PME_13 = 13,
	IRQ_0 = 32,
	IRQ_1 = 33,
	IRQ_2 = 34,
	IRQ_3 = 35,
	IRQ_4 = 36,
	IRQ_5 = 37,
	IRQ_6 = 38,
	IRQ_7 = 39,
	IRQ_8 = 40,
	IRQ_9 = 41,
	IRQ_10 = 42,
	IRQ_11 = 43,
	IRQ_12 = 44,
	IRQ_13 = 45,
	IRQ_14 = 46,
	IRQ_15 = 47,
} irqs;

typedef struct {
    uint32_t edi, esi, ebp, oesp, ebx, edx, ecx, eax;
    uint16_t gs, padding1, fs, padding2, es, padding3, ds, padding4, interrupt_number;
    uint32_t error_code, eip;
    uint16_t cs, padding5;
    uint32_t eflags, esp;
    uint16_t ss, padding6;
} registers_t;

typedef void (*isr_t) (registers_t *);

void interrupts_init();
void register_interrupt_handler(uint8_t n, isr_t handler);
void do_nothing(registers_t * regs);

extern isr_t interrupt_handlers[256];

#endif