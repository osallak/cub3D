/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 18:11:55 by yakhoudr          #+#    #+#             */
/*   Updated: 2022/12/05 13:23:20 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static	void	ft_clear_table(char **ptr, int index)
{
	int	i;

	i = 0;
	while (i <= index)
	{
		free(ptr[i]);
		ptr[i] = 0;
		i++;
	}
	free(ptr);
	ptr = 0;
}

static unsigned int	count_rows(const char *s, char c)
{
	unsigned int	i;
	unsigned int	flag;
	unsigned int	word_count;

	flag = 0;
	word_count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && flag == 0)
		{
			word_count++;
			flag = 1;
		}
		else if (s[i] == c)
			flag = 0;
		i++;
	}
	return (word_count);
}

static int	ft_check_table(char **ptr, int start, int end, int index)
{
	int	exit_code;

	exit_code = 1;
	ptr[index] = malloc((sizeof(char) * (end - start)) + 1);
	if (!ptr[index])
	{
		exit_code = 0;
		ft_clear_table(ptr, index);
	}
	return (exit_code);
}

static void	ft_complete(char *s, char	**splitted, char c)
{
	size_t	i;
	int		index;
	int		j;

	i = -1;
	j = 0;
	index = -1;
	while (++i <= ft_strlen(s))
	{
		if (s[i] != c && index < 0)
			index = i;
		else if ((s[i] == c || i == ft_strlen(s)) && index >= 0)
		{
			if (!ft_check_table(splitted, index, i, j))
				break ;
			ft_strlcpy(splitted[j++], &s[index], (i - index) + 1);
			index = -1;
		}
	}
	splitted[j] = 0x0;
}

char	**ft_split(char *s, char c)
{
	char			**splitted;

	if (!s)
		return (0x0);
	splitted = malloc((sizeof(char *) * (count_rows(s, c) + 1)));
	if (!splitted)
		return (0x0);
	ft_complete(s, splitted, c);
	return (splitted);
}
