#ifndef TERMINAL_H
# define TERMINAL_H

# include <stddef.h>
# include <stdint.h>
# include <stdbool.h>

# include "io.h"
# include "string/string.h"

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000
#define VGA_DISPLAY_SPACE 0xFF

#define MAX_FB_ROWS 1024

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

struct terminal_state {
	size_t row;
	size_t column;
	size_t scroll_row;
	uint8_t color;
};

void set_current_screen(int screen);
void set_screen_update_flag(void);
char* get_current_screen_buffer(void);
char* get_current_line(void);
void update_cursor(void);
void update_screen(void);
void navigate_arrow_keys(uint8_t scancode);
void terminal_initialize_current(void);
void terminal_initialize(void);
void terminal_setcolor(enum vga_color fg, enum vga_color bg);
void terminal_newline(void);
void terminal_backspace(void);
void terminal_putchar(char c);
int terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

#endif // TERMINAL_H
