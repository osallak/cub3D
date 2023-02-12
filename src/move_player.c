/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 19:53:51 by osallak           #+#    #+#             */
/*   Updated: 2023/02/11 19:57 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	move_slideways(t_cub_manager *manager, double *nx, double *ny, int sd)
{
	double	rotation_angle;

	normalize_angle(&rotation_angle);
	rotation_angle = manager->player.rotation_angle + radians(90) * sd;
	normalize_angle(&rotation_angle);
	*nx = manager->player.x + cos(rotation_angle) * \
	manager->player.walk_speed * manager->time.delta_time;
	*ny = manager->player.y + sin(rotation_angle) * \
	manager->player.walk_speed * manager->time.delta_time;
}

void	update_player_position(t_cub_manager *manager, \
t_pair_double d_b_a, bool flag)
{
	if (!flag)
		return ;
	manager->player.y = d_b_a.y;
	manager->player.x = d_b_a.x;
}

t_pair_double	get_x_y_increment(t_cub_manager *manager, double nx,
								double ny, int *st)
{
	t_pair_double	increment_xy;
	t_pair_double	dxy;
	double			steps;

	dxy.x = nx - manager->player.x;
	dxy.y = ny - manager->player.y;
	steps = fmax(fabs(dxy.x), fabs(dxy.y));
	*st = round(steps);
	increment_xy.x = dxy.x / (steps);
	increment_xy.y = dxy.y / (steps);
	return (increment_xy);
}

void	move_player_dda(t_cub_manager *manager, double nx, double ny)
{
	t_pair_double	d_b_a;
	bool			increment;
	t_pair_double	increment_xy;
	int				st;
	int				i;

	d_b_a.x = nx;
	d_b_a.y = ny;
	increment = true;
	increment_xy = get_x_y_increment(manager, nx, ny, &st);
	i = -1;
	while (++i <= st)
	{
		if (is_wall(manager, nx, ny))
		{
			increment = false;
			break ;
		}
		nx += increment_xy.x;
		ny += increment_xy.y;
	}
	update_player_position(manager, d_b_a, increment);
}

void	move_player(t_cub_manager *manager)
{
	double	nx;
	double	ny;

	nx = 0;
	ny = 0;
	if (manager->__move_slideways)
		move_slideways(manager, &nx, &ny, manager->__move_slideways);
	else if (manager->player.move_y)
	{
		ny = manager->player.y + sin(manager->player.rotation_angle) \
		* manager->player.walk_direction * manager->player.walk_speed \
		* manager->time.delta_time;
		nx = manager->player.x + cos(manager->player.rotation_angle) \
		* manager->player.walk_direction * manager->player.walk_speed * \
		manager->time.delta_time;
	}
	move_player_dda(manager, nx, ny);
	rotate_player(manager);
}
