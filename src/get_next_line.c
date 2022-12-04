/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 18:29:02 by yakhoudr          #+#    #+#             */
/*   Updated: 2021/12/09 15:54:22 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_append(char **s, char **line)
{
	int		len;
	char	*tmp;

	len = 0;
	while ((*s)[len] != '\n' && (*s)[len] != '\0')
		len++;
	if ((*s)[len] == '\n')
	{
		*line = ft_substr(*s, 0, len + 1);
		tmp = ft_strdup(&(*s)[len + 1]);
		free(*s);
		*s = tmp;
		if ((*s)[0] == 0)
		{
			free(*s);
			*s = 0x0;
		}
	}
	else
	{
		*line = ft_strdup(*s);
		free(*s);
		*s = 0x0;
	}
	return (*line);
}

static char	*ft_line(char **s, char **line, int ret, int fd)
{
	if (ret <= 0 && s[fd] == NULL)
		return (0x0);
	else
		return (ft_append(&s[fd], line));
}

char	*get_next_line(int fd)
{
	int			ret;
	static char	*s[10240];
	char		buf[BUFFER_SIZE + 1];
	char		*tmp;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0x0);
	ret = read(fd, buf, BUFFER_SIZE);
	while (ret > 0)
	{
		buf[ret] = 0;
		if (s[fd] == 0x0)
			s[fd] = ft_strdup(buf);
		else
		{
			tmp = ft_strjoin(s[fd], buf);
			free(s[fd]);
			s[fd] = tmp;
		}
		if (ft_strchr(s[fd], '\n'))
			break ;
		ret = read(fd, buf, BUFFER_SIZE);
	}
	return (ft_line(s, &line, ret, fd));
}
