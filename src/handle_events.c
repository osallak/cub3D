/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 01:53:19 by osallak           #+#    #+#             */
/*   Updated: 2023/02/12 01:58:24 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	controls(int key, t_cub_manager *manager)
{
	if (key == KEY_RIGHT)
		manager->player.rotate = 1;
	else if (key == KEY_LEFT)
		manager->player.rotate = -1;
	else if (key == KEY_W)
	{
		manager->player.walk_direction = 1;
		manager->player.move_y = true;
	}
	if (key == KEY_S)
	{
		manager->player.move_y = true;
		manager->player.walk_direction = -1;
	}
	if (key == KEY_A)
		manager->__move_slideways = -1;
	if (key == KEY_D)
		manager->__move_slideways = 1;
	handle_gun_events(key, manager);
	if (key == 53)
		exit(EXIT_SUCCESS);
	return (0);
}

int	controls_up(int key, t_cub_manager *manager)
{
	if (key == KEY_RIGHT || key == KEY_LEFT)
		manager->player.rotate = 0;
	else if (key == KEY_W || key == KEY_S)
	{
		manager->player.move_y = false;
		manager->player.walk_direction = 0;
	}
	if (key == KEY_A || key == KEY_D)
		manager->__move_slideways = 0;
	if (key == KEY_SPACE)
	{
		manager->weapons.gun_state = STAND;
		manager->weapons.gun_frames = 0;
	}
	return (0);
}

void	handle_gun_events(int key, t_cub_manager *manager)
{
	if (key == KEY_SPACE)
	{
		manager->weapons.gun_state = SHOOT;
		manager->weapons.gun_frames = 3;
	}
	if (key == KEY_R)
	{
		if (manager->weapons.gun_type == SNIPER)
			manager->weapons.gun_type = PISTOL;
		else
			manager->weapons.gun_type = SNIPER;
	}
}

int	__mouse_move(int x, int y, t_cub_manager *manager)
{
	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
		return (EXIT_SUCCESS);
	manager->mouse_move = true;
	if (x < manager->mouse_x)
		controls(KEY_LEFT, manager);
	else if (x > manager->mouse_x)
		controls(KEY_RIGHT, manager);
	manager->mouse_x = x;
	return (0);
}

int	__mouse_press(int button, int x, int y, t_cub_manager *manager)
{
	if (button == 1 && x > 0 && x < WIDTH && y > 0 && y < HEIGHT)
		controls(KEY_SPACE, manager);
	if (button == 2 && x > 0 && x < WIDTH && y > 0 && y < HEIGHT)
		controls(KEY_R, manager);
	return (0);
}
