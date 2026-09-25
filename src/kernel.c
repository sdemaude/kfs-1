#include "printk/printk.h"
#include "terminal.h"
#include "keyboard.h"


static void print_welcome_message(int screen_number) {
	terminal_setcolor(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
	printk("Hello, kernel World!\n");
	printk("Screen %d/%d\n", screen_number, SCREEN_COUNT);
	printk("Use F1-F4 to switch between screens.\n");
	printk("  _  _   ____\n");
	printk(" | || | |___ \\\n");
	printk(" | || |_  __) |\n");
	printk(" |__   _|/ __/\n");
	printk("    |_| |_____|\n");
	terminal_setcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	printk("42 - Kernel From Scratch\n\n");
}

void kernel_main(void) {
	terminal_initialize();

	for (int screen = 0; screen < SCREEN_COUNT; screen++) {
		set_current_screen(screen);
		print_welcome_message(screen + 1);
	}
	set_current_screen(0); // Start with the first screen

	while (1) {
		update_screen();
		keyboard_handler();
	}
}
