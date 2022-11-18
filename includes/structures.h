/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 23:40:55 by osallak           #+#    #+#             */
/*   Updated: 2022/11/14 18:39:53 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STRUCTURES_H
# define __STRUCTURES_H

/**************************************
|     typedefs/structures/defines..   |
+*************************************/

# define OPEN_ERROR -1
# define YES true
#define NO false
#define REQUIRED_LEN 6
#define NOT_NULL (void *)1

typedef unsigned char uint_8;

typedef struct s_point
{
    double  x;
    double  y;
}   t_point;

typedef struct s_vec2
{
    t_point	head_point;
    t_point	tail_point;
    double  magnitude;
}   t_vec2;

typedef struct s_map
{
    char	**map;
    int		map_height;
    int		map_with;
}   t_map;

typedef struct s_textures
{
    int n_texture;
    int s_texture;
    int e_texture;
    int w_texture;
}   t_textures;

typedef struct s_rgb
{
    uint_8 red;
    uint_8 green;
    uint_8 blue;
}   t_rgb;

struct s_colors
{
    t_rgb floor_color;
    t_rgb ceilling_color;
}   t_colors;

#endif