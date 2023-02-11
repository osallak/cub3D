/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_gun_tex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 19:06:13 by osallak           #+#    #+#             */
/*   Updated: 2023/02/11 19:06:29 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	protect_gun_textures(t_cub_manager *manager, int flag)
{
	int	i;

	i = -1;
	while (++i < 4)
	{
		if (flag && manager->weapons.gun[i].img == NULL)
			panic("Error: while decoding gun textures\n");
		if (!flag && manager->weapons.gun[i].tex_img_data.addr == NULL)
			panic("Error\ncannot get gun texture data\n");
	}
}

void	__get_gun_data(t_cub_manager *manager)
{
	manager->weapons.gun[PISTOL].tex_img_data.addr = mlx_get_data_addr \
	(manager->weapons.gun[PISTOL].img, &manager->weapons.gun[PISTOL]. \
	tex_img_data.bits_per_pixel, &manager->weapons.gun[PISTOL]. \
	tex_img_data.line_length, &manager->weapons.gun[PISTOL]. \
	tex_img_data.endian);
	manager->weapons.gun[SNIPER].tex_img_data.addr = mlx_get_data_addr \
	(manager->weapons.gun[SNIPER].img, &manager->weapons.gun[SNIPER]. \
	tex_img_data.bits_per_pixel, &manager->weapons.gun[SNIPER]. \
	tex_img_data.line_length, &manager->weapons.gun[SNIPER]. \
	tex_img_data.endian);
	manager->weapons.gun[PISTOL + 1].tex_img_data.addr = mlx_get_data_addr \
	(manager->weapons.gun[PISTOL + 1].img, &manager->weapons.gun[PISTOL + 1] \
	.tex_img_data.bits_per_pixel, &manager->weapons.gun[PISTOL + 1]. \
	tex_img_data.line_length, &manager->weapons.gun[PISTOL + 1]. \
	tex_img_data.endian);
	manager->weapons.gun[SNIPER + 1].tex_img_data.addr = mlx_get_data_addr \
	(manager->weapons.gun[SNIPER + 1].img, &manager->weapons.gun[SNIPER + 1]. \
	tex_img_data.bits_per_pixel, &manager->weapons.gun[SNIPER + 1]. \
	tex_img_data.line_length, &manager->weapons.gun[SNIPER + 1]. \
	tex_img_data.endian);
	protect_gun_textures(manager, false);
}

void	__load_gun_textures(t_cub_manager *manager)
{
	manager->weapons.gun[PISTOL].img = mlx_xpm_file_to_image \
	(manager->mlx_manager.mlx, STANDING_PISTOL_PATH, \
	&manager->weapons.gun[PISTOL].wi, &manager->weapons. \
	gun[PISTOL].hi);
	manager->weapons.gun[SNIPER].img = mlx_xpm_file_to_image \
	(manager->mlx_manager.mlx, STANDING_SNIPER_PATH, \
	&manager->weapons.gun[SNIPER].wi, &manager->weapons. \
	gun[SNIPER].hi);
	manager->weapons.gun[PISTOL + 1].img = mlx_xpm_file_to_image \
	(manager->mlx_manager.mlx, SHOOTING_PISTOL_PATH, &manager-> \
	weapons.gun[PISTOL + 1].wi, &manager->weapons.gun[PISTOL + 1].hi);
	manager->weapons.gun[SNIPER + 1].img = mlx_xpm_file_to_image \
	(manager->mlx_manager.mlx, SHOOTING_SNIPER_PATH, &manager-> \
	weapons.gun[SNIPER + 1].wi, &manager->weapons.gun[SNIPER + 1].hi);
	protect_gun_textures(manager, true);
	__get_gun_data(manager);
}
