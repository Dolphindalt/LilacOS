#include <kernel/isr.h>

#include <stdio.h>

void isr_handler(registers_t registers)
{
    printf("Interrupt: %u\n", registers.int_no);
}