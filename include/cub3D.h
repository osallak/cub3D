/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 16:02:45 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/08 17:10:10 by osallak          ###   ########.fr       */
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
# include <sys/time.h>

# define VALID_CHARS " 10SWEND"
# define DOOR_CHAR 'D'
# define PLAYER_CHAR "SWEN"
# define TILE_SIZE 30
# define WALL_STRIP_WIDTH 1.0
# define FOV (60)
# define SCALING_FACTOR ((double)0.3)
# define LINE_LENGTH (30)
# define WIDTH 1080
# define NORTH 0
# define SOUTH 1 
# define EAST 2
# define WEST 3
# define DOOR 4
# define HEIGHT 720
# define R_SPEED (TILE_SIZE * 2)
# define W_SPEED (TILE_SIZE * 2)
# define MINIMAP_WIDTH 10
# define MINIMAP_HEIGHT 5
# define MINIMAP_X 10
# define MINIMAP_Y 10
# define NUMBER_OF_RAYS (WIDTH / WALL_STRIP_WIDTH)
# define NUMBER_OF_RAYS (WIDTH / WALL_STRIP_WIDTH)
# define AIM_SYMBOL_PATH "assets/target.xpm"
# define SHOOTING_PISTOL_PATH "assets/hold.xpm"
# define STANDING_PISTOL_PATH "assets/shoot.xpm" 
# define STANDING_SNIPER_PATH "assets/shooting_sniper.xpm"//TODO: check this
# define SHOOTING_SNIPER_PATH "assets/standing_sniper.xpm"
# define SHOOT 0
# define STAND 1
#define PISTOL 2
#define SNIPER 0
# define mini_x 20
# define mini_y 15
# define VALID_ID "NSWEFC"

typedef struct s_cub_time
{
	long long 	lastTick;
	double 		delta_time;
}	t_cub_time;

typedef struct	s_img_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_img_data;

typedef struct s_quadri_long
{
	long	a;
	long	b;
	long	c;
	long	d;
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

enum {
	KEY_A = 0,
	KEY_S = 1,
	KEY_D = 2,
	KEY_W = 13,
	KEY_ESC = 53,
	KEY_LEFT = 123,
	KEY_RIGHT = 124,
	KEY_UP = 126,
	KEY_DOWN = 125,
	KEY_SPACE = 49,
	KEY_R = 15,
};

typedef struct s_pair_double
{
	double	x;
	double	y;
}	t_pair_double;

typedef struct s_draw_point_struct
{
	t_pair_double	point;
	t_pair_double	limits;
	int				color;
}	t_draw_point_struct;

typedef struct s_door
{
	int x;
	int y;	
	double dist;
	struct s_door *prev;
	struct s_door *next;
}	t_door;

typedef struct s_draw_lines_struct
{
	t_pair_double	start;
	t_pair_double	end;
	t_pair_double	limits;
	int				color;
}	t_draw_lines_struct;

typedef struct s_draw_circle
{
	t_pair_double   center;
    double        	radius;
	t_pair_double	limits;
	int				color;
}	t_draw_circle;

typedef struct s_texture
{
	int wi;
	int hi;
	void *img;
	t_img_data tex_img_data;
}	t_texture;

typedef struct s_map_manager
{
	char		*no;
	char		*so;
	char		*we;
	char		*ea;
	int			f;
	int			c;
	char		c_player;
	char		**map;
	int			map_width;
	int			map_height;
	t_texture	wall_textures[5];
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
	bool move_x;
	bool move_y;
	int rotate;
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

typedef struct s_weapons
{
	t_texture	gun[4];
	t_texture	aim_symbol;
	int			gun_frames;
	int			gun_state;
	int			gun_type;//this can be PISTOL or SNSIPER
}	t_weapons;

typedef struct s_cub_manager
{
	t_map_manager	*map;
	t_mlx_manager	mlx_manager;
	t_player		player;
	t_ray			*rays;
	t_cub_time		time;
	t_door			*head;
	t_door			*next;
	t_door			*tail;
	t_door			*door;
	t_weapons		weapons;
	int				mouse_x;
	bool			mouse_move;//set this to false 
	bool			__move_slideways;//if this is true do not change the rotation angle
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