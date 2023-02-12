/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 22:49:56 by osallak           #+#    #+#             */
/*   Updated: 2023/02/11 22:53:02 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	render_celling_floor(t_cub_manager *manager, t_rend_attr att, int i)
{
	__render_ceiling(manager, i, att.wall_top_pix);
	__render_floor(manager, i, att.wall_bottom_pix);
}

void	rendering_3d_walls(t_cub_manager *manager)
{
	t_rend_attr	att;
	int			i;
	int			tex;

	init_rend_attr(&att);
	i = -1;
	while (++i < NUMBER_OF_RAYS)
	{
		tex = -1;
		calc_wall_att(manager, &att, i);
		att.x = 0;
		att.y = 0;
		if (manager->rays[i].was_vert_h)
			tex = get_ver_tex(manager, &att, i);
		else
			tex = get_hor_tex(manager, &att, i);
		if (tex != -1)
		{
			att.off_x = att.off_x / TILE_SIZE * \
			(double)manager->map->wall_textures[tex].wi;
			paint_wall(manager, &att, i, tex);
			render_celling_floor(manager, att, i);
		}
	}
	__render_gun(manager);
}

void	__render_gun(t_cub_manager *mn)
{
	int			x_start;
	int			y_start;
	t_texture	choosen_gun;
	int			x;
	int			y;

	choosen_gun = mn->weapons.gun[mn->weapons.gun_type + mn->weapons.gun_state];
	x_start = WIDTH / 2 - choosen_gun.wi / 2;
	y_start = HEIGHT - choosen_gun.hi;
	y = y_start - 1;
	while (++y < y_start + choosen_gun.hi)
	{
		x = x_start - 1;
		while (++x < x_start + choosen_gun.wi)
			put_color_into_wall(mn, x, y, \
			*(((int *)choosen_gun.tex_img_data.addr + \
			((y - y_start) * choosen_gun.wi + (x - x_start)))));
	}
	reset_gun_frames(mn);
}

void	__render_ceiling(t_cub_manager *manager, int x, int wallTopPixel)
{
	t_draw_point_struct	p;
	int					y;

	p.limits.x = WIDTH;
	p.limits.y = HEIGHT;
	p.color = manager->map->c;
	p.point.x = x;
	y = 0;
	while (++y < wallTopPixel)
	{
		p.point.y = y;
		cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);
	}
}

void	__render_floor(t_cub_manager *manager, int x, int y)
{
	t_draw_point_struct	p;

	p.limits.x = WIDTH;
	p.limits.y = HEIGHT;
	p.color = manager->map->f;
	p.point.x = x;
	while (y < HEIGHT)
	{
		p.point.y = y;
		cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);
		y++;
	}
}
