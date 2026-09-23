#include "terminal.h"


size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;


static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void update_cursor()
{
	uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void navigate_arrow_keys(uint8_t scancode)
{
	switch (scancode) {
		case 0x48: // Up arrow
			if (terminal_row > 0) terminal_row--;
			break;
		case 0x50: // Down arrow
			if (terminal_row < VGA_HEIGHT - 1) terminal_row++;
			break;
		case 0x4B: // Left arrow
			if (terminal_column > 0) terminal_column--;
			break;
		case 0x4D: // Right arrow
			if (terminal_column < VGA_WIDTH - 1) terminal_column++;
			break;
	}
	update_cursor();
}

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	update_cursor();
}

void terminal_setcolor(enum vga_color fg, enum vga_color bg) 
{
    terminal_color = vga_entry_color(fg, bg);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) 
{
	if (c == '\n') {
		terminal_column = 0;
		++terminal_row;
	} else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);	
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT)
				terminal_row = 0;
		}
	}
	update_cursor();
}

int terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
	return size;
}

void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}