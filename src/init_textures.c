/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 19:00:52 by osallak           #+#    #+#             */
/*   Updated: 2023/02/12 19:16:05 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	protect_textures(t_cub_manager *manager, bool flag)
{
	int	i;

	i = -1;
	while (++i <= DOOR)
	{
		if (flag && manager->map->wall_textures[i].img == NULL)
			panic("Error: while decoding textures");
		if (!flag && manager->map->wall_textures[i].tex_img_data.addr == NULL)
			panic("Error\ncannot get texture data");
	}
}

void	getting_textures_data(t_cub_manager *manager)
{
	int	i;

	i = -1;
	while (++i <= DOOR)
	{
		manager->map->wall_textures[i].tex_img_data.addr = \
		mlx_get_data_addr(manager->map->wall_textures[i].img, \
		&manager->map->wall_textures[i].tex_img_data.bits_per_pixel, \
		&manager->map->wall_textures[i].tex_img_data.line_length, \
		&manager->map->wall_textures[i].tex_img_data.endian);
	}
	protect_textures(manager, false);
}

void	decoding_xpm_files(t_cub_manager *manager)
{
	manager->map->wall_textures[NORTH].img = mlx_xpm_file_to_image \
	(manager->mlx_manager.mlx, manager->map->no, &manager->map->\
	wall_textures[NORTH].wi, &manager->map->wall_textures[NORTH].hi);
	manager->map->wall_textures[SOUTH].img = mlx_xpm_file_to_image \
	(manager->mlx_manager.mlx, manager->map->so, &manager->map->\
	wall_textures[SOUTH].wi, &manager->map->wall_textures[SOUTH].hi);
	manager->map->wall_textures[EAST].img = mlx_xpm_file_to_image \
	(manager->mlx_manager.mlx, manager->map->ea, &manager->map->\
	wall_textures[EAST].wi, &manager->map->wall_textures[EAST].hi);
	manager->map->wall_textures[WEST].img = mlx_xpm_file_to_image \
	(manager->mlx_manager.mlx, manager->map->we, &manager->map->\
	wall_textures[WEST].wi, &manager->map->wall_textures[WEST].hi);
	protect_textures(manager, true);
	getting_textures_data(manager);
}

void	load_door_textures(t_cub_manager *manager)
{
	manager->map->wall_textures[DOOR].img = mlx_xpm_file_to_image \
	(manager->mlx_manager.mlx, DOOR_PATH, &manager->map-> \
	wall_textures[DOOR].wi, &manager->map->wall_textures[DOOR].hi);
	manager->map->wall_textures[DOOR].tex_img_data.addr = mlx_get_data_addr \
	(manager->map->wall_textures[DOOR].img, &manager->map->wall_textures \
	[DOOR].tex_img_data.bits_per_pixel, &manager->map->wall_textures[DOOR] \
	.tex_img_data.line_length, &manager->map->wall_textures[DOOR]. \
	tex_img_data.endian);
}
