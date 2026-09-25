#include "terminal.h"


uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;
uint16_t terminal_fb[SCREEN_COUNT][VGA_WIDTH * MAX_FB_ROWS];
struct terminal_state terminal_states[4];
bool has_screen_updated = false;
int current_screen = 0;


void set_current_screen(int screen) {
	if (screen >= 0 && screen < SCREEN_COUNT) {
		current_screen = screen;
		has_screen_updated = true;
		update_cursor();
	}
}

void set_screen_update_flag() {
	has_screen_updated = true;
}

char* get_current_screen_buffer() {
	return (char*)terminal_fb[current_screen];
}

char* get_current_line() {
	size_t row = terminal_states[current_screen].row;
	size_t scroll_row = terminal_states[current_screen].scroll_row;
	size_t start_index = (scroll_row + row) * VGA_WIDTH;
	static char line_buffer[VGA_WIDTH + 1]; // +1 for null terminator

	while (start_index < (scroll_row + row + 1) * VGA_WIDTH && start_index < VGA_WIDTH * MAX_FB_ROWS) {
		uint16_t entry = terminal_fb[current_screen][start_index];
		char c = (char)(entry & 0xFF); // Extract the character from the entry
		if (c == '\0' || (unsigned char)c == VGA_DISPLAY_SPACE) {
			break; // Stop at null terminator or space
		}
		line_buffer[start_index - (scroll_row + row) * VGA_WIDTH] = c;
		start_index++;
	}
	line_buffer[start_index - (scroll_row + row) * VGA_WIDTH] = '\0';
	
	return (char*)line_buffer;
}

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

void update_cursor() {
	if (terminal_states[current_screen].column >= VGA_WIDTH)
		terminal_states[current_screen].column = VGA_WIDTH - 1;

	size_t cursor_row = 0;
	if (terminal_states[current_screen].row >= terminal_states[current_screen].scroll_row)
		cursor_row = terminal_states[current_screen].row - terminal_states[current_screen].scroll_row;
	if (cursor_row >= VGA_HEIGHT)
		cursor_row = VGA_HEIGHT - 1;

	uint16_t pos = cursor_row * VGA_WIDTH + terminal_states[current_screen].column;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void update_screen(void) {
	if (!has_screen_updated)
		return;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = (terminal_states[current_screen].scroll_row + y) * VGA_WIDTH + x;
			terminal_buffer[y * VGA_WIDTH + x] = terminal_fb[current_screen][index];
		}
	}

	has_screen_updated = false;
}

void scroll_up_or_down(int direction) {
	bool screen_scrolled = false;

	if (direction > 0) {
		if (terminal_states[current_screen].scroll_row < MAX_FB_ROWS - VGA_HEIGHT
			&& terminal_states[current_screen].row >= terminal_states[current_screen].scroll_row + VGA_HEIGHT - 1) {
			terminal_states[current_screen].scroll_row++;
			screen_scrolled = true;
		}
	} else if (direction < 0) {
		if (terminal_states[current_screen].scroll_row > 0) {
			terminal_states[current_screen].scroll_row--;
			screen_scrolled = true;
		}
	}
	if (screen_scrolled) {
		set_screen_update_flag();
	}

	update_cursor();
}

void navigate_arrow_keys(uint8_t scancode) {
	switch (scancode) {
		case 0x48: // Up arrow
			scroll_up_or_down(-1);
			return;
		case 0x50: // Down arrow
			scroll_up_or_down(1);
			return;
		case 0x4B: // Left arrow
			if (terminal_states[current_screen].column > 0) {
				terminal_states[current_screen].column--;
			}
			break;
		case 0x4D: // Right arrow
			if (terminal_states[current_screen].column < VGA_WIDTH - 1) {
				terminal_states[current_screen].column++;
			}
			break;
	}
	update_cursor();
}

void terminal_scroll() {
	if (terminal_states[current_screen].row >= VGA_HEIGHT) {
		terminal_states[current_screen].scroll_row++;
		if (terminal_states[current_screen].scroll_row >= MAX_FB_ROWS - VGA_HEIGHT) {
			terminal_states[current_screen].scroll_row = 0;
		}
		// Clear the new row that will appear at the bottom
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = (terminal_states[current_screen].scroll_row + VGA_HEIGHT - 1) * VGA_WIDTH + x;
			terminal_fb[current_screen][index] = vga_entry(VGA_DISPLAY_SPACE, terminal_states[current_screen].color);
		}
	}
}

void terminal_initialize_current(void) {
	terminal_states[current_screen].row = 0;
	terminal_states[current_screen].column = 0;
	terminal_states[current_screen].scroll_row = 0;
	terminal_states[current_screen].color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

	for (size_t y = 0; y < MAX_FB_ROWS; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_fb[current_screen][index] = vga_entry(VGA_DISPLAY_SPACE, terminal_states[current_screen].color);
		}
	}
	update_cursor();
}

void terminal_initialize(void) {
	for (int i = 0; i < SCREEN_COUNT; i++) {
		terminal_states[i].row = 0;
		terminal_states[i].column = 0;
		terminal_states[i].scroll_row = 0;
		terminal_states[i].color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	}
	for (size_t y = 0; y < MAX_FB_ROWS; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			for (int i = 0; i < SCREEN_COUNT; i++) {
				terminal_fb[i][index] = vga_entry(VGA_DISPLAY_SPACE, terminal_states[i].color);
			}
		}
	}
	update_cursor();
}

void terminal_setcolor(enum vga_color fg, enum vga_color bg)  {
    terminal_states[current_screen].color = vga_entry_color(fg, bg);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	if (x >= VGA_WIDTH || y >= MAX_FB_ROWS) {
		return;
	}
	const size_t index_in_fb = y * VGA_WIDTH + x;
	if (index_in_fb < VGA_WIDTH * MAX_FB_ROWS) {
		terminal_fb[current_screen][index_in_fb] = vga_entry(c, color);
	}
}

void terminal_newline() {
	terminal_states[current_screen].row++;
	terminal_states[current_screen].column = 0;

	set_screen_update_flag();
	update_cursor();
}

void terminal_backspace() {
	if (terminal_states[current_screen].column > 0) {
		terminal_states[current_screen].column--;
	} else {
		return; // At the top-left corner, nothing to backspace
	}
	const size_t index_in_fb = terminal_states[current_screen].row * VGA_WIDTH + terminal_states[current_screen].column;
	if (index_in_fb < VGA_WIDTH * MAX_FB_ROWS) {
		terminal_fb[current_screen][index_in_fb] = vga_entry(VGA_DISPLAY_SPACE, terminal_states[current_screen].color);
	}
	set_screen_update_flag();
	update_cursor();
}


void terminal_putchar(char c) {
	set_screen_update_flag();

	if (c == '\n') {
		if (++terminal_states[current_screen].row != MAX_FB_ROWS) {
			terminal_states[current_screen].column = 0;
		} else {
			terminal_states[current_screen].row = MAX_FB_ROWS - 1;
		}
		scroll_up_or_down(1);
	}
	else {
		terminal_putentryat(c, terminal_states[current_screen].color, terminal_states[current_screen].column, terminal_states[current_screen].row);
		if (++terminal_states[current_screen].column == VGA_WIDTH) {
			terminal_states[current_screen].column = 0;
			if (++terminal_states[current_screen].row == MAX_FB_ROWS) {
				terminal_states[current_screen].row = 0;
			}
		}
	}
	update_cursor();
}

int terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
	return size;
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}
