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

char scancode_to_ascii(uint8_t scancode)
{
    if (scancode >= sizeof(keyboard_map))
        return 0;

    return keyboard_map[scancode];
}

void keyboard_handler(void)
{
    // Check if keyboard data is available
    if (!(inb(0x64) & 1)) {
        return;
    }

    uint8_t scancode = inb(0x60);

    if (scancode & 0x80) {
        return;
    }

	//check if one of the arrow keys is pressed
	if (scancode == 0x48 || scancode == 0x50 || scancode == 0x4B || scancode == 0x4D) {
		navigate_arrow_keys(scancode);
		return;
	}

    char c = scancode_to_ascii(scancode);

    if (c) {
        terminal_putchar(c);
    }
}