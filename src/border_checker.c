/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   border_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 12:49:06 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 12:49:21 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	check_horizonal_borders(char **map, int index)
{
	unsigned int	i;
	unsigned int	end;

	end = get_end(map, index);
	i = get_begin(map, index);
	while (map[index][i] && i < end)
	{
		if (map[index] && map[index][i] != ' ' && map[index][i] != '1')
			panic("Error: invalid map");
		if (index == 0 && map[index][i] == ' ')
		{
			if (map[index + 1] && (map[index + 1][i] != '1'\
			&& map[index + 1][i] != ' '))
				panic("Error: invalid map");
		}
		else if (index != 0 && map[index][i] == ' ')
		{
			if (map[index - 1] && (map[index - 1][i] != '1'\
			&& map[index - 1][i] != ' '))
				panic("Error: invalid map");
		}
		i += 1;
	}
}

long	_norm_check_valid_space(long *i, char **map, long j, long *line_size)
{
	*i = get_begin(map, j);
	*line_size = ft_strlen(map[j]);
	return (get_end(map, j));
}

void	check_valid_space(char **map, long map_lines)
{
	long	j;
	long	i;
	long	end;
	long	line_size;

	j = -1;
	while (map[++j])
	{
		end = _norm_check_valid_space(&i, map, j, &line_size);
		while (++i <= end)
		{
			if (map[j][i] == ' ')
			{
				if ((i > 0 && map[j][i - 1] && map[j][i - 1] != '1'\
				&& map[j][i - 1] != ' ') || (i + 1 < line_size && map[j][i + 1]
				&& map[j][i + 1] != '1' && map[j][i + 1] != ' ')
				|| (j > 0 && i < ft_strlen(map[j - 1])
				&& map[j - 1][i] && map[j - 1][i] != '1'
				&& map[j - 1][i] != ' ') ||
				(j + 1 < map_lines && i < ft_strlen(map[j + 1])
				&& map[j + 1] && map[j + 1][i] != '1' && map[j + 1][i] != ' '))
					panic("Error: invalid map");
			}
		}
	}
}

void	_norm_check_map_character(t_quadri_long *tmp, char **map)
{
	tmp->d = 1;
	tmp->c = map[tmp->a][tmp->b];
}

void	check_for_duplicate_pcharacter(t_quadri_long *tmp, char **map)
{
	if ((*tmp).d
		&& ft_strchr(PLAYER_CHAR, map[(*tmp).a][(*tmp).b]))
		panic("Error: invalid map: duplicate player character");
	if (ft_strchr(PLAYER_CHAR, map[(*tmp).a][(*tmp).b]))
		_norm_check_map_character(tmp, map);
}

char	check_map_characters(char **map)
{
	t_quadri_long	tmp;

	tmp.a = -1;
	tmp.c = 0;
	tmp.d = 0;
	while (map[++tmp.a])
	{
		tmp.b = -1;
		while (++tmp.b < ft_strlen(map[tmp.a]))
		{
			if (!ft_strchr(VALID_CHARS, map[tmp.a][tmp.b]))
				panic("Error: invalid map: impostor character");
			else
				check_for_duplicate_pcharacter(&tmp, map);
		}
	}
	if (tmp.c == 0)
		panic("Error: invalid map: player character is missing");
	return ((char) tmp.c);
}
