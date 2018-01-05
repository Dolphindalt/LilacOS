#ifndef PORT_H
#define PORT_H 1

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

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

static inline void io_wait(void)
{
    /* TODO: This is probably fragile. */
    asm volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}

static inline void enable_interrupts(void)
{
	asm volatile ("sti");
}

static inline void disable_interrupts(void)
{
	asm volatile ("cli");
}

static inline void halt(void)
{
	asm volatile ("hlt");
}

typedef unsigned long int address;

#endif