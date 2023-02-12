/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 11:57:26 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 11:59:20 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	check_no_asset(char **map_line, t_map_manager *map_manager)
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

void	check_so_asset(char **map_line, t_map_manager *map_manager)
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

void	check_we_asset(char **map_line, t_map_manager *map_manager)
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

void	parse_assets(t_map_manager	*map_manager, \
const int map_fd, char **map_line, long *skip)
{
	skip_lines(skip, map_line, map_fd);
	if (!*map_line)
		return ;
	while (ft_strchr(VALID_ID, *map_line[0]))
	{
		++(*skip);
		check_no_asset(map_line, map_manager);
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
