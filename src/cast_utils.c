/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:07:47 by osallak           #+#    #+#             */
/*   Updated: 2023/02/11 20:08:45 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

bool	__inside_wall(int x, int y, bool isfacingup, t_cub_manager *mn)
{
	int		x_index;
	int		y_index;
	double	d;

	if (isfacingup == true)
		y -= 1;
	x_index = x / TILE_SIZE;
	y_index = y / TILE_SIZE;
	if (!(x_index >= 0 && x_index < mn->map->map_width \
	&& y_index >= 0 && y_index < mn->map->map_height))
		return (false);
	d = __distance(mn->player.x, mn->player.y, x, y);
	return (mn->map->map[y_index][x_index] == '1' \
	|| (mn->map->map[y_index][x_index] == 'D' && (d > TILE_SIZE)));
}

bool	__inside_wall_ver(int x, int y, bool isfacingleft, t_cub_manager *mn)
{
	int		x_index;
	int		y_index;
	double	d;

	if (isfacingleft == true)
		x -= 1;
	x_index = x / TILE_SIZE;
	y_index = y / TILE_SIZE;
	if (!(x_index >= 0 && x_index < mn->map->map_width \
	&& y_index >= 0 && y_index < mn->map->map_height))
		return (false);
	d = __distance(mn->player.x, mn->player.y, x, y);
	return (mn->map->map[y_index][x_index] == '1' \
	|| (mn->map->map[y_index][x_index] == 'D' && (d > TILE_SIZE)));
}

void	intitialize_caster_var(t_cast_function *var)
{
	var->xintercept = 0.0;
	var->yintercept = 0.0;
	var->xstep = 0.0;
	var->ystep = 0.0;
	var->found_horz_wall_hit = false;
	var->horz_wall_hit_x = 0;
	var->horz_wall_hit_y = 0;
	var->next_horz_touch_x = 0.0;
	var->next_horz_touch_y = 0.0;
	var->found_ver_hit = false;
	var->ver_hit_x = 0;
	var->ver_hit_y = 0;
	var->next_ver_touch_x = 0.0;
	var->next_ver_touch_y = 0.0;
	var->horz_hit_distance = 0.0;
	var->vert_hit_distance = 0.0;
}

void	initialize_horizontal_check(t_ray *ray, \
t_cub_manager *manager, t_cast_function *var)
{
	var->yintercept = floor(manager->player.y / TILE_SIZE) * (double)TILE_SIZE;
	if (ray->is_fac_down)
		var->yintercept += TILE_SIZE;
	var->xintercept = manager->player.x + (var->yintercept - \
	manager->player.y) / tan(ray->ray_angle);
	var->ystep = TILE_SIZE;
	if (ray->is_fac_up)
		var->ystep *= -1;
	var->xstep = TILE_SIZE / tan(ray->ray_angle);
	if (ray->is_fac_left && var->xstep > 0)
		var->xstep *= -1;
	if (ray->is_fac_right && var->xstep < 0)
		var->xstep *= -1;
	var->next_horz_touch_x = var->xintercept;
	var->next_horz_touch_y = var->yintercept;
}

void	check_horizontal_intersection(t_ray *ray, t_cub_manager *manager, \
t_cast_function *var)
{
	while (var->next_horz_touch_x >= 0 && var->next_horz_touch_x < manager->\
	map->map_width * TILE_SIZE && var->next_horz_touch_y >= 0 && var->\
	next_horz_touch_y < manager->map->map_height * TILE_SIZE)
	{
		if (__inside_wall(var->next_horz_touch_x, var->next_horz_touch_y, \
		ray->is_fac_up, manager))
		{
			var->found_horz_wall_hit = true;
			var->horz_wall_hit_x = var->next_horz_touch_x;
			var->horz_wall_hit_y = var->next_horz_touch_y;
			break ;
		}
		var->next_horz_touch_x += var->xstep;
		var->next_horz_touch_y += var->ystep;
	}
}
