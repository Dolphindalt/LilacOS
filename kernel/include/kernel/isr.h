#ifndef ISR_H
#define ISR_H 1

#include <stdint.h>

typedef struct registers {
    uint32_t ds; // data selector
    uint32_t edi, esi, edp, esp, ebx, edx, ecx, eax; // registers to push
    uint32_t int_no, err_code; // interrupt number and error code
    uint32_t eip, cs, eflags, useresp, ss; // more stuff to push
} registers_t;

#endif