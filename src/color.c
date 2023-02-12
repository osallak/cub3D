/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 11:59:32 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 12:00:21 by yakhoudr         ###   ########.fr       */
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

void	get_color(char **map_line, int *color_value, char color)
{
	unsigned int	i;
	unsigned int	j;
	int				k;
	char			*color_str;
	unsigned char	rgb[3];

	skip_spaces(&i, *map_line);
	k = 0;
	if (i == 1 && (*map_line)[i] == ' '\
	&& (*map_line)[i - 1] == color)
	{
		if (*color_value != -1)
			panic("Error: duplicate color value");
		while (i < ft_strlen(*map_line))
		{
			get_color_value(*map_line, &i, &j, &color_str);
			rgb[k] = ft_atoi(color_str);
			free(color_str);
			i = j + 1;
			++k;
		}	
		*color_value = create_trgb(0, rgb[0], rgb[1], rgb[2]);
	}
}

void	check_c_asset(char **map_line, t_map_manager *map_manager)
{
	get_color(map_line, &map_manager->c, 'C');
}

void	check_f_asset(char **map_line, t_map_manager *map_manager)
{
	get_color(map_line, &map_manager->f, 'F');
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}
