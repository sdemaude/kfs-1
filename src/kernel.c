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

	// TODO : add a welcome message to each screen + how to switch between screens using F1-F4 + number of the current screen
	terminal_setcolor(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
	printk("Hello, kernel World!\n");
	printk("  _  _   ____\n");
	printk(" | || | |___ \\\n");
	printk(" | || |_  __) |\n");
	printk(" |__   _|/ __/\n");
	printk("    |_| |_____|\n");
	terminal_setcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	printk("42 - Kernel From Scratch\n");

	while (1) {
		update_screen();
		keyboard_handler();
	}
}
