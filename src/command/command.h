#ifndef COMMAND_H
# define COMMAND_H

#include "../printk/printk.h"
#include "../terminal.h"
#include "../gdt.h"
#include "../io.h"

void command_help();
void command_clear();
void command_print_stack();
void command_halt();
void command_reboot();
void command_echo(char* text);

#endif // COMMAND_H