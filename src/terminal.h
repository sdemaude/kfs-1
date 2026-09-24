#ifndef TERMINAL_H
# define TERMINAL_H

# include <stddef.h>
# include <stdint.h>
# include <stdbool.h>
# include "io.h"

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

#define MAX_FB_ROWS 75

#define SCREEN_COUNT 4

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void update_cursor(void);
void update_screen(void);
void navigate_arrow_keys(uint8_t scancode);
void terminal_initialize(void);
void terminal_setcolor(enum vga_color fg, enum vga_color bg);
void terminal_backspace(void);
void terminal_putchar(char c);
int terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

extern bool has_screen_updated;
extern int current_screen;


#endif // TERMINAL_H
