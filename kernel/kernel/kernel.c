#include <stdio.h>

#include <kernel/tty.h>
#include<kernel/gdt.h>

void kernel_main(void) {
	globalDescriptorTableInit();
	terminal_initialize();
	printf("Hello, kernel World!\n");
}
