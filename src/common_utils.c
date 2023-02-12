/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 02:03:36 by osallak           #+#    #+#             */
/*   Updated: 2023/02/12 13:41:19 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	__destroy(void)
{
	exit(EXIT_SUCCESS);
}

long long	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

void	time_utils(t_cub_manager *manager)
{
	if (manager->time.last_tick == 0)
		manager->time.last_tick = get_time();
	manager->time.delta_time = (get_time() - manager->time.last_tick) / 1000.0;
	manager->time.last_tick = get_time();
}

long	get_map_height(t_map_manager *map_manager)
{
	long	i;

	i = 0;
	while (map_manager->map[i])
		++i;
	return (i);
}

void	init_rend_attr(t_rend_attr *att)
{
	att->p.limits.x = WIDTH;
	att->p.limits.y = HEIGHT;
	att->p.color = 0x00ffffff;
	att->off_x = -1;
	att->off_y = -1;
}
