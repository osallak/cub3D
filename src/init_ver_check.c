/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ver_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 02:12:24 by osallak           #+#    #+#             */
/*   Updated: 2023/02/12 02:12:41 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	initialize_vertical_check(t_ray *ray, t_cub_manager *manager, \
t_cast_function *var)
{
	var->xintercept = floor(manager->player.x / TILE_SIZE) * TILE_SIZE;
	if (ray->is_fac_right)
		var->xintercept += TILE_SIZE;
	var->yintercept = manager->player.y + (var->xintercept - \
	manager->player.x) * tan(ray->ray_angle);
	var->xstep = TILE_SIZE;
	if (ray->is_fac_left)
		var->xstep = -var->xstep;
	var->ystep = TILE_SIZE * tan(ray->ray_angle);
	if (ray->is_fac_up && var->ystep > 0)
		var->ystep *= -1;
	if (ray->is_fac_down && var->ystep < 0)
		var->ystep *= -1;
	var->next_ver_touch_x = var->xintercept;
	var->next_ver_touch_y = var->yintercept;
}
