#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>

void kernel_main(void) {
	descriptor_tables_init();
	terminal_initialize();
	printf("Hello, kernel World!\n");

	asm volatile("int $0x3");
	asm volatile("int $0x4");

	/*while(1)
	{
		
	}*/
	return;
}
