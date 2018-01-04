#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

/*
    To be called in kernel_main. Prepare the terminal for use.
*/
void terminal_initialize();

/*
    Print a single character to the terminal.
*/
void terminal_putchar(char c);

/*
    Write a string of known size to the terminal.
*/
void terminal_write(const char* data, size_t size);

/*
    Write a string to the terminal.
*/
void terminal_writestring(const char* data);

void terminal_writewarning(const char *data);

#endif
