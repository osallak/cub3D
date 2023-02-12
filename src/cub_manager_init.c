/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_manager_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 19:09:04 by osallak           #+#    #+#             */
/*   Updated: 2023/02/12 18:29:52 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	free_old_map(t_cub_manager *manager, char **map)
{
	int	i;

	i = -1;
	while (manager->map->map[++i])
		free(manager->map->map[i]);
	free(manager->map->map);
	manager->map->map = map;
}

void	init_map(t_cub_manager *manager, t_map_manager *mapm)
{
	char	**map;
	int		i;
	int		j;

	manager->map = mapm;
	manager->map->map_width = get_map_width(manager->map);
	manager->map->map_height = get_map_height(manager->map);
	map = xalloc(sizeof(char *) * manager->map->map_height);
	i = -1;
	while (++i < manager->map->map_height)
		map[i] = xalloc(manager->map->map_width);
	i = -1;
	while (++i < manager->map->map_height)
	{
		j = -1;
		while (++j < manager->map->map_width)
		{
			if (j < (int)ft_strlen(manager->map->map[i]))
				map[i][j] = manager->map->map[i][j];
			else
				map[i][j] = ' ';
		}
	}
	free_old_map(manager, map);
}

int	check_if_cell_has_player(t_cub_manager *manager, int i, int j)
{
	if (ft_strchr(PLAYER_CHAR, manager->map->map[i][j]))
	{
		manager->player.x = j * TILE_SIZE + TILE_SIZE / 2.0;
		manager->player.y = i * TILE_SIZE + TILE_SIZE / 2.0;
		if (manager->map->map[i][j] == 'S')
			manager->player.rotation_angle = radians(90);
		if (manager->map->map[i][j] == 'N')
			manager->player.rotation_angle = radians(270);
		if (manager->map->map[i][j] == 'W')
			manager->player.rotation_angle = radians(180);
		if (manager->map->map[i][j] == 'E')
			manager->player.rotation_angle = 0;
		return (1);
	}
	return (0);
}

void	init_manager_attr(t_cub_manager *manager)
{
	manager->rays = xalloc((WIDTH / WALL_STRIP_WIDTH) * sizeof(t_ray));
	manager->weapons.gun_state = STAND;
	manager->weapons.gun_frames = 0;
	manager->weapons.gun_type = PISTOL;
	manager->door = 0x0;
	manager->head = 0x0;
	manager->tail = 0x0;
	manager->next = 0x0;
	manager->mouse_x = WIDTH / 2;
	manager->mouse_move = false;
	manager->__move_slideways = 0;
}
