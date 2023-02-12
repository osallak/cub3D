/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 15:59:25 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 12:50:46 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	init_map_manager(t_map_manager *map_manager)
{
	map_manager->c = -1;
	map_manager->f = -1;
	map_manager->no = 0x0;
	map_manager->so = 0x0;
	map_manager->we = 0x0;
	map_manager->ea = 0x0;
}

static void	check_for_assets(t_map_manager *map_manager)
{
	if (map_manager->f == -1
		|| map_manager->c == -1
		|| !map_manager->no
		|| !map_manager->so
		|| !map_manager->we
		|| !map_manager->ea)
		panic("Error: can't retrieve assets");
}

static void	skip_parsed_assets(char **map_line, int skip, char *file)
{
	int	map_fd;

	map_fd = open(file, O_RDONLY);
	if (map_fd == -1)
		exit(EXIT_FAILURE);
	while (skip--)
	{
		*map_line = get_next_line(map_fd);
		free(*map_line);
	}
}

void	parse_map_file(int map_fd, char *file)
{
	char			**map;
	unsigned int	map_lines;
	char			*map_line;
	t_map_manager	*map_manager;
	long			skip;

	skip = 0;
	map_manager = xalloc(sizeof(t_map_manager));
	init_map_manager(map_manager);
	parse_assets(map_manager, map_fd, & map_line, &skip);
	check_for_assets(map_manager);
	map_lines = get_map_size(map_fd, &map_line);
	if (map_lines < 3)
		panic("Error: invalid map");
	map = malloc(sizeof(char *) * (map_lines + 1));
	if (!map)
		exit(EXIT_FAILURE);
	skip_parsed_assets(&map_line, skip, file);
	fill_map(map, map_fd, map_lines);
	map_manager->c_player = check_map_characters(map);
	check_validity_of_map(map, map_lines);
	map_manager->map = map;
	render(map_manager);
}
