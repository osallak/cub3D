/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:28:35 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 18:30:04 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	init_player(t_cub_manager *manager)
{
	int	found_player;
	int	i;
	int	j;

	found_player = 0;
	i = -1;
	manager->player.rotation_speed = radians(TILE_SIZE * 2);
	manager->time.delta_time = 0;
	manager->time.last_tick = 0;
	manager->player.walk_speed = TILE_SIZE * 2.5;
	i = -1;
	while (++i < manager->map->map_height)
	{
		j = -1;
		while (++j < manager->map->map_width)
			if (check_if_cell_has_player(manager, i, j))
				break ;
	}
	manager->player.move_x = false;
	manager->player.move_y = false;
	manager->player.rotate = false;
}
