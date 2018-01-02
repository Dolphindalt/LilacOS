#ifndef PORT_H
#define PORT_H

#include <stdint.h>

#define PORTS(num, instr) \
static inline void port##num##_write(uint##num##_t data, uint16_t port) \
{ \
    asm volatile("out"#instr" %0, %1" : : "a"(data), "Nd"(port)); \
} \
static inline uint##num##_t port##num##_read(uint16_t port) \
{ \
    uint##num##_t result; \
    asm volatile("in"#instr" %1, %0" : "=a"(result) : "Nd"(port)); \
    return result; \
} \

PORTS(8,b)

PORTS(16,w)

PORTS(32,l)

static inline void port8slow_write(uint8_t data, uint16_t port)
{
    asm volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a"(data), "Nd"(port));
}

typedef unsigned long int address;

#endif