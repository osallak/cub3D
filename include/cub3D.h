/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 16:02:45 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/01/08 11:46:10 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <math.h>
# include "mlx.h"
# include "get_next_line.h"

# define VALID_CHARS " 10SWEN"
# define PLAYER_CHAR "SWEN"
# define TILE_SIZE 30
# define LEN_SINGLE_RAY 1
# define FOV 60

size_t	ft_strlcpy(char *dst, char *src, size_t dstsize);

typedef struct s_quadri_long
{
	long	end;
	long	left;
	long	right;
	long	bbegin;
}	t_quadri_long;

enum {
	ON_KEYDOWN = 2,
	ON_KEYUP = 3,
	ON_MOUSEDOWN = 4,
	ON_MOUSEUP = 5,
	ON_MOUSEMOVE = 6,
	ON_EXPOSE = 12,
	ON_DESTROY = 17
};

typedef struct s_map_manager
{
	char		*no;
	char		*so;
	char		*we;
	char		*ea;
	long		f;
	long		c;
	char		c_player;
	char		**map;
	int			wall_colors;
	int			map_width;
	int			map_height;
}	t_map_manager;

typedef struct s_mlx_manager
{
	void	*mlx;
	void	*mlx_window;
}t_mlx_manager;

typedef struct s_player
{
	double	rotation_angle;
	double	rotation_speed;
	double		walk_speed;
	int		turn_direction;
	int		walk_direction;
	double	x;
	double	y;
}	t_player;

typedef struct s_cub_manager
{
	t_map_manager	*map;
	t_player		player;
	t_mlx_manager	mlx_manager;
}	t_cub_manager;

char	**ft_split(char *s, char c);
int		ft_atoi(char *str);
void	panic(const char *str);
void	render(t_map_manager *map_manager);
int		create_trgb(int t, int r, int g, int b);
#endif