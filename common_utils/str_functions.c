/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 09:27:42 by osallak           #+#    #+#             */
/*   Updated: 2022/11/06 09:37:59 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../includes/includes.h"

int32_t	ft_strlen(char *s)
{
	int32_t	len;

	len = -1;
	while (++len[s])
		;
	return (len);
}
