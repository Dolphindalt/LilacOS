#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/isr.h>
#include <kernel/port.h>

void kernel_main()
{
	disable_interrupts();
	terminal_initialize();
	descriptor_tables_init();
	interrupts_init();

	enable_interrupts();

	asm("int $0x03");
	asm("int $0x04");

	printf("Hello, kernel World!\n");
	printf("Hello, kernel World!\n");

	while(1);
}
