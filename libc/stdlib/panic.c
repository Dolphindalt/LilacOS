#include <stdlib.h>
#include <stdio.h>

void panic(const char *str)
{
    printf("%s", str);
    while(1);
}

void panic_assert(const char *file, int32_t line, const char *desc)
{
    asm("cli");
    printf("assert fail: %s at %s : %d\n", desc, file, line);
    while(1);
}