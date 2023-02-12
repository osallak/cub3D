/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 12:49:43 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 12:49:57 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	_norm_norm_fill_map(char **map, int map_fd, \
	int *index, char **filtered_line)
{
	char			*map_line;

	map_line = get_next_line(map_fd);
	if (map_line[ft_strlen(map_line) - 1] == '\n')
		*filtered_line = ft_substr(map_line, 0, ft_strlen(map_line) - 1);
	map[++(*index)] = ft_strdup(*filtered_line);
	free(map_line);
	free(*filtered_line);
}

void	_norm_add_line(char **filtered_line,
char *map_line, int	*index, char **map)
{
	if (map_line[ft_strlen(map_line) - 1] == '\n')
		*filtered_line = ft_substr(map_line, 0, ft_strlen(map_line) - 1);
	if (filtered_line && *filtered_line)
		map[++(*index)] = ft_strdup(*filtered_line);
	else
		map[++(*index)] = ft_strdup(map_line);
}

void	_norm_fill_map(char **map, int map_fd, int index, long map_lines)
{
	char			*map_line;
	char			*filtered_line;

	filtered_line = 0x0;
	_norm_norm_fill_map(map, map_fd, &index, &filtered_line);
	map_line = get_next_line(map_fd);
	if (ft_strlen(map_line) == 1)
	{
		map[++index] = 0x0;
		return ;
	}
	while (map_line && index + 1 < map_lines)
	{
		filtered_line = 0x0;
		_norm_add_line(&filtered_line, map_line, &index, map);
		free(map_line);
		free(filtered_line);
		map_line = get_next_line(map_fd);
		if (ft_strlen(map_line) == 1 || ft_strlen(map_line) == 0)
			break ;
	}
	map[++index] = 0x0;
}
