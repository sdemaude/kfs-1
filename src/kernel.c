#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "printk/printk.h"
#include "terminal.h"
#include "keyboard.h"

// /* Check if the compiler thinks you are targeting the wrong operating system. */
// #if defined(__linux__)
// #error "You are not using a cross-compiler, you will most certainly run into trouble"
// #endif

// /* This tutorial will only work for the 32-bit ix86 targets. */
// #if !defined(__i386__)
// #error "This tutorial needs to be compiled with a ix86-elf compiler"
// #endif


void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_initialize();

	// TODO : add function to print the welcome message
	terminal_setcolor(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
	printk("Hello, kernel World!\n");
	terminal_setcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	printk("A %i project...\n", 42);

	while (1) {
		// Wait for keyboard input
		keyboard_handler();
				if (has_screen_updated) {
			update_screen();
		}
	}
}