/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 23:35:38 by osallak           #+#    #+#             */
/*   Updated: 2022/11/14 18:29:36 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"
#include "../includes/structures.h"

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i++])
		;
	return (i);
}

static bool	__check_filename(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	return (len > 4 && filename[len - 1] == 'b' && filename[len - 2] == 'u'
		&& filename[len - 3] == 'c' && filename[len - 4] == '.');	
}

static void __error_check(void *ptr)
{
	if (ptr == NULL)
	{
		perror(NULL);
		exit(EXIT_FAILURE);
	}
}


char	*__join_buffer(char *s1, char *s2)
{
	char	*joined;
	int		i;

	joined = (char *)malloc(sizeof(char) * ft_strlen(s1) + 2);
	i = 0;
	while (s1[i])
	{
		joined[i] = s1[i];
		i++;
	}
	joined[i++] = *s2;
	joined[i] = '\0';
	free(s1);
	return (joined);
}

static char	*__readline(int fd)
{
	char	*line;
	char	buff;
	int		bytes;

	bytes = 0;
	line = (char *)malloc(1);
	__error_check(line);
	*line = 0;
	while (true)
	{
		bytes = read(fd, &buff, 1);
		if (bytes == -1)
		{
			free(line);
			__error_check(NULL);
		}
		if (bytes == 0)
			break ;
		line = __join_buffer(line, &buff);
		if (buff == '\n')
			break ;
	}
	if (*line == '\0'){
		free (line);
		return (NULL);
	}
	return (line);
}

static char	*__getline(int fd, bool __flag)
{
	char *line;

	line = __readline(fd);
	if (!line)
		return (NULL);
	if (__flag == true)
	{
		free(line);
		return (NOT_NULL);
	}
	return (line);
}

/*memory leaks below*/
static size_t	__count_file_lines(int fd)
{
	if (__getline(fd, true) == 0x0)
		return (0);
	return (1 + __count_file_lines(fd));
}

char**	read_file(char *filename)
{
	int		fd;
	char	*line;
	char	**ret;
	int		i;

	if (!__check_filename(filename))
	{
		write(2, "Error\nInvalid Filename\n", 22);
		exit(0);
	}
	fd = open(filename, O_RDONLY);
	if (fd == OPEN_ERROR)
	{
		perror(0x0);
		exit(EXIT_FAILURE);
	}
	int len = __count_file_lines(fd);
	ret = malloc(sizeof(char *) * len + 1);
	close(fd);
	fd = open(filename, O_RDONLY);
	if (fd == OPEN_ERROR)
	{
		perror(0x0);
		exit(EXIT_FAILURE);
	}
	i = 0;
	line = NULL;
	while (true)
	{
		line = __getline(fd, false);
		// lseek(fd, 0, 0);
		if (!line)
			break ;
		ret[i] = line;
		i++;
	}
	ret[i] = NULL;
	return (ret);
}
