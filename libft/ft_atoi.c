/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maoliiny <maoliiny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:47:08 by maoliiny          #+#    #+#             */
/*   Updated: 2025/05/06 11:10:34 by maoliiny         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atoi(const char *nptr)
{
	long	num;
	long	minus;
	int		dig;

	num = 0;
	minus = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '+' || *nptr == '-')
		if (*nptr++ == '-')
			minus = -1;
	while (ft_isdigit(*nptr))
	{
		dig = *nptr - '0';
		num = num * 10 + dig;
		nptr++;
	}
	return (num * minus);
}
