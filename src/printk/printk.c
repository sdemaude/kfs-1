#include "printk.h"

static int	print_format(char conv, va_list ap)
{
	int	ct;

	ct = 0;
	if (conv == 'c')
		ct += print_char(va_arg(ap, int));
	else if (conv == 's')
		ct += print_str(va_arg(ap, char *));
	else if (conv == 'p')
		ct += print_adress(va_arg(ap, unsigned long long));
	else if (conv == 'd')
		ct += print_digit(va_arg(ap, int), 10, 0);
	else if (conv == 'i')
		ct += print_digit(va_arg(ap, int), 10, 0);
	else if (conv == 'u')
		ct += print_digit(va_arg(ap, unsigned int), 10, 0);
	else if (conv == 'x')
		ct += print_digit(va_arg(ap, unsigned int), 16, 0);
	else if (conv == 'X')
		ct += print_digit(va_arg(ap, unsigned int), 16, 1);
	else
		ct += terminal_write(&conv, 1);
	return (ct);
}

int	printk(const char *format, ...)
{
	va_list	ap;
	int		count;

	va_start(ap, format);
	count = 0;
	while (*format != '\0')
	{
		if (*format == '%')
			count += print_format(*(++format), ap);
		else
			count += terminal_write(format, 1);
		format++;
	}
	va_end(ap);
	return (count);
}
