/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_textures.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 22:47:11 by osallak           #+#    #+#             */
/*   Updated: 2023/02/12 13:42:50 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	get_ver_tex(t_cub_manager *manager, t_rend_attr *att, int i)
{
	int	tex;

	tex = -1;
	att->off_x = fmod(manager->rays[i].wall_hit_y, TILE_SIZE);
	att->y = manager->rays[i].wall_hit_y / TILE_SIZE;
	if (manager->rays[i].is_fac_right)
	{
		att->x = (manager->rays[i].wall_hit_x + 1.) / TILE_SIZE;
		if (manager->map->map[att->y][att->x] != 'D')
			tex = EAST;
		else
			tex = DOOR;
	}
	else
	{
		att->x = (manager->rays[i].wall_hit_x - 1.) / TILE_SIZE;
		if (manager->map->map[att->y][att->x] != 'D')
			tex = WEST;
		else
			tex = DOOR;
	}
	return (tex);
}

int	get_hor_tex(t_cub_manager *manager, t_rend_attr *att, int i)
{
	int	tex;

	tex = -1;
	att->x = manager->rays[i].wall_hit_x / TILE_SIZE;
	if (manager->rays[i].is_fac_down)
	{
		att->y = (manager->rays[i].wall_hit_y + 1.) / TILE_SIZE;
		if (manager->map->map[att->y][att->x] != 'D')
			tex = SOUTH;
		else
			tex = DOOR;
	}
	else
	{
		att->y = (manager->rays[i].wall_hit_y - 1.) / TILE_SIZE;
		if (manager->map->map[att->y][att->x] != 'D')
			tex = NORTH;
		else
			tex = DOOR;
	}
	att->off_x = fmod(manager->rays[i].wall_hit_x, TILE_SIZE);
	return (tex);
}
