/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 16:54:43 by yakhoudr          #+#    #+#             */
/*   Updated: 2022/12/05 13:22:43 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

size_t	ft_strlcpy(char *dst, char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize >= 1)
	{
		while (i + 1 < dstsize && src[i])
		{
			dst[i] = src[i];
			i++;
		}
		if (i < dstsize)
			dst[i] = 0;
	}
	return (ft_strlen(src));
}
