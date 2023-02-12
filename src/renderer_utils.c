/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 23:32:12 by osallak           #+#    #+#             */
/*   Updated: 2023/02/12 13:44:11 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	render_cell(t_cub_manager *manager, double mapsx, double mapsy, int i)
{
	t_draw_point_struct	p;
	int					j;
	int					x;
	int					y;

	p.point.y = i;
	j = -1;
	while (++j < MINI_X * 10)
	{
		p.point.x = j;
		x = (mapsy + i) / MINI_Y;
		y = (mapsx + j) / MINI_X;
		if (x >= 0 && x < manager->map->map_height && y >= 0 \
		&& y < manager->map->map_width && manager->map->map[x][y] == '1')
			p.color = 0x00ffffff;
		else if (x >= 0 && x < manager->map->map_height && y >= 0 \
		&& y < manager->map->map_width && (manager->map->map[x][y] == 'D' \
		|| manager->map->map[x][y] == 'O'))
			p.color = 0x00ff0000;
		else
			p.color = 0x00000000;
		cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);
	}
}

static t_pair_double	draw_line_helper(t_draw_lines_struct l, int *st)
{
	t_pair_double	dxy;
	double			steps;
	t_pair_double	incs;

	dxy.x = l.end.x - l.start.x;
	dxy.y = l.end.y - l.start.x;
	steps = fmax(fabs(dxy.x), fabs(dxy.y));
	*st = round(steps);
	incs.x = dxy.x / steps;
	incs.y = dxy.y / steps;
	return (incs);
}

void	draw_line(t_cub_manager *manager, t_draw_lines_struct lines)
{
	t_pair_double		incs;
	t_draw_point_struct	coor;
	int					st;
	int					i;

	draw_line_helper(lines, &st);
	coor.point.x = lines.start.x;
	coor.point.y = lines.start.y;
	coor.limits = lines.limits;
	coor.color = lines.color;
	i = -1;
	while (++i <= st)
	{
		cub_mlx_pixel_put(&manager->mlx_manager.img_data, coor);
		coor.point.x += incs.x;
		coor.point.y += incs.y;
	}
}

void	clear_window(t_cub_manager *manager, int color, int lx, int ly)
{
	int					tmp;
	t_draw_point_struct	p;

	p.point.x = 0;
	p.point.y = 0;
	tmp = p.point.x;
	p.color = color;
	p.limits.x = lx;
	p.limits.y = ly;
	while (p.point.y < ly)
	{
		p.point.x = tmp;
		while (p.point.x < lx)
		{
			cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);
			++p.point.x;
		}
		++p.point.y;
	}
}

void	cub_mlx_pixel_put(t_img_data *data, t_draw_point_struct p)
{
	char	*dst;

	if (p.point.x < 0 || p.point.x >= p.limits.x \
	|| p.point.y < 0 || p.point.y >= p.limits.y)
		return ;
	dst = data->addr + ((int)p.point.y * data->\
	line_length + (int)p.point.x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = p.color;
}
