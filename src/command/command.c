#include "command.h"

// HELP
void command_help() {
    printk("Available commands:\n");
    printk("help - Display this help message\n");
    printk("clear - Clear the terminal screen\n");
}

// CLEAR
void command_clear() {
    terminal_initialize_current();
    set_screen_update_flag();
}

// PRINT STACK

// HALT

// REBOOT

// ECHO
