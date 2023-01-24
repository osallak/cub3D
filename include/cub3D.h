/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 16:02:45 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/01/24 11:55:02 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <math.h>
# include <limits.h>
# include <stdbool.h>
# include "mlx.h"
# include "get_next_line.h"

# define VALID_CHARS " 10SWEN"
# define PLAYER_CHAR "SWEN"
# define TILE_SIZE 30
# define WALL_STRIP_WIDTH 1.0
# define FOV (60)
# define SCALING_FACTOR ((double)0.3)
# define LINE_LENGTH (30)
# define WIDTH 1080
# define HEIGHT 720
# define R_SPEED 5
# define W_SPEED 5
# define MINIMAP_WIDTH 10
# define MINIMAP_HEIGHT 5
# define MINIMAP_X 10
# define MINIMAP_Y 10
# define NUMBER_OF_RAYS (WIDTH / WALL_STRIP_WIDTH)
typedef struct	s_img_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_img_data;

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
	void		*mlx;
	void		*mlx_window;
	t_img_data	img_data;
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

typedef struct s_ray
{
	double	rayAngle;
    double	wallHitX;
    double	wallHitY;
    double	distance;
    bool	wasHitVertical;
    bool	isRayFacingUp;
    bool	isRayFacingDown;
    bool	isRayFacingLeft;
    bool	isRayFacingRight;
}	t_ray;

typedef struct s_cub_manager
{
	t_map_manager	*map;
	t_mlx_manager	mlx_manager;
	t_player		player;
	t_ray			*rays;
}	t_cub_manager;

char	**ft_split(char *s, char c);
int		ft_atoi(char *str);
void	panic(const char *str);
int		render(t_map_manager *map_manager);
int		create_trgb(int t, int r, int g, int b);
void	cubmlx_pixel_put(t_img_data *data, int x, int y, int color);
size_t	ft_strlcpy(char *dst, char *src, size_t dstsize);
int		draw(t_cub_manager * manager);
#endif