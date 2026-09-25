#include "parser.h"

void parse_command(char* current_line) {
	if (current_line[0] == '\0') {
		return; // No command to parse
	}

	if (strcmp(current_line, "help") == 0) {
		command_help();
	} else if (strcmp(current_line, "clear") == 0) {
		command_clear();
	} else {
		printk("Unknown command: %s\n", current_line);
	}
}