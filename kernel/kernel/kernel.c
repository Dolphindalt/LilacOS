#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();

	printf("Hello, kernel World!\n");

	int i = 10;
	while(--i > 0)
	{
		terminal_writewarning("Hello\n");
	}
}
