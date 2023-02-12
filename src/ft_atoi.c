/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 10:59:31 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 18:17:16 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	__atoi_norm(char *str, int *begin, long int *res, unsigned int *i)
{
	if (!(str[*begin] >= '0' && str[*begin] <= '9') || *i > 2)
		panic("Error: invalid map: invalid color input");
	*res = (*res * 10) + (str[*begin] - '0');
	++(*i);
	++(*begin);
}

int	ft_atoi(char *str)
{
	unsigned int	i;
	long int		res;
	int				begin;
	int				end;

	i = 0;
	if (!str)
		return (0);
	res = 0;
	while (str[i] == ' ')
		++i;
	begin = i;
	end = ft_strlen(str) - 1;
	while (str[end] == ' ')
		--end;
	i = 0;
	if (end < begin)
		panic("Error: invalid colors");
	while (begin <= end)
		__atoi_norm(str, &begin, &res, &i);
	if (res < 0 || res > 255)
		panic("Error: invalid map: color out of range");
	return (res);
}
