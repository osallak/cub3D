/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 16:02:29 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 19:06:38 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	parse_map_file(int map_fd, char *file);

int	main(int ac, char **av)
{
	int		map_fd;
	int		len;

	if (ac == 2)
	{
		len = ft_strlen(av[1]);
		if (len < 5)
			panic("invalid map file");
		--len;
		if (!(av[1][len] == 'b' && av[1][len - 1] == \
		'u' && av[1][len - 2] == 'c' && av[1][len - 3] == '.'))
			panic("invalid map file");
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
