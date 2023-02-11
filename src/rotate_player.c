/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 19:58:30 by osallak           #+#    #+#             */
/*   Updated: 2023/02/11 19:59:09 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	rotate_player(t_cub_manager *manager)
{
	if (manager->player.rotate)
	{
		manager->player.rotation_angle += manager->player.rotate * \
		manager->player.rotation_speed * manager->time.delta_time;
		normalize_angle(&manager->player.rotation_angle);
	}
	if (manager->mouse_move)
	{
		controls_up(KEY_LEFT, manager);
		manager->mouse_move = false;
	}
}
