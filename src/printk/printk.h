#ifndef PRINTK_H
# define PRINTK_H

# include <stdarg.h>
# include "../terminal.h"


int	printk(const char *format, ...);
int	print_char(char c);
int	print_str(char *str);
int	print_digit(long nb, int base, int m);
int	print_adress(unsigned long long nb);

#endif // PRINTK_H