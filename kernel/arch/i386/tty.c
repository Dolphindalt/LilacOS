#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/port.h>
#include <kernel/ssp.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_updatecursor()
{
    unsigned temp;
    temp = terminal_column + VGA_WIDTH * terminal_row;
    port16_write(14, 0x3D4);
    port16_write(temp >> 8, 0x3D5);
    port16_write(15, 0x3D4);
    port16_write(temp, 0x3D5);
}

void terminal_clearline(uint8_t from, uint8_t to)
{
    uint16_t i = VGA_WIDTH * from;
    for(; i < (VGA_WIDTH * (to+1)); i++)
        terminal_buffer[i] = 0x0;
}

void terminal_scrollup(uint8_t line)
{
    uint16_t i;
    for(i = 0; i < (VGA_WIDTH * VGA_HEIGHT)-1; i++)
        terminal_buffer[i] = terminal_buffer[i + VGA_WIDTH * line];
    terminal_clearline(VGA_HEIGHT-1-line, VGA_HEIGHT-1);
    if((((signed)terminal_column) - line) < 0)
    {
        terminal_column = 0;
        terminal_row = 0;
    }
    else terminal_column -= line;
}

void terminal_checknewline()
{
    if(terminal_row >= VGA_HEIGHT-1) terminal_scrollup(1);
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	switch(c)
    {
        case '\n': terminal_row++; terminal_column = -1; break;
        default: 
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(c, color); 
            break;
        }
    }
    terminal_checknewline();
}

void terminal_putchar(char c) {
	unsigned char uc = c;
	terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
	terminal_updatecursor();
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}
