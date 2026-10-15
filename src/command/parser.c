#include "parser.h"

void parse_command(char* current_line) {
	if (current_line[0] == '\0') {
		return; // No command to parse
	}

	if (strcmp(current_line, "help") == 0) {
		command_help();
	} else if (strcmp(current_line, "clear") == 0) {
		command_clear();
	} else if (strcmp(current_line, "print-stack") == 0) {
		command_print_stack(100); // Print 100 stack entries
	} else if (strcmp(current_line, "halt") == 0) {
		command_halt();
	} else if (strcmp(current_line, "reboot") == 0) {
		command_reboot();
	} else if (strncmp(current_line, "echo ", 5) == 0) {
		command_echo(current_line + 5);
	} else {
		printk("Unknown command: %s\n", current_line);
	}
}