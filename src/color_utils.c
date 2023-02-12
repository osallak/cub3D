/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:21:07 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 18:23:40 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	get_color_value(char *map_line, unsigned int *i, \
unsigned int *j, char **color_value)
{
	while (*i < ft_strlen(map_line) && map_line[*i] == ' ')
		++(*i);
	*j = *i;
	while (*j < ft_strlen(map_line) && map_line[*j] != ',')
		++(*j);
	*color_value = ft_substr(map_line, *i, *j);
}

void	panic_rgb(int k)
{
	if (k > 2)
		panic("Error: color supported formats is rgb");
}

void	__norm_color(unsigned int *i, int *k, unsigned int *j, char *color_str)
{
	*i = *j + 1;
	++(*k);
	free(color_str);
}
