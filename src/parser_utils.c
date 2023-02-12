/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 12:01:35 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 16:10:28 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	my_strcmp(char *a, char *b)
{
	int	i;

	if (!a || !b)
		return (0);
	i = 0;
	while (a[i])
	{
		if (a[i] != b[i])
			break ;
		++i;
	}
	return ((unsigned char)a[i] - (unsigned char)b[i]);
}

char	*filter_line(char *line)
{
	char	*res;
	long	size;

	if (!line)
		return (0x0);
	size = ft_strlen(line);
	if (line[size - 1] == '\n')
	{
		res = ft_substr(line, 0, size - 1);
		free(line);
		return (res);
	}
	return (line);
}

void	skip_lines(long *skip, char **map_line, const int map_fd)
{
	free(*map_line);
	*map_line = (get_next_line(map_fd));
	while (*map_line && *map_line[0] == '\n')
	{
		free(*map_line);
		*map_line = (get_next_line(map_fd));
		++(*skip);
	}
	*map_line = filter_line(*map_line);
}

void	skip_spaces(unsigned int *i, char *map_line)
{
	*i = 0;
	while (*i < ft_strlen(map_line))
	{
		if (map_line[*i] == ' ')
			break ;
		++(*i);
	}
}
