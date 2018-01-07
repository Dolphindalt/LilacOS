#include <stdio.h>
#include <stdlib.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/isr.h>
#include <kernel/port.h>
#include <kernel/timer.h>
#include <kernel/paging.h>

void kernel_main()
{
	terminal_initialize();
	descriptor_tables_init();
	interrupts_init();
	paging_init();
	timer_init(50);

	printf("Hello World\n");

	unsigned int* a = malloc(8);
	unsigned int* b = malloc(8), c = malloc(8);
	unsigned int* d = malloc(8);
	printf("a: %u b: %u c: %u d: %u\n", a, b, c, d);
	
	free(a);
	free(c);
	free(b);
	free(d);

	printf("Hello World\n");

	while(1);
}
