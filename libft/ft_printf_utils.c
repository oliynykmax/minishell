/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maoliiny <maoliiny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 21:08:29 by maoliiny          #+#    #+#             */
/*   Updated: 2025/05/06 11:00:06 by maoliiny         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf_pnum(unsigned long long un, char *base)
{
	char	buffer[30];
	int		buffer_index;
	int		counter;

	buffer_index = 0;
	counter = 0;
	if (un == 0)
		return (write(1, &base[0], 1));
	while (un > 0)
	{
		buffer[buffer_index++] = base[un % ft_strlen(base)];
		un /= ft_strlen(base);
	}
	while (buffer_index > 0)
	{
		if (write(1, &buffer[--buffer_index], 1) == -1)
			return (-1);
		counter += 1;
	}
	return (counter);
}

int	ft_printf_diux(long un, char *base)
{
	char	buffer[30];
	int		buffer_index;
	int		counter;
	int		temp;

	buffer_index = 0;
	counter = 0;
	if (ft_strlen(base) == 10 && un < 0)
	{
		if (write(1, "-", 1) == -1)
			return (-1);
		counter++;
		un = -un;
	}
	if (un == 0)
		return (write(1, &base[0], 1));
	while (un > 0)
	{
		buffer[buffer_index++] = base[un % ft_strlen(base)];
		un /= ft_strlen(base);
	}
	temp = write_reverse(buffer, buffer_index);
	if (temp == -1)
		return (temp);
	return (counter + temp);
}

int	write_reverse(const char *buffer, int len)
{
	int	written;

	written = 0;
	while (len > 0)
	{
		if (write(1, &buffer[--len], 1) == -1)
			return (-1);
		written++;
	}
	return (written);
}
