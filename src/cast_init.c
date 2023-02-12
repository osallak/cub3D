/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 22:44:02 by osallak           #+#    #+#             */
/*   Updated: 2023/02/12 13:42:50 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	cast_all_rays(t_cub_manager *manager)
{
	double	ray_angle;
	int		i;
	double	angle_increment;

	angle_increment = radians(FOV) / (WIDTH / WALL_STRIP_WIDTH);
	ray_angle = manager->player.rotation_angle - (radians(FOV) / 2.0);
	i = -1;
	while (++i < (WIDTH / WALL_STRIP_WIDTH))
	{
		normalize_angle(&ray_angle);
		manager->rays[i].ray_angle = ray_angle;
		__initialize_ray_attributes(&manager->rays[i]);
		cast(&manager->rays[i], manager);
		ray_angle += angle_increment;
	}
	rendering_3d_walls(manager);
}

void	check_vertical_intersection(t_ray *ray, t_cub_manager *manager, \
t_cast_function *var)
{
	while (var->next_ver_touch_x >= 0 && var->next_ver_touch_x \
	< manager->map->map_width * TILE_SIZE && var->next_ver_touch_y \
	>= 0 && var->next_ver_touch_y < manager->map->map_height * TILE_SIZE)
	{
		if (__inside_wall_ver(var->next_ver_touch_x, \
		var->next_ver_touch_y, ray->is_fac_left, manager))
		{
			var->found_ver_hit = true;
			var->ver_hit_x = var->next_ver_touch_x;
			var->ver_hit_y = var->next_ver_touch_y;
			break ;
		}
		var->next_ver_touch_x += var->xstep;
		var->next_ver_touch_y += var->ystep;
	}
}

void	__initialize_ray_attributes(t_ray *ray)
{
	ray->distance = 0;
	ray->wall_hit_x = 0;
	ray->wall_hit_y = 0;
	ray->was_vert_h = false;
	if (ray->ray_angle > 0 && ray->ray_angle < M_PI)
		ray->is_fac_down = true;
	else
		ray->is_fac_down = false;
	ray->is_fac_up = !ray->is_fac_down;
	if (ray->ray_angle < M_PI_2 || ray->ray_angle > 1.5 * M_PI)
		ray->is_fac_right = true;
	else
		ray->is_fac_right = false;
	ray->is_fac_left = !ray->is_fac_right;
}
