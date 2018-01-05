#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include <stdint.h>
#include <stddef.h>

#define EOF (-1)

int u32tostr(char *str, size_t len, uint32_t value, int base);
int s32tostr(char *str, size_t len, int32_t value, unsigned base);
int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);

#endif
