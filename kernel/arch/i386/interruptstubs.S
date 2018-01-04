

.set IRQ_BASE, 0x20

.section .text

.extern handle_interrupt


.macro HandleException num
.global handle_exception\num
handle_exception\num:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm


.macro HandleInterruptRequest num
.global handle_interrupt_request\num
handle_interrupt_request\num:
    movb $\num + IRQ_BASE, (interruptnumber)
    pushl $0
    jmp int_bottom
.endm


HandleException 0x00
HandleException 0x01
HandleException 0x02
HandleException 0x03
HandleException 0x04
HandleException 0x05
HandleException 0x06
HandleException 0x07
HandleException 0x08
HandleException 0x09
HandleException 0x0A
HandleException 0x0B
HandleException 0x0C
HandleException 0x0D
HandleException 0x0E
HandleException 0x0F
HandleException 0x10
HandleException 0x11
HandleException 0x12
HandleException 0x13

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x02
HandleInterruptRequest 0x03
HandleInterruptRequest 0x04
HandleInterruptRequest 0x05
HandleInterruptRequest 0x06
HandleInterruptRequest 0x07
HandleInterruptRequest 0x08
HandleInterruptRequest 0x09
HandleInterruptRequest 0x0A
HandleInterruptRequest 0x0B
HandleInterruptRequest 0x0C
HandleInterruptRequest 0x0D
HandleInterruptRequest 0x0E
HandleInterruptRequest 0x0F
HandleInterruptRequest 0x31

int_bottom:

    # save registers
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    # load ring 0 segment register
    #cld
    #mov $0x10, %eax
    #mov %eax, %eds
    #mov %eax, %ees

    # call C Handler
    pushl %esp
    push (interruptnumber)
    call handle_interrupt
    add %esp, 6
    mov %eax, %esp # switch the stack

    # restore registers
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

.global ignore_interrupt_request
ignore_interrupt_request:

    iret


.data
    interruptnumber: .byte 0