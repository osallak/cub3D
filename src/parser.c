/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 15:59:25 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/09 11:38:00 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"
int	create_trgb(int t, int r, int g, int b);
void	panic(const char *str)
{
	printf("%s\n", str);
	exit(EXIT_FAILURE);
}

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
	// char	*map_line;
	long	index;

	// map_line = get_next_line(map_fd);
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
	// printf("--->%d\n", end);
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
	while (map[index][i]  == ' ' && i < size)
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

long _norm_check_valid_space(long *i, char **map, long j, long *line_size)
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
			{
				if (begin - 1 >= get_begin(map, tmp.a) && begin + 1 <= get_end(map, tmp.a) && tmp.a - 1 >= 0 && tmp.a + 1 < map_lines)
				{
					if (!((map[tmp.a][begin - 1] == '1' && map[tmp.a][begin + 1] == '1') || (map[tmp.a - 1][begin] == '1' && map[tmp.a + 1][begin] == '1')))
					    panic("Error: invalid map: door is not surrounded by walls");
                }
			}
			++begin;
		}
	}
}

void	_norm_check_map_character(t_quadri_long* tmp, char **map)
{
	tmp->d = 1;
	tmp->c = map[tmp->a][tmp->b];
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
			{
				panic("Error: invalid map: impostor character");
			}
			else
			{
				if (tmp.d
					&& ft_strchr(PLAYER_CHAR, map[tmp.a][tmp.b]))
					panic("Error: invalid map");
				if (ft_strchr(PLAYER_CHAR, map[tmp.a][tmp.b]))
					_norm_check_map_character(&tmp, map);
			}
		}
	}
	if (tmp.c == 0)
		panic("Error: invalid map: player character is missing");
	return ((char) tmp.c);
}

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
	*map_line = (get_next_line(map_fd));
	while (*map_line && *map_line[0] == '\n')
	{
		free(*map_line);
		*map_line = (get_next_line(map_fd));
		++(*skip);
	}
	*map_line = filter_line(*map_line);
}

void	check_no_asset(char **map_line, t_map_manager* map_manager)
{
	char	**splitted;
	int		fd;

	splitted = ft_split(*map_line, ' ');
	if (!splitted[1])
		panic("Error: missing asset");
	if (splitted && !my_strcmp(splitted[0], "NO") && splitted[1])
	{
		if (map_manager->no)
			panic("Erorr: invalid map");
		fd = open(splitted[1], O_RDONLY);
		if (fd == -1)
			panic("Error: can't open asset file");
		map_manager->no = ft_strdup(splitted[1]);
	}
}

void	check_so_asset(char **map_line, t_map_manager* map_manager)
{
	char	**splitted;
	int		fd;

	splitted = ft_split(*map_line, ' ');
	if (splitted && !my_strcmp(splitted[0], "SO") && splitted[1])
	{
		if (map_manager->so)
			panic("Erorr: invalid map");
		fd = open(splitted[1], O_RDONLY);
		if (fd == -1)
			panic("Error: can't open asset file");
		close(fd);
		map_manager->so = ft_strdup(splitted[1]);
	}
}

void	check_we_asset(char **map_line, t_map_manager* map_manager)
{
	char	**splitted;
	int		fd;

	splitted = ft_split(*map_line, ' ');
	if (splitted && !my_strcmp(splitted[0], "WE") && splitted[1])
	{
		if (map_manager->we)
			panic("Erorr: invalid map");
		fd = open(splitted[1], O_RDONLY);
		if (fd == -1)
			panic("Error: can't open asset file");
		close(fd);
		map_manager->we = ft_strdup(splitted[1]);
	}
}

void	check_ea_asset(char **map_line, t_map_manager *map_manager)
{
	char	**splitted;
	int		fd;

	splitted = ft_split(*map_line, ' ');
	if (splitted && !my_strcmp(splitted[0], "EA") && splitted[1])
	{
		if (map_manager->ea)
			panic("Erorr: invalid map");
		fd = open(splitted[1], O_RDONLY);
		if (fd == -1)
			panic("Error: can't open asset file");
		close(fd);
		map_manager->ea = ft_strdup(splitted[1]);
	}
}

