/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 12:44:38 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 12:45:26 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	_norm_get_map_size(char **map_line, const int map_fd)
{
	*map_line = get_next_line(map_fd);
	while (*map_line)
	{
		if (ft_strlen(*map_line) != 1)
			panic("Error: newline in the middle of the map");
		free(*map_line);
		*map_line = get_next_line(map_fd);
	}
}

unsigned int	get_map_size(const int map_fd, char **map_line)
{
	long	index;

	index = 0;
	if (*map_line)
		++index;
	while (*map_line)
	{
		free(*map_line);
		*map_line = get_next_line(map_fd);
		if (!*map_line || ft_strlen(*map_line) == 1)
			break ;
		++index;
		if (*map_line && ft_strlen(*map_line) == 1)
			break ;
	}
	_norm_get_map_size(&*map_line, map_fd);
	close(map_fd);
	return (index);
}

void	fill_map(char **map, int map_fd, long map_lines)
{
	char			*filtered_line;
	char			*map_line;
	unsigned int	line_size;
	long			index;

	filtered_line = NULL;
	map_line = get_next_line(map_fd);
	index = -1;
	if (!map_line)
		return ;
	line_size = ft_strlen(map_line);
	if (map_line[line_size - 1] == '\n')
		filtered_line = ft_substr(map_line, 0, line_size - 1);
	map[++index] = ft_strdup(filtered_line);
	free(map_line);
	free(filtered_line);
	_norm_fill_map(map, map_fd, index, map_lines);
}

long	get_end(char **map, int index)
{
	long	end;

	end = ft_strlen(map[index]) - 1;
	while (map[index] && end > 0 && map[index][end] == ' ')
		--end;
	return (end);
}

long	get_begin(char **map, int index)
{
	long	i;
	long	size;

	i = 0;
	size = ft_strlen(map[index]);
	while (map[index][i] == ' ' && i < size)
		++i;
	return (i);
}