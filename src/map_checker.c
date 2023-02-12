/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 13:45:39 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 13:45:57 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

char	check_map_characters(char **map)
{
	t_quadri_long	tmp;

	tmp.a = -1;
	tmp.c = 0;
	tmp.d = 0;
	while (map[++tmp.a])
	{
		tmp.b = -1;
		while (++tmp.b < ft_strlen(map[tmp.a]))
		{
			if (!ft_strchr(VALID_CHARS, map[tmp.a][tmp.b]))
				panic("Error: invalid map: impostor character");
			else
				check_for_duplicate_pcharacter(&tmp, map);
		}
	}
	if (tmp.c == 0)
		panic("Error: invalid map: player character is missing");
	return ((char) tmp.c);
}