void	check_c_asset(char **map_line, t_map_manager *map_manager)
{
	char	**splitted;
	char	**colors;

	splitted = ft_split(*map_line, ' ');
	if (splitted && !my_strcmp(splitted[0], "C") && splitted[1])
	{
		if (map_manager->c != -1)
			panic("Error: invalid map: duplicate ceil color");
		colors = ft_split(splitted[1], ',');
		if (splitted[1][0] == ',')
			panic("error: `, at the begin of C asset");
		for (unsigned int i = 0;i < ft_strlen(splitted[1]);++i)
		{
			if (splitted[1][i] == ',' && splitted[1][i+1] == ',')
				panic("error: duplicate `,");
			if (splitted[1][i] == ',' && !splitted[1][i + 1])
			{
				panic("error: `, at end of string");
			}
		}
		if (!colors || (colors && (!colors[0] || !colors[1] || !colors[2])))
			panic("Error: missing colors");
		map_manager->c = create_trgb(0, ft_atoi(colors[0]),
				ft_atoi(colors[1]), ft_atoi(colors[2]));
	}
}

void	check_f_asset(char **map_line, t_map_manager *map_manager)
{
	char	**colors;
	char	**splitted;

	splitted = ft_split(*map_line, ' ');
	if (splitted && !my_strcmp(splitted[0], "F") && splitted[1])
	{
		if (map_manager->f != -1)
			panic("Error: invalid map: duplicate floor color");
		colors = ft_split(splitted[1], ',');
		if (splitted[1][0] == ',')
			panic("error: `, at the begin of F asset");
		for (unsigned int i = 0;i < ft_strlen(splitted[1]);++i)
		{
			if (splitted[1][i] == ',' && splitted[1][i+1] == ',')
				panic("error: duplicate `,");
			if (splitted[1][i] == ',' && !splitted[1][i + 1])
			{
				panic("error: `, at end of string");
			}
		}
		if (!colors || (colors && (!colors[0] || !colors[1] || !colors[2])))
			panic("Error: missing colors");
		map_manager->f = create_trgb(0, ft_atoi(colors[0]),
				ft_atoi(colors[1]), ft_atoi(colors[2]));
	}
}

void	parse_assets(t_map_manager	*map_manager, const int map_fd, char **map_line, long *skip)
{
	skip_lines(skip, map_line, map_fd);
	if (!*map_line)
		return ;
	while (ft_strchr(VALID_ID, *map_line[0]))
	{
		++(*skip);
		check_no_asset(map_line, map_manager); // free 
		check_so_asset(map_line, map_manager);
		check_we_asset(map_line, map_manager);
		check_ea_asset(map_line, map_manager);
		check_c_asset(map_line, map_manager);
		check_f_asset(map_line, map_manager);
		skip_lines(skip, map_line, map_fd);
		if (!*map_line)
			return ;
	}
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}


void	init_map_manager(t_map_manager *map_manager)
{
	map_manager->c = -1;
	map_manager->f = -1;
	map_manager->no = 0x0;
	map_manager->so = 0x0;
	map_manager->we = 0x0;
	map_manager->ea = 0x0;
}

void	parse_map_file(int map_fd, char *file)
{
	char			**map;
	unsigned int	map_lines;
	char			*map_line;
	t_map_manager	*map_manager = xalloc(sizeof(t_map_manager));
	long			skip = 0;

	init_map_manager(map_manager);
	parse_assets(map_manager, map_fd, & map_line, &skip);
	if (map_manager->f == -1 || map_manager->c == -1 || !map_manager->no || !map_manager->so
	|| !map_manager->we || !map_manager->ea)
		panic("Error: can't retrieve assets");
	// printf("%ld\n", skip);
	map_lines = get_map_size(map_fd, &map_line);
	// printf("%d\n", map_lines);
	if (map_lines < 3)
		panic("Error: invalid map");
	map = malloc(sizeof(char *) * (map_lines + 1));
	if (!map)
		exit(EXIT_FAILURE);
	map_fd = open(file, O_RDONLY);
	while (skip--)
	{
		map_line = get_next_line(map_fd);
		free(map_line);
	}
	
	if (map_fd == -1)
		exit(EXIT_FAILURE);
	fill_map(map, map_fd, map_lines);
	map_manager->c_player = check_map_characters(map);
	// for (unsigned int i = 0;i < map_lines;i += 1)
	// 	printf("%s\n", map[i]);
	check_validity_of_map(map, map_lines);
	map_manager->map = map;
	render(map_manager);
}
