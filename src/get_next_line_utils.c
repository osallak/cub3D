/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 18:29:18 by yakhoudr          #+#    #+#             */
/*   Updated: 2022/12/03 16:03:11 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

unsigned int	ft_strlen(char *str)
{
	unsigned int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(char *s)
{
	int		len;
	int		i;
	char	*tmp;
	char	*r;

	if (s)
	{
		i = -1;
		len = ft_strlen(s);
		tmp = malloc(len + 1);
		if (!tmp)
			return (0x0);
		r = tmp;
		while (s[++i])
		{
			*(tmp++) = s[i];
		}
		*tmp = 0;
		return (r);
	}
	return (0x0);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	char	*str;
	char	*r;

	str = 0x0;
	r = 0x0;
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	if (!s1 && !s2)
		return (0x0);
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		exit(EXIT_FAILURE);
	r = str;
	i = -1;
	while (s1[++i])
		*(str++) = s1[i];
	i = -1;
	while (s2[++i])
		*(str++) = s2[i];
	*str = 0;
	return (r);
}

char	*ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (&str[i]);
		i++;
	}
	return (0x0);
}

char	*ft_substr(char *s, unsigned int start, unsigned int end)
{
	char	*str;
	char	*r;

	r = 0x0;
	if (!s || start >= ft_strlen(s))
		return (0x0);
	str = malloc(end - start + 1);
	r = str;
	if (!str)
		return (0x0);
	while (start < end)
	{
		*str = s[start];
		start++;
		str++;
	}
	*str = 0;
	return (r);
}
