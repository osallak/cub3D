/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 18:56:04 by osallak           #+#    #+#             */
/*   Updated: 2023/02/12 01:59:01 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

double	radians(double angle)
{
	return (angle * (M_PI / (double)180));
}

double	dist(int x1, int x2, int y1, int y2)
{
	return (sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0));
}

double	__distance(double x, double y, double x1, double y1)
{
	return (sqrt((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y)));
}

void	calc_wall_att(t_cub_manager *manager, t_rend_attr *att, int i)
{
	att->perb_distance = manager->rays[i].distance * \
	cos(manager->rays[i].ray_angle - manager->player.rotation_angle);
	att->projec_plane_dis = (WIDTH / 2.0) / tan(radians(FOV) / 2.0);
	att->proj_wall_h = ((double)TILE_SIZE / att->perb_distance) * \
	att->projec_plane_dis;
	att->wall_strip_h = att->proj_wall_h;
	att->wall_top_pix = (HEIGHT / 2.0) - (att->wall_strip_h / 2.0);
	att->wall_bottom_pix = (HEIGHT / 2.0) + (att->wall_strip_h / 2.0);
}

void	normalize_angle(double *ang)
{
	if (*ang > 2 * M_PI)
		*ang -= 2 * M_PI;
	if (*ang < 0)
		*ang += 2 * M_PI;
}
