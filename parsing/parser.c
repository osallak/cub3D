/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 08:15:37 by osallak           #+#    #+#             */
/*   Updated: 2022/11/11 01:15:09 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"
#include "../includes/structures.h"

# define  DS int

bool	__check_length(char **file)
{
	int	count;

	count = 0;
	while (file[count++])
		;
	
	return (count >= REQUIRED_LEN);
}


bool __search(char target, char *line)
{
	while (line)
	{
		if (*line == target)
			return (true);
		line++;
	}
	return (false);
}

//__start the check after the configuration files
bool	__check_elements(char **file)
{
	int	i;
	int	j;

	i = 0;
	while (file[i] != 0x0)
	{
		j = 0;
		while (file[i][j] != 0)
		{
			if (!__search(file[i][j], "01NEWS \n"))
			{
				//print error message
				//free(file);
				return (false);
			}
		}
	}
	return true;	
}

bool	__surrounded_by_walls(char **file)
{

	return true;
}

DS  parser(char **file)
{
	if (!__check_length(file) || !__check_elements(file)!__surrounded_by_wals(file))
		return (-1);
	return 1;
}
