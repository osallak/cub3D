/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 16:02:45 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/12 18:50:57 by yakhoudr         ###   ########.fr       */
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
# define WIDTH 960
# define HEIGHT 720
# define NORTH 0
# define SOUTH 1 
# define EAST 2
# define WEST 3
# define DOOR 4
# define MINIMAP_WIDTH 10
# define MINIMAP_HEIGHT 5
# define MINIMAP_X 10
# define MINIMAP_Y 10
# define AIM_SYMBOL_PATH "assets/target.xpm"
# define SHOOTING_PISTOL_PATH "assets/hold.xpm"
# define STANDING_PISTOL_PATH "assets/shoot.xpm" 
# define STANDING_SNIPER_PATH "assets/shooting_sniper.xpm"//TODO: check this
# define SHOOTING_SNIPER_PATH "assets/standing_sniper.xpm"
# define DOOR_PATH "assets/door1.xpm"
# define SHOOT 0
# define STAND 1
# define PISTOL 2
# define SNIPER 0
# define MINI_X 15
# define MINI_Y 15
# define VALID_ID "NSWEFC"

typedef struct s_cub_time
{
	long long	last_tick;
	double		delta_time;
}	t_cub_time;

typedef struct cast_function
{
	double	xintercept;
	double	yintercept;
	double	xstep;
	double	ystep;
	bool	found_horz_wall_hit;
	double	horz_wall_hit_x;
	double	horz_wall_hit_y;
	double	next_horz_touch_x;
	double	next_horz_touch_y;
	bool	found_ver_hit;
	double	ver_hit_x;
	double	ver_hit_y;
	double	next_ver_touch_x;
	double	next_ver_touch_y;
	double	horz_hit_distance;
	double	vert_hit_distance;
}	t_cast_function;

typedef struct cast_helper
{
	double		xintercept;
	double		yintercept;
	double		xstep;
	double		ystep;
	double		next_horz_touch_x;
	double		next_horz_touch_y;
	double		next_vert_touch_x;
	double		next_vert_touch_y;
	double		horz_wall_hit_x;
	double		horz_wall_hit_y;
	double		vert_wall_hit_x;
	double		vert_wall_hit_y;
	double		horz_hit_distance;
	double		vert_hit_distance;
	bool		found_horz_wall_hit;
	bool		found_vert_wall_hit;
}	t_cast_helper;

typedef struct s_img_data {
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
	int				x;
	int				y;
	double			dist;
	struct s_door	*prev;
	struct s_door	*next;
}	t_door;

typedef struct s_render_attr
{
	t_draw_point_struct	p;
	double				off_x;
	double				off_y;
	int					x;
	int					y;
	double				perb_distance;
	double				projec_plane_dis;
	double				proj_wall_h;
	double				wall_strip_h;
	double				wall_top_pix;
	double				wall_bottom_pix;
}	t_rend_attr;

typedef struct s_draw_lines_struct
{
	t_pair_double	start;
	t_pair_double	end;
	t_pair_double	limits;
	int				color;
}	t_draw_lines_struct;

typedef struct s_draw_circle
{
	t_pair_double	center;
	double			radius;
	t_pair_double	limits;
	int				color;
}	t_draw_circle;

typedef struct s_texture
{
	int			wi;
	int			hi;
	void		*img;
	t_img_data	tex_img_data;
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
	double	walk_speed;
	int		turn_direction;
	int		walk_direction;
	double	x;
	double	y;
	bool	move_x;
	bool	move_y;
	int		rotate;
}	t_player;

typedef struct s_ray
{
	double	ray_angle;
	double	wall_hit_x;
	double	wall_hit_y;
	double	distance;
	bool	was_vert_h;
	bool	is_fac_up;
	bool	is_fac_down;
	bool	is_fac_left;
	bool	is_fac_right;
}			t_ray;

typedef struct s_weapons
{
	t_texture	gun[4];
	t_texture	aim_symbol;
	int			gun_frames;
	int			gun_state;
	int			gun_type;
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
	bool			mouse_move;
	int				__move_slideways;
}	t_cub_manager;

