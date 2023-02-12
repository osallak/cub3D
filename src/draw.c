/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 23:47:41 by osallak           #+#    #+#             */
/*   Updated: 2023/02/11 23:49:21 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	draw_minimap(t_cub_manager *manager)
{
	double	mapsx;
	double	mapsy;
	int		i;

	mapsx = manager->player.x - 5.0 * TILE_SIZE;
	mapsx = mapsx / TILE_SIZE * mini_x;
	mapsy = manager->player.y - 3.0 * TILE_SIZE;
	mapsy = mapsy / TILE_SIZE * mini_y;
	i = -1;
	while (++i < mini_y * 6)
		render_cell(manager, mapsx, mapsy, i);
}

void	draw_player(t_cub_manager *manager)
{
	t_draw_lines_struct	line;
	double				incr;
	int					i;
	double				angle;

	incr = radians(FOV) / 3.0;
	angle = manager->player.rotation_angle - \
		(radians((double)FOV / 2.0));
	line.color = 0x0000ff00;
	line.start.x = round(mini_x * 10 / 2.0);
	line.start.y = round(mini_y * 6 / 2.0);
	i = -1;
	while (++i < 3)
	{
		line.end.x = line.start.x + cos(angle) * mini_x;
		line.end.y = line.start.y + sin(angle) * mini_x;
		draw_line(manager, line);
		angle += incr;
	}
}

int	draw(t_cub_manager *manager)
{
	time_utils(manager);
	move_player(manager);
	cast_all_rays(manager);
	draw_minimap(manager);
	draw_player(manager);
	mlx_put_image_to_window(manager->mlx_manager.mlx, \
	manager->mlx_manager.mlx_window, manager->mlx_manager.img_data.img, 0, 0);
	clear_window(manager, 0x00000000, WIDTH, HEIGHT);
	return (0);
}

void	paint_wall(t_cub_manager *manager, t_rend_attr *att, int i, int tex)
{
	int	j;

	j = att->wall_top_pix;
	while (j < att->wall_bottom_pix - 1)
	{
		att->p.point.x = i * WALL_STRIP_WIDTH;
		att->off_y = (j - (int)att->wall_top_pix) / att->wall_strip_h * \
		manager->map->wall_textures[tex].hi;
		att->p.point.y = j;
		att->p.color = *(((int *)manager->map->wall_textures[tex].\
		tex_img_data.addr + ((int)att->off_y * manager->map->\
		wall_textures[tex].wi + (int)att->off_x)));
		cub_mlx_pixel_put(&manager->mlx_manager.img_data, att->p);
		j++;
	}
}

void	put_color_into_wall(t_cub_manager *manager, int x, int y, int color)
{
	t_draw_point_struct	p;

	if (color == 0)
		return ;
	p.limits.x = WIDTH;
	p.limits.y = HEIGHT;
	p.color = color;
	p.point.x = x;
	p.point.y = y;
	cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);
}
