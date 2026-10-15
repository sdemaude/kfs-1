#include "command.h"

// HELP
void command_help() {
    printk("Available commands:\n");
    printk("help - Display this help message\n");
    printk("clear - Clear the terminal screen\n");
    printk("print-stack - Print the current call stack\n");
    printk("halt - Halt the system\n");
    printk("reboot - Reboot the system\n");
    printk("echo <text> - Print the provided text\n");
}

// CLEAR
void command_clear() {
    terminal_initialize_current();
    set_screen_update_flag();
}


// PRINT STACK

unsigned int get_esp(void)
{
    unsigned int esp;

    __asm__ volatile ("mov %%esp, %0" : "=r"(esp));
    return esp;
}

static void print_hex32(unsigned int value)
{
    int shift;
    unsigned int digit;

    for (shift = 28; shift >= 0; shift -= 4)
    {
        digit = (value >> shift) & 0xF;
        if (digit < 10)
            printk("%c", '0' + digit);
        else
            printk("%c", 'A' + digit - 10);
    }
}

void command_print_stack(unsigned int count)
{
    unsigned int esp;
    unsigned int *stack;
    unsigned int i;
    unsigned int j;

    esp = get_esp();
    stack = (unsigned int *)esp;

    printk("Kernel stack dump\n");
    printk("Address   | +00      +04      +08      +0C\n");
    printk("----------+-----------------------------------\n");

    for (i = 0; i < count; i += 4)
    {
        print_hex32((unsigned int)&stack[i]);
        printk(" | ");

        for (j = 0; j < 4 && i + j < count; j++)
        {
            print_hex32(stack[i + j]);
            printk(" ");
        }
        printk("\n");
    }
}


// HALT
void command_halt(void)
{
    __asm__ volatile ("cli");

    while (1)
        __asm__ volatile ("hlt");
}

// REBOOT
void command_reboot(void)
{
    __asm__ volatile ("cli");

    while (inb(0x64) & 0x02)
        ;

    outb(0x64, 0xFE);

    while (1)
        __asm__ volatile ("hlt");
}

// ECHO
void command_echo(char *str)
{
    printk("%s\n", str);
}