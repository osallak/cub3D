/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 10:59:31 by yakhoudr          #+#    #+#             */
/*   Updated: 2022/12/05 17:06:14 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	ft_atoi(char *str)
{
	unsigned int	i;
	long int		res;

	i = 0;
	if (!str)
		return (0);
	res = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9') || i > 3)
			panic("Error: invalid map: invalid color input");
		res = (res * 10) + (str[i++] - '0');
	}
	if (res < 0 || res > 255)
		panic("Error: invalid map: color out of range");
	return (res);
}
