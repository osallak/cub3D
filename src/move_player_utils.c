/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:50:19 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 18:50:26 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

bool	is_wall(t_cub_manager *manager, double nx, double ny)
{
	return (nx / TILE_SIZE >= 0 && nx / TILE_SIZE < \
	manager->map->map_width && ny / TILE_SIZE >= \
	0 && ny / TILE_SIZE < manager->map->map_height \
	&& (manager->map->map[(int) \
ny / TILE_SIZE][(int) \
nx / TILE_SIZE] == '1' || \
	(manager->map->map[(int) \
	ny / TILE_SIZE][(int) manager->player.x / TILE_SIZE] == '1' \
	&& manager->map->map[(int) \
manager->player.y / TILE_SIZE][(int) \
nx / TILE_SIZE] == '1')));
}
