.global _gdt_flush
_gdt_flush:
    mov 4(%esp), %eax # load gdt_ptr_t passed as special pointer parameter
    lgdt (%eax)
    mov $0x10, %ax # offset in gdt to data segments
    mov %ax, %ds # load data segment selectors
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
    jmp $0x08, $flush # long jump
flush:
    ret

.global _idt_flush
_idt_flush:
    mov 4(%esp), %eax # load idt_ptr_t passed as special pointer parameter
    lidt (%eax)
    ret