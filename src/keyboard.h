#ifndef KEYBOARD_H
# define KEYBOARD_H

#include "terminal.h"

void keyboard_handler(void);
char scancode_to_ascii(uint8_t scancode);

#endif // KEYBOARD_H