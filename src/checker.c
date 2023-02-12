/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 12:46:54 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 12:47:32 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	check_horizontal_path(char **map, long begin, long bbegin, long j)
{
	t_quadri_long	tmp;

	tmp.a = begin;
	tmp.c = get_end(map, j);
	while (tmp.a >= bbegin && map[j][tmp.a] != '1')
	{
		if (map[j][tmp.a] == ' ')
			break ;
		--tmp.a;
	}
	if ((tmp.a >= bbegin
			&& map[j][tmp.a] != '1') || tmp.a < bbegin)
		panic("Error: invalid map");
	tmp.b = begin;
	while (tmp.b <= tmp.c && map[j][tmp.b] != '1')
	{
		if (map[j][tmp.b] == ' ')
			break ;
		++tmp.b;
	}
	if ((tmp.b <= tmp.c && tmp.b >= 0 && tmp.b < ft_strlen(map[j])
			&& map[j][tmp.b] != '1') || tmp.b > tmp.c)
		panic("Error: invalid map");
}

void	_norm_check_vertical_path(t_quadri_long *tmp, long begin, char **map)
{
	while (tmp->a > -1 && map[tmp->a]
		&& begin < ft_strlen(map[tmp->a]) && map[tmp->a][begin] != '1')
	{
		if (map[tmp->a][begin] == ' ')
			break ;
		--tmp->a;
	}
}

void	check_vertical_path(char **map, long begin, long map_lines, long j)
{
	t_quadri_long	tmp;

	tmp.a = j;
	_norm_check_vertical_path(&tmp, begin, map);
	if ((tmp.a >= 0 && begin < ft_strlen(map[tmp.a])
			&& map[tmp.a][begin] != '1')
				|| tmp.a < 0 || begin > ft_strlen(map[tmp.a]))
		panic("Error: invalid map");
	tmp.b = j;
	while (tmp.b - 1 < map_lines
		&& begin < ft_strlen(map[tmp.b])
		&& map[tmp.b] && map[tmp.b][begin] != '1')
	{
		if (map[tmp.b][begin] == ' ')
			break ;
		++tmp.b;
	}
	if ((tmp.b < map_lines && map[tmp.b]
			&& begin < ft_strlen(map[tmp.b])
			&& map[tmp.b][begin] != '1') || tmp.b >= map_lines)
		panic("Error: invalid map");
}

void	check_for_walls(t_quadri_long *tmp, \
long *begin, char **map, unsigned int map_lines)
{
	if (*begin - 1 >= get_begin(map, (*tmp).a) && *begin + 1 <= \
	get_end(map, (*tmp).a) && (*tmp).a - 1 >= 0 && (*tmp).a + 1 < map_lines)
	{
		if (!((map[(*tmp).a][*begin - 1] == '1'\
		&& map[(*tmp).a][*begin + 1] == '1') \
		|| (map[(*tmp).a - 1][*begin] == '1'\
		&& map[(*tmp).a + 1][*begin] == '1')))
			panic("Error: invalid map: door is not \
surrounded by walls");
	}
}

void	check_validity_of_map(char **map, unsigned int map_lines)
{
	long			begin;
	t_quadri_long	tmp;

	tmp.a = -1;
	check_valid_space(map, map_lines);
	while (map[++tmp.a])
	{
		tmp.b = -1;
		tmp.c = get_end(map, tmp.a);
		begin = get_begin(map, tmp.a);
		tmp.d = begin;
		while (begin <= tmp.c)
		{
			if (map[tmp.a][begin] != '1' && map[tmp.a][begin] != ' ')
			{
				check_horizontal_path(map, begin, tmp.d, tmp.a);
				check_vertical_path(map, begin, map_lines, tmp.a);
			}
			if (map[tmp.a][begin] == DOOR_CHAR)
				check_for_walls(&tmp, &begin, map, map_lines);
			++begin;
		}
	}
}