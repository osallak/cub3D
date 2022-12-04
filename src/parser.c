/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 15:59:25 by yakhoudr          #+#    #+#             */
/*   Updated: 2022/12/04 19:42:05 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	panic(const char *str)
{
	printf("%s\n", str);
	exit(EXIT_FAILURE);
}

unsigned int	get_map_size(const int map_fd)
{
	char			*map_line;
	unsigned int	index;

	map_line = get_next_line(map_fd);
	index = -1;
	if (map_line)
		++index;
	while (map_line)
	{
		free(map_line);
		++index;
		map_line = get_next_line(map_fd);
		if (map_line && ft_strlen(map_line) == 1)
			break;
	}
	close(map_fd);
	return (index);
}

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

void	_norm_fill_map(char **map, int map_fd, int index)
{
	char			*map_line;
	char			*filtered_line;

	filtered_line = 0x0;
	_norm_norm_fill_map(map, map_fd, &index, &filtered_line);
	map_line = get_next_line(map_fd);
	while (map_line)
	{
		filtered_line = 0x0;
		if (map_line[ft_strlen(map_line) - 1] == '\n')
			filtered_line = ft_substr(map_line, 0, ft_strlen(map_line) - 1);
		if (filtered_line)
			map[++index] = ft_strdup(filtered_line);
		else
			map[++index] = ft_strdup(map_line);
		free(map_line);
		free(filtered_line);
		map_line = get_next_line(map_fd);
	}
	map[++index] = 0x0;
}

void	fill_map(char **map, int map_fd)
{
	char			*filtered_line;
	char			*map_line;
	unsigned int	line_size;
	unsigned int	index;

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
	_norm_fill_map(map, map_fd, index);
}

unsigned int	get_end(char **map, int index)
{
	unsigned int	end;

	end = ft_strlen(map[index]) - 1;
	// printf("--->%d\n", end);
	while (map[index][end] == ' ')
		--end;
	return (end);
}

unsigned int	get_begin(char **map, int index)
{
	unsigned int	i;

	i = 0;
	while (map[index][i] == ' ')
		++i;
	return (i);
}

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

// void	check_line(char **map, char *line, int index)
// {
// 	unsigned int	line_size;
// 	unsigned int	begin;
// 	unsigned int	end;

// 	begin = get_begin(map, index);
// 	end = get_end(map, index);
	
// }

void	check_valid_space(char **map, long map_lines)
{
	long	j;
	long	i;
	long	end;
	long	line_size;

	j = -1;
	while (map[++j])
	{
		i = get_begin(map, j);
		line_size = ft_strlen(map[j]);
		end = get_end(map, j);
		while (++i <= end)
		{
			if (map[j][i] == ' ')
			{
				if ((i > 0 && map[j][i - 1]
					&& map[j][i - 1] != '1' && map[j][i - 1] != ' ')
				|| (i + 1 < line_size && map[j][i + 1]
				&& map[j][i + 1] != '1' && map[j][i + 1] != ' ')
				|| (j > 0 && i < ft_strlen(map[j - 1])
				&& map[j - 1][i] && map[j - 1][i] != '1'
				&& map[j - 1][i] != ' ') || (j + 1 < map_lines
				&& i < ft_strlen(map[j + 1])
				&& map[j + 1] && map[j + 1][i] != '1' && map[j + 1][i] != ' '))
					panic("Error: invalid map");
			}
		}
	}
}

typedef	struct	s_quadri_long
{
	long	end;
	long	left;
	long	right;
	long	bbegin;
}	t_quadri_long;

void	check_horizontal_path(char **map, long begin, long bbegin, long j)
{
	t_quadri_long	tmp;

	tmp.left = begin;
	tmp.end = get_end(map, j);
	while (tmp.left >= bbegin && map[j][tmp.left] != '1')
	{
		if (map[j][tmp.left] == ' ')
			break ;
		--tmp.left;
	}
	if ((tmp.left >= bbegin
			&& map[j][tmp.left] != '1') || tmp.left < bbegin)
		panic("Error: invalid map");
	tmp.right = begin;
	while (tmp.right <= tmp.end && map[j][tmp.right] != '1')
	{
		if (map[j][tmp.right] == ' ')
			break ;
		++tmp.right;
	}
	if ((tmp.right <= tmp.end
			&& map[j][tmp.right] != '1') || tmp.right > tmp.end)
		panic("Error: invalid map");
}

void	check_vertical_path(char **map, long begin, long map_lines, long j)
{
	t_quadri_long	tmp;

	tmp.left = j;
	while (tmp.left > -1 && map[tmp.left]
		&& begin < ft_strlen(map[tmp.left]) && map[tmp.left][begin] != '1')
	{
		if (map[tmp.left][begin] == ' ')
			break ;
		--tmp.left;
	}
	if ((tmp.left >= 0 && map[tmp.left][begin] != '1') || tmp.left < 0)
		panic("Error: invalid map");
	tmp.right = j;
	while (tmp.right - 1 < map_lines
		&& begin < ft_strlen(map[tmp.right])
		&& map[tmp.right] && map[tmp.right][begin] != '1')
	{
		if (map[tmp.right][begin] == ' ')
			break ;
		++tmp.right;
	}
	if ((tmp.right < map_lines && map[tmp.right]
			&& map[tmp.right][begin] != '1') || tmp.right >= map_lines)
		panic("Error: invalid map");
}

void	check_validity_of_map(char **map, unsigned int map_lines)
{
	long	i;
	long	j;
	// long	line_size;
	// int				found_upper_wall = 0;

	// first non 1 character encounterd if first row check if upper and left bound exist
	// 
	// j = -1;
	// (void)map_lines;
	// while (map[++j])
	// {
	// 	i = get_begin(map, j);
	// 	line_size = ft_strlen(map[j]);
	// 	unsigned int end = get_end(map, j);
	// 	while (++i <= end)
	// 	{
	// 		if (map[j][i] == ' ')
	// 		{
	// 			if ((i > 0 && map[j][i - 1] && map[j][i - 1] != '1' && map[j][i - 1] != ' ') || (i + 1 < line_size && map[j][i + 1] && map[j][i + 1] != '1' && map[j][i + 1] != ' ')||
	// 			(j > 0 && i < ft_strlen(map[j - 1]) && map[j - 1][i] && map[j - 1][i] != '1' && map[j - 1][i] != ' ') || (j + 1 < map_lines && i < ft_strlen(map[j + 1]) && map[j + 1] && map[j + 1][i] != '1' && map[j + 1][i] != ' '))
	// 			{
	// 				printf("space!!!line: %ld\tcol: %ld\n", j, i);
	// 				panic("Error: invalid map");
	// 			}
	// 		}
	// 	}
	// }
	check_valid_space(map, map_lines);
	// check_horizontal_path(map)
	j = -1;
	while (map[++j])
	{
		i = -1;
		long end = get_end(map, j);
		long begin = get_begin(map, j);
		long bbegin = begin;
		while(begin <= end)
		{
			if (map[j][begin] != '1' && map[j][begin] != ' ')
			{
				check_horizontal_path(map, begin, bbegin, j);
				// check_vertical_path(map, begin, bbegin, j);
				check_vertical_path(map, begin, map_lines, j);
				// long left = begin;
				// // printf("left:%ld\n", i);
				// while(left >= bbegin && map[j][left] != '1')
				// {
				// 	if (map[j][left] == ' ')
				// 		break;
				//  	left --;
				// }
				// if ((left >= bbegin && map[j][left] != '1') || left < bbegin)
				// {
				// 	printf("left!!!line: %ld\tcol: %ld\n", j, left);
				// 	panic("Error: invalid map");
				// }
				// long right = begin;
				// while (right <= end && map[j][right] != '1')
				// {
				// 	if (map[j][right] == ' ')
				// 		break ;
				// 	right += 1;
				// }
				// if ((right <= end && map[j][right] != '1') || right > end)
				// {
				// 	printf("right!!!line: %ld\tcol: %ld\n", j, right);
				// 	panic("Error: invalid map");
				// }
				// long up = j;
				// //printf("up:%ld\n", j);
				// while (up > -1 && map[up] && begin < ft_strlen(map[up]) && map[up][begin] != '1')
				// {
				// 	if (map[up][begin] == ' ')
				// 		break;
				// 	--up;
				// }
				// if ((up >= 0 && map[up][begin] != '1') || up < 0)
				// {
				// 	printf("up!!!line: %ld\tcol: %ld\n", up, i);
				// 	panic("Error: invalid map");
				// }
				// long down = j;
				// while (down - 1 < map_lines && begin < ft_strlen(map[down]) && map[down] && map[down][begin] != '1')
				// {
				// 	if (map[down][begin] == ' ')
				// 		break;
				// 	++down;
				// }
				// if ((down < map_lines && map[down] && map[down][begin] != '1') || down >= map_lines)
				// {
				// 	printf("down!!!line: %ld\tcol: %ld\n", down, i);
				// 	panic("Error: invalid map");
				// }
			}
			++begin;
		}
	}
}

void	parse_map_file(int map_fd, char *file)
{
	char			**map;
	unsigned int	map_lines;

	// TODO: parse assets
	map_lines = get_map_size(map_fd);
	if (map_lines < 3)
		panic("Error: invalid map");
	map = malloc(sizeof(char *) * (map_lines + 1));
	if (!map)
		exit(EXIT_FAILURE);
	map_fd = open(file, O_RDONLY);
	if (map_fd == -1)
		exit(EXIT_FAILURE);
	fill_map(map, map_fd);
	check_validity_of_map(map, map_lines);
}
