/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maoliiny <maoliiny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:28:03 by maoliiny          #+#    #+#             */
/*   Updated: 2025/05/05 14:16:32 by maoliiny         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf(const char *s, ...)
{
	va_list	args;
	int		counter;
	int		temp;

	counter = 0;
	if (!s)
		return (-1);
	va_start(args, s);
	while (*s)
	{
		if (*s != '%')
		{
			if (write(1, s++, 1) == -1)
				return (-1);
			counter += 1;
			continue ;
		}
		temp = choose_type(&args, &s);
		if (temp == -1)
			return (temp);
		counter += temp;
		s++;
	}
	va_end(args);
	return (counter);
}

int	choose_type(va_list *args, const char **s)
{
	(*s)++;
	if (**s == 's')
		return (ft_printf_s(va_arg(*args, char *)));
	else if (**s == 'c')
		return (ft_printf_c(va_arg(*args, int)));
	else if (**s == '%')
		return (write(1, "%", 1));
	else if (**s == 'd' || **s == 'i')
		return (ft_printf_diux(va_arg(*args, int), "0123456789"));
	else if (**s == 'u')
		return (ft_printf_diux(va_arg(*args, unsigned), "0123456789"));
	else if (**s == 'x')
		return (ft_printf_diux(va_arg(*args, unsigned), "0123456789abcdef"));
	else if (**s == 'X')
		return (ft_printf_diux(va_arg(*args, unsigned), "0123456789ABCDEF"));
	else if (**s == 'p')
		return (ft_printf_p(va_arg(*args, void *)));
	if (write(1, "%", 1) == -1 || write(1, *s, 1) == -1)
		return (-1);
	return (2);
}

int	ft_printf_c(char c)
{
	return (write(1, &c, 1));
}

int	ft_printf_s(char *s)
{
	if (!s)
		return (write(1, "(null)", 6));
	return (write(1, s, ft_strlen(s)));
}

int	ft_printf_p(void *ptr)
{
	if (!ptr)
		return (write(1, "(nil)", 5));
	if (write(1, "0x", 2) == -1)
		return (-1);
	return (2 + ft_printf_pnum((unsigned long long)ptr, "0123456789abcdef"));
}
