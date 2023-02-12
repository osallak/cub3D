/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:26:42 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/09 02:37: by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

long	get_map_width(t_map_manager *map_manager)
{
	long	res;
	int		i;
	long	line_size;

	res = 0;
	i = 0;
	while (map_manager->map[i])
	{
		line_size = ft_strlen(map_manager->map[i]);
		if (line_size > res)
			res = line_size;
		++i;
	}
	return (res);
}

void	*xalloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		exit(EXIT_FAILURE);
	return (ptr);
}

void	reset_gun_frames(t_cub_manager *manager)
{
	if (manager->weapons.gun_frames == 0)
		manager->weapons.gun_state = STAND;
	else
		manager->weapons.gun_frames--;
}

void	init_mlx(t_cub_manager *manager)
{
	manager->mlx_manager.mlx = mlx_init();
	manager->mlx_manager.mlx_window = \
		mlx_new_window(manager->mlx_manager.mlx, WIDTH, HEIGHT, "cub3D");
	manager->mlx_manager.img_data.img = \
		mlx_new_image(manager->mlx_manager.mlx, WIDTH, HEIGHT);
	manager->mlx_manager.img_data.addr = \
		mlx_get_data_addr(manager->mlx_manager.img_data.img, \
		&manager->mlx_manager.img_data.bits_per_pixel, \
		&manager->mlx_manager.img_data.line_length, \
		&manager->mlx_manager.img_data.endian);
}

int	render(t_map_manager *map_manager)
{
	t_cub_manager	manager;

	init_map(&manager, map_manager);
	init_player(&manager);
	init_mlx(&manager);
	load_door_textures(&manager);
	decoding_xpm_files(&manager);
	__load_gun_textures(&manager);
	init_manager_attr(&manager);
	mlx_loop_hook(manager.mlx_manager.mlx, draw, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, ON_KEYDOWN, 1L << 0, \
	controls, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, ON_KEYUP, 1L << 0, controls_up, \
	&manager);
	mlx_hook(manager.mlx_manager.mlx_window, 6, 1, __mouse_move, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, 17, 0, __destroy, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, 4, 1, __mouse_press, &manager);
	mlx_loop(manager.mlx_manager.mlx);
	return (0);
}
