#ifndef SYSTEM_H
#define SYSTEM_H

#include<stdint.h>

typedef unsigned long int address;

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void io_wait()
{
    asm volatile("jmp 1f\n\t" "1:jmp 2f\n\t" "2:");
}

#endif