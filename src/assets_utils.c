/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:13:49 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 18:15:36 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	free_uncessary(char ***splitted, char *cmp, char *map_line)
{
	int	i;

	i = -1;
	*splitted = ft_split(map_line, ' ');
	if (my_strcmp((*splitted)[0], cmp))
	{
		while ((*splitted)[++i])
			free((*splitted)[i]);
		free(*splitted);
	}
}