char			**ft_split(char *s, char c);
int				ft_atoi(char *str);
void			panic(const char *str);
int				render(t_map_manager *map_manager);
int				create_trgb(int t, int r, int g, int b);
void			cubmlx_pixel_put(t_img_data *data, int x, int y, int color);
size_t			ft_strlcpy(char *dst, char *src, size_t dstsize);
int				draw(t_cub_manager *manager);
void			*xalloc(size_t size);
void			rendering_3d_walls(t_cub_manager *manager);
void			cast_all_rays(t_cub_manager *manager);
double			radians(double angle);
double			dist(int x1, int x2, int y1, int y2);
double			__distance(double x, double y, double x1, double y1);
void			protect_textures(t_cub_manager *manager, bool flag);
void			getting_textures_data(t_cub_manager *manager);
void			decoding_xpm_files(t_cub_manager *manager);
void			protect_gun_textures(t_cub_manager *manager, int flag);
void			__get_gun_data(t_cub_manager *manager);
void			__load_gun_textures(t_cub_manager *manager);
void			load_door_textures(t_cub_manager *manager);
void			init_map(t_cub_manager *manager, t_map_manager *mapm);
int				check_if_cell_has_player(t_cub_manager *manager, int i, int j);
void			init_player(t_cub_manager *manager);
void			init_manager_attr(t_cub_manager *manager);
void			move_player(t_cub_manager *manager);
void			move_player_dda(t_cub_manager *manager, double nx, double ny);
t_pair_double	get_x_y_increment(t_cub_manager *manager, double nx, \
double ny, int *st);
void			update_player_position(t_cub_manager *manager, \
t_pair_double d_b_a, bool flag);
void			move_slideways(t_cub_manager *manager, double *nx, double *ny, \
int sd);
void			rotate_player(t_cub_manager *manager);
void			cast(t_ray *ray, t_cub_manager *manager);
void			choose_ver_intersection(t_ray *ray, t_cast_function *var);
void			choose_horz_intersection(t_ray *ray, t_cast_function *var);
void			choose_closest_intersection(t_ray *ray, t_cast_function *var);
void			calculate_ray_length(t_ray *ray, t_cub_manager *mn, \
t_cast_function *var);
void			check_horizontal_intersection(t_ray *ray, \
t_cub_manager *manager, t_cast_function *var);
void			initialize_horizontal_check(t_ray *ray, \
t_cub_manager *manager, t_cast_function *var);
void			intitialize_caster_var(t_cast_function *var);
bool			__inside_wall_ver(int x, int y, bool isfacingleft, \
t_cub_manager *mn);
bool			__inside_wall(int x, int y, bool isfacingup, t_cub_manager *mn);
long			get_map_width(t_map_manager *map_manager);
long			get_map_height(t_map_manager *map_manager);
void			normalize_angle(double *ang);
int				controls_up(int key, t_cub_manager *manager);
void			check_vertical_intersection(t_ray *ray, t_cub_manager *manager, \
t_cast_function *var);
int				my_strcmp(char *a, char *b);
void			__initialize_ray_attributes(t_ray *ray);
void			check_vertical_intersection(t_ray *ray, t_cub_manager *manager, \
t_cast_function *var);
void			cast_all_rays(t_cub_manager *manager);
int				get_ver_tex(t_cub_manager *manager, t_rend_attr *att, int i);
int				get_hor_tex(t_cub_manager *manager, t_rend_attr *att, int i);
void			render_celling_floor(t_cub_manager *manager, \
t_rend_attr att, int i);
void			rendering_3d_walls(t_cub_manager *manager);
void			__render_gun(t_cub_manager *mn);
void			__render_ceiling(t_cub_manager *manager, int x, int y);
void			__render_floor(t_cub_manager *manager, int x, int y);
void			cub_mlx_pixel_put(t_img_data *data, t_draw_point_struct p);
void			clear_window(t_cub_manager *manager, int color, int lx, int ly);
void			draw_line(t_cub_manager *manager, t_draw_lines_struct lines);
void			render_cell(t_cub_manager *manager, double mapsx, double mapsy, \
int i);
void			draw_minimap(t_cub_manager *manager);
void			draw_player(t_cub_manager *manager);
int				draw(t_cub_manager *manager);
void			paint_wall(t_cub_manager *manager, t_rend_attr *att, int i, \
int tex);
void			put_color_into_wall(t_cub_manager *manager, int x, int y, \
int color);
int				controls(int key, t_cub_manager *manager);
int				controls_up(int key, t_cub_manager *manager);
void			handle_gun_events(int key, t_cub_manager *manager);
int				__mouse_move(int x, int y, t_cub_manager *manager);
int				__mouse_press(int button, int x, int y, t_cub_manager *manager);
void			normalize_angle(double *ang);
int				__destroy(void);
long long		get_time(void);
void			time_utils(t_cub_manager *manager);
long			get_map_height(t_map_manager *map_manager);
void			init_rend_attr(t_rend_attr *att);
void			initialize_vertical_check(t_ray *ray, t_cub_manager *manager, \
t_cast_function *var);
long			get_map_width(t_map_manager *map_manager);
void			*xalloc(size_t size);
void			reset_gun_frames(t_cub_manager *manager);
void			init_mlx(t_cub_manager *manager);
void			calc_wall_att(t_cub_manager *manager, t_rend_attr *att, int i);
void			parse_assets(t_map_manager	*map_manager, \
const int map_fd, char **map_line, long *skip);
unsigned int	get_map_size(const int map_fd, char **map_line);
void			fill_map(char **map, int map_fd, long map_lines);
char			check_map_characters(char **map);
void			check_validity_of_map(char **map, unsigned int map_lines);
void			skip_lines(long *skip, char **map_line, const int map_fd);
void			check_c_asset(char **map_line, t_map_manager *map_manager);
void			check_f_asset(char **map_line, t_map_manager *map_manager);
void			skip_spaces(unsigned int *i, char *map_line);
long			get_end(char **map, int index);
long			get_begin(char **map, int index);
void			_norm_norm_fill_map(char **map, int map_fd, \
	int *index, char **filtered_line);
void			_norm_fill_map(char **map, int map_fd, \
int index, long map_lines);
void			check_valid_space(char **map, long map_lines);
void			check_for_duplicate_pcharacter(t_quadri_long *tmp, char **map);
void			free_uncessary(char ***splitted, char *cmp, char *map_line);
void			get_color_value(char *map_line, unsigned int *i, \
unsigned int *j, char **color_value);
void			panic_rgb(int k);
void			__norm_color(unsigned int *i, int *k, \
unsigned int *j, char *color_str);
void			init_player(t_cub_manager *manager);
bool			is_wall(t_cub_manager *manager, double nx, double ny);
#endif