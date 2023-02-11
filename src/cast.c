/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:02:10 by osallak           #+#    #+#             */
/*   Updated: 2023/02/11 20:02:47 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	calculate_ray_length(t_ray *ray, t_cub_manager *mn, \
t_cast_function *var)
{
	var->horz_hit_distance = __distance(mn->player.x, \
	mn->player.y, var->horz_wall_hit_x, var->horz_wall_hit_y);
	var->vert_hit_distance = __distance(mn->player.x, \
	mn->player.y, var->ver_hit_x, var->ver_hit_y);
	ray->was_vert_h = false;
}

void	choose_closest_intersection(t_ray *ray, t_cast_function *var)
{
	if (var->horz_hit_distance < var->vert_hit_distance)
	{
		ray->wallHitX = var->horz_wall_hit_x;
		ray->wallHitY = var->horz_wall_hit_y;
		ray->distance = var->horz_hit_distance;
	}
	else
	{
		ray->wallHitX = var->ver_hit_x;
		ray->wallHitY = var->ver_hit_y;
		ray->distance = var->vert_hit_distance;
		ray->was_vert_h = true;
	}
}

void	choose_horz_intersection(t_ray *ray, t_cast_function *var)
{
	ray->wallHitX = var->horz_wall_hit_x;
	ray->wallHitY = var->horz_wall_hit_y;
	ray->distance = var->horz_hit_distance;
}

void	choose_ver_intersection(t_ray *ray, t_cast_function *var)
{
	ray->wallHitX = var->ver_hit_x;
	ray->wallHitY = var->ver_hit_y;
	ray->distance = var->vert_hit_distance;
	ray->was_vert_h = true;
}

void	cast(t_ray *ray, t_cub_manager *manager)
{
	t_cast_function	var;

	intitialize_caster_var(&var);
	normalize_angle(&ray->ray_angle);
	initialize_horizontal_check(ray, manager, &var);
	check_horizontal_intersection(ray, manager, &var);
	initialize_vertical_check(ray, manager, &var);
	check_vertical_intersection(ray, manager, &var);
	calculate_ray_length(ray, manager, &var);
	if (var.found_horz_wall_hit && var.found_ver_hit)
		choose_closest_intersection(ray, &var);
	else if (var.found_horz_wall_hit)
		choose_horz_intersection(ray, &var);
	else if (var.found_ver_hit)
		choose_ver_intersection(ray, &var);
}
