/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 18:56:04 by osallak           #+#    #+#             */
/*   Updated: 2023/02/11 18:57:31 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

double	radians(double angle)
{
	return (angle * (M_PI / (double)180));
}

double	dist(int x1, int x2, int y1, int y2)
{
	return (sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0));
}

double	__distance(double x, double y, double x1, double y1)
{
	return (sqrt((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y)));
}

