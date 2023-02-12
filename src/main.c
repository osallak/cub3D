/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 16:02:29 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 02:20:24 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	parse_map_file(int map_fd, char *file);

int	main(int ac, char **av)
{
	int	map_fd;

	if (ac == 2)
	{	
		map_fd = open(av[1], O_RDONLY);
		if (map_fd == -1)
		{
			perror("cub3D");
			return (EXIT_FAILURE);
		}
		parse_map_file(map_fd, av[1]);
	}
	else
	{
		printf("usage: cub3D map\n");
	}
	return (EXIT_SUCCESS);
}
