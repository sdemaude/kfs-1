#include "printk.h"
#include <stdint.h>

int	print_char(char c)
{
	return (terminal_write(&c, 1));
}

int	print_str(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (print_str("(null)"));
	while (str[i])
		terminal_write(&str[i++], 1);
	return (i);
}

int	print_digit(long nb, int base, int m)
{
	int		ct;
	char	*formatmin;
	char	*formatmaj;

	ct = 0;
	formatmin = "0123456789abcdef";
	formatmaj = "0123456789ABCDEF";
	if (nb < 0 && base == 10)
	{
		terminal_write("-", 1);
		return (print_digit(-nb, base, m) + 1);
	}
	if (nb < base && m == 0)
		return (print_char(formatmin[nb]));
	if (nb < base && m == 1)
		return (print_char(formatmaj[nb]));
	else
	{
		ct = print_digit(nb / base, base, m);
		return (print_digit(nb % base, base, m) + ct);
	}
}


int	print_adress(unsigned long long nb)
{
	int		ct;
	char	*format;

	ct = 0;
	format = "0123456789abcdef";
	if (nb == 0)
		return (print_str("(nil)"));
	ct += terminal_write("0x", 2);
	if (nb < 16)
		ct += print_char(format[nb]);
	else
	{
		ct += print_adress(nb / 16);
		ct += print_adress(nb % 16);
	}
	return (ct);
}
