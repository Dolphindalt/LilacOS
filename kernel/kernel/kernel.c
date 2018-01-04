#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/interrupts.h>

void kernel_main(void) {
	terminal_initialize();
	gdt_init();
	interrupts_init(0x20);

	

	interrupts_activate();

	printf("Hello, kernel World!\n");
	printf("Hello, kernel World!\n");
}
