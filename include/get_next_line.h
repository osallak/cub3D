/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 18:29:09 by yakhoudr          #+#    #+#             */
/*   Updated: 2022/12/03 10:06:11 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 69
# endif
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>

char			*get_next_line(int fd);
unsigned int	ft_strlen(char *str);
char			*ft_strdup(char *s);
char			*ft_strjoin(char *s1, char *s2);
char			*ft_strchr(char *str, char c);
char			*ft_substr(char *s, unsigned int start, unsigned int end);
#endif
