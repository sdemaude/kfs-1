#include "keyboard.h"


static const char keyboard_map[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',
   '9', '0', '-', '=', '\b',
   '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o',
   'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
   ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
   '.', '/', 0,
   '*', 0, ' '
};

char scancode_to_ascii(uint8_t scancode) {
    if (scancode >= sizeof(keyboard_map))
        return 0;

    return keyboard_map[scancode];
}

void keyboard_handler(void) {
    // Check if keyboard data is available
    if (!(inb(0x64) & 1)) {
        return;
    }

    uint8_t scancode = inb(0x60);

	// Check if the key is released (scancode with the highest bit set)
    if (scancode & 0x80) {
        return;
    }

	// Check if one of the arrow keys is pressed
	if (scancode == 0x48 || scancode == 0x50 || scancode == 0x4B || scancode == 0x4D) {
		navigate_arrow_keys(scancode);
		return;
	}

	// Check if one of the F1-F4 keys is pressed
	if (scancode == 0x3B || scancode == 0x3C || scancode == 0x3D || scancode == 0x3E) {
		switch (scancode) {
			case 0x3B: // F1
				set_current_screen(0);
				break;
			case 0x3C: // F2
				set_current_screen(1);
				break;
			case 0x3D: // F3
				set_current_screen(2);
				break;
			case 0x3E: // F4
				set_current_screen(3);
				break;
		}
		set_screen_update_flag();
		update_cursor();
		return;
	}

	// Check if the backspace key is pressed
	if (scancode == 0x0E) {
		terminal_backspace();
		return;
	}

	// Check if the enter key is pressed
	if (scancode == 0x1C) {
		char* current_line = get_current_line();
		terminal_newline();
		parse_command(current_line);
		return;
	}

    char c = scancode_to_ascii(scancode);

    if (c) {
        terminal_putchar(c);
    }
}