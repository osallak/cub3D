/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:26:42 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/09 02:37: by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"





void	cub_mlx_pixel_put(t_img_data *data, t_draw_point_struct p)
{
	char	*dst;

	if (p.point.x < 0 || p.point.x >= p.limits.x \
	|| p.point.y < 0 || p.point.y >= p.limits.y)
		return ;
	dst = data->addr + ((int)p.point.y * data->\
	line_length + (int)p.point.x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = p.color;
}

long long	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

t_pair_double	draw_line_helper(t_draw_lines_struct l, int *st)
{
	t_pair_double	dxy;
	double			steps;
	t_pair_double	incs;

	dxy.x = l.end.x - l.start.x;
	dxy.y = l.end.y - l.start.x;
	steps = fmax(fabs(dxy.x), fabs(dxy.y));
	*st = round(steps);
	incs.x = dxy.x / steps;
	incs.y = dxy.y / steps;
	return (incs);
}

void	draw_line(t_cub_manager *manager, t_draw_lines_struct lines)
{
	t_pair_double		incs;
	t_draw_point_struct	coor;
	int					st;
	int					i;

	draw_line_helper(lines, &st);
	coor.point.x = lines.start.x;
	coor.point.y = lines.start.y;
	coor.limits = lines.limits;
	coor.color = lines.color;
	i = -1;
	while (++i <= st)
	{
		cub_mlx_pixel_put(&manager->mlx_manager.img_data, coor);
		coor.point.x += incs.x;
		coor.point.y += incs.y;
	}
}

void	normalize_angle(double *ang)
{
	if (*ang > 2 * M_PI)
		*ang -= 2 * M_PI;
	if (*ang < 0)
		*ang += 2 * M_PI;
}

void	clear_window(t_cub_manager *manager, int color, int lx, int ly)
{
	int					tmp;
	t_draw_point_struct	p;

	p.point.x = 0;
	p.point.y = 0;
	tmp = p.point.x;
	p.color = color;
	p.limits.x = lx;
	p.limits.y = ly;
	while (p.point.y < ly)
	{
		p.point.x = tmp;
		while (p.point.x < lx)
		{
			cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);
			++p.point.x;
		}
		++p.point.y;
	}
}

long	get_map_width(t_map_manager *map_manager)
{
	long	res;
	int		i;
	long	line_size;

	res = 0;
	i = 0;
	while (map_manager->map[i])
	{
		line_size = ft_strlen(map_manager->map[i]);
		if (line_size > res)
			res = line_size;
		++i;
	}
	return (res);
}

long	get_map_height(t_map_manager *map_manager)
{
	long	i;

	i = 0;
	while (map_manager->map[i])
		++i;
	return (i);
}

void	handle_gun_events(int key, t_cub_manager *manager)
{
	if (key == KEY_SPACE)
	{
		manager->weapons.gun_state = SHOOT;
		manager->weapons.gun_frames = 3;
	}
	if (key == KEY_R)
	{
		if (manager->weapons.gun_type == SNIPER)
			manager->weapons.gun_type = PISTOL;
		else
			manager->weapons.gun_type = SNIPER;
	}
}

int	controls(int key, t_cub_manager *manager)
{
	if (key == KEY_RIGHT)
		manager->player.rotate = 1;
	else if (key == KEY_LEFT)
		manager->player.rotate = -1;
	else if (key == KEY_W)
	{
		manager->player.walk_direction = 1;
		manager->player.move_y = true;
	}
	if (key == KEY_S)
	{
		manager->player.move_y = true;
		manager->player.walk_direction = -1;
	}
	if (key == KEY_A)
		manager->__move_slideways = -1;
	if (key == KEY_D)
		manager->__move_slideways = 1;
	handle_gun_events(key, manager);
	if (key == 53)
		exit(EXIT_SUCCESS);
	return (0);
}

int	controls_up(int key, t_cub_manager *manager)
{
	if (key == KEY_RIGHT || key == KEY_LEFT)
		manager->player.rotate = 0;
	else if (key == KEY_W || key == KEY_S)
	{
		manager->player.move_y = false;
		manager->player.walk_direction = 0;
	}
	if (key == KEY_A || key == KEY_D)
		manager->__move_slideways = 0;
	if (key == KEY_SPACE)
	{
		manager->weapons.gun_state = STAND;
		manager->weapons.gun_frames = 0;
	}
	return (0);
}

// sd stands for slide direction





void	time_utils(t_cub_manager *manager)
{
	if (manager->time.lastTick == 0)
		manager->time.lastTick = get_time();
	manager->time.delta_time = (get_time() - manager->time.lastTick) / 1000.0;
	manager->time.lastTick = get_time();
}

void	render_cell(t_cub_manager *manager, double mapsx, double mapsy, int i)
{
	t_draw_point_struct	p;
	int					j;
	int					x;
	int					y;

	p.point.y = i;
	j = -1;
	while (++j < mini_x * 10)
	{
		p.point.x = j;
		x = (mapsy + i) / mini_y;
		y = (mapsx + j) / mini_x;
		if (x >= 0 && x < manager->map->map_height && y >= 0 \
		&& y < manager->map->map_width && manager->map->map[x][y] == '1')
			p.color = 0x00ffffff;
		else if (x >= 0 && x < manager->map->map_height && y >= 0 \
		&& y < manager->map->map_width && (manager->map->map[x][y] == 'D' \
		|| manager->map->map[x][y] == 'O'))
			p.color = 0x00ff0000;
		else
			p.color = 0x00000000;
		cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);
	}
}

void	draw_minimap(t_cub_manager *manager)
{
	double	mapsx;
	double	mapsy;
	int		i;

	mapsx = manager->player.x - 5.0 * TILE_SIZE;
	mapsx = mapsx / TILE_SIZE * mini_x;
	mapsy = manager->player.y - 3.0 * TILE_SIZE;
	mapsy = mapsy / TILE_SIZE * mini_y;
	i = -1;
	while (++i < mini_y * 6)
		render_cell(manager, mapsx, mapsy, i);
}

void	draw_player(t_cub_manager *manager)
{
	t_draw_lines_struct	line;
	double				incr;
	int					i;
	double				angle;

	incr = radians(FOV) / 3.0;
	angle = manager->player.rotation_angle - \
		(radians((double)FOV / 2.0));
	line.color = 0x0000ff00;
	line.start.x = round(mini_x * 10 / 2.0);
	line.start.y = round(mini_y * 6 / 2.0);
	i = -1;
	while (++i < 3)
	{
		line.end.x = line.start.x + cos(angle) * mini_x;
		line.end.y = line.start.y + sin(angle) * mini_x;
		draw_line(manager, line);
		angle += incr;
	}
}

int	draw(t_cub_manager *manager)
{
	time_utils(manager);
	move_player(manager);
	cast_all_rays(manager);
	draw_minimap(manager);
	draw_player(manager);
	mlx_put_image_to_window(manager->mlx_manager.mlx, \
	manager->mlx_manager.mlx_window, manager->mlx_manager.img_data.img, 0, 0);
	clear_window(manager, 0x00000000, WIDTH, HEIGHT);
	return (0);
}





void	initialize_vertical_check(t_ray *ray, t_cub_manager *manager, \
t_cast_function *var)
{
	var->xintercept = floor(manager->player.x / TILE_SIZE) * TILE_SIZE;
	if (ray->is_fac_right)
		var->xintercept += TILE_SIZE;
	var->yintercept = manager->player.y + (var->xintercept - \
	manager->player.x) * tan(ray->ray_angle);
	var->xstep = TILE_SIZE;
	if (ray->is_fac_left)
		var->xstep = -var->xstep;
	var->ystep = TILE_SIZE * tan(ray->ray_angle);
	if (ray->is_fac_up && var->ystep > 0)
		var->ystep *= -1;
	if (ray->is_fac_down && var->ystep < 0)
		var->ystep *= -1;
	var->next_ver_touch_x = var->xintercept;
	var->next_ver_touch_y = var->yintercept;
}

void	check_vertical_intersection(t_ray *ray, t_cub_manager *manager, \
t_cast_function *var)
{
	while (var->next_ver_touch_x >= 0 && var->next_ver_touch_x \
	< manager->map->map_width * TILE_SIZE && var->next_ver_touch_y \
	>= 0 && var->next_ver_touch_y < manager->map->map_height * TILE_SIZE)
	{
		if (__inside_wall_ver(var->next_ver_touch_x, \
		var->next_ver_touch_y, ray->is_fac_left, manager))
		{
			var->found_ver_hit = true;
			var->ver_hit_x = var->next_ver_touch_x;
			var->ver_hit_y = var->next_ver_touch_y;
			break ;
		}
		var->next_ver_touch_x += var->xstep;
		var->next_ver_touch_y += var->ystep;
	}
}



void	__initialize_ray_attributes(t_ray *ray)
{
	ray->distance = 0;
	ray->wallHitX = 0;
	ray->wallHitY = 0;
	ray->was_vert_h = false;
	if (ray->ray_angle > 0 && ray->ray_angle < M_PI)
		ray->is_fac_down = true;
	else
		ray->is_fac_down = false;
	ray->is_fac_up = !ray->is_fac_down;
	if (ray->ray_angle < M_PI_2 || ray->ray_angle > 1.5 * M_PI)
		ray->is_fac_right = true;
	else
		ray->is_fac_right = false;
	ray->is_fac_left = !ray->is_fac_right;
}


void	*xalloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		exit(EXIT_FAILURE);
	return (ptr);
}

void	cast_all_rays(t_cub_manager *manager)
{
	double	ray_angle;
	int		i;
	double	angle_increment;

	angle_increment = radians(FOV) / NUMBER_OF_RAYS;
	ray_angle = manager->player.rotation_angle - (radians(FOV) / 2.0);
	i = -1;
	while (++i < NUMBER_OF_RAYS)
	{
		normalize_angle(&ray_angle);
		manager->rays[i].ray_angle = ray_angle;
		__initialize_ray_attributes(&manager->rays[i]);
		cast(&manager->rays[i], manager);
		ray_angle += angle_increment;
	}
	rendering_3d_walls(manager);
}

void	__render_ceiling(t_cub_manager *manager, int x, int wallTopPixel)
{
	t_draw_point_struct	p;
	int					y;

	p.limits.x = WIDTH;
	p.limits.y = HEIGHT;
	p.color = manager->map->c;
	p.point.x = x;
	y = 0;
	while (++y < wallTopPixel)
	{
		p.point.y = y;
		cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);
	}
}

void	__render_floor(t_cub_manager *manager, int x, int y)
{
	t_draw_point_struct	p;

	p.limits.x = WIDTH;
	p.limits.y = HEIGHT;
	p.color = manager->map->f;
	p.point.x = x;
	while (y < HEIGHT)
	{
		p.point.y = y;
		cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);
		y++;
	}
}

void	put_color_into_wall(t_cub_manager *manager, int x, int y, int color)
{
	t_draw_point_struct	p;

	if (color == 0)
		return ;
	p.limits.x = WIDTH;
	p.limits.y = HEIGHT;
	p.color = color;
	p.point.x = x;
	p.point.y = y;
	cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);
}

void	reset_gun_frames(t_cub_manager *manager)
{
	if (manager->weapons.gun_frames == 0)
		manager->weapons.gun_state = STAND;
	else
		manager->weapons.gun_frames--;
}

void	__render_gun(t_cub_manager *mn)
{
	int			x_start;
	int			y_start;
	t_texture	choosen_gun;
	int			x;
	int			y;

	choosen_gun = mn->weapons.gun[mn->weapons.gun_type + mn->weapons.gun_state];
	x_start = WIDTH / 2 - choosen_gun.wi / 2;
	y_start = HEIGHT - choosen_gun.hi;
	y = y_start - 1;
	while (++y < y_start + choosen_gun.hi)
	{
		x = x_start - 1;
		while (++x < x_start + choosen_gun.wi)
			put_color_into_wall(mn, x, y, \
			*(((int *)choosen_gun.tex_img_data.addr + \
			((y - y_start) * choosen_gun.wi + (x - x_start)))));
	}
	reset_gun_frames(mn);
}

void	init_rend_attr(t_rend_attr *att)
{
	att->p.limits.x = WIDTH;
	att->p.limits.y = HEIGHT;
	att->p.color = 0x00ffffff;
	att->off_x = -1;
	att->off_y = -1;
}

void	calc_wall_att(t_cub_manager *manager, t_rend_attr *att, int i)
{
	att->perb_distance = manager->rays[i].distance * \
	cos(manager->rays[i].ray_angle - manager->player.rotation_angle);
	att->projec_plane_dis = (WIDTH / 2.0) / tan(radians(FOV) / 2.0);
	att->proj_wall_h = ((double)TILE_SIZE / att->perb_distance) * \
	att->projec_plane_dis;
	att->wall_strip_h = att->proj_wall_h;
	att->wall_top_pix = (HEIGHT / 2.0) - (att->wall_strip_h / 2.0);
	att->wall_bottom_pix = (HEIGHT / 2.0) + (att->wall_strip_h / 2.0);
}

int	get_ver_tex(t_cub_manager *manager, t_rend_attr *att, int i)
{
	int	tex;

	tex = -1;
	att->off_x = fmod(manager->rays[i].wallHitY, TILE_SIZE);
	att->y = manager->rays[i].wallHitY / TILE_SIZE;
	if (manager->rays[i].is_fac_right)
	{
		att->x = (manager->rays[i].wallHitX + 1.) / TILE_SIZE;
		if (manager->map->map[att->y][att->x] != 'D')
			tex = EAST;
		else
			tex = DOOR;
	}
	else
	{
		att->x = (manager->rays[i].wallHitX - 1.) / TILE_SIZE;
		if (manager->map->map[att->y][att->x] != 'D')
			tex = WEST;
		else
			tex = DOOR;
	}
	return (tex);
}

int	get_hor_tex(t_cub_manager *manager, t_rend_attr *att, int i)
{
	int	tex;

	tex = -1;
	att->x = manager->rays[i].wallHitX / TILE_SIZE;
	if (manager->rays[i].is_fac_down)
	{
		att->y = (manager->rays[i].wallHitY + 1.) / TILE_SIZE;
		if (manager->map->map[att->y][att->x] != 'D')
			tex = SOUTH;
		else
			tex = DOOR;
	}
	else
	{
		att->y = (manager->rays[i].wallHitY - 1.) / TILE_SIZE;
		if (manager->map->map[att->y][att->x] != 'D')
			tex = NORTH;
		else
			tex = DOOR;
	}
	att->off_x = fmod(manager->rays[i].wallHitX, TILE_SIZE);
	return (tex);
}

void	paint_wall(t_cub_manager *manager, t_rend_attr *att, int i, int tex)
{
	int	j;

	j = att->wall_top_pix;
	while (j < att->wall_bottom_pix - 1)
	{
		att->p.point.x = i * WALL_STRIP_WIDTH;
		att->off_y = (j - (int)att->wall_top_pix) / att->wall_strip_h * \
		manager->map->wall_textures[tex].hi;
		att->p.point.y = j;
		att->p.color = *(((int *)manager->map->wall_textures[tex].\
		tex_img_data.addr + ((int)att->off_y * manager->map->\
		wall_textures[tex].wi + (int)att->off_x)));
		cub_mlx_pixel_put(&manager->mlx_manager.img_data, att->p);
		j++;
	}
}

void	render_celling_floor(t_cub_manager *manager, t_rend_attr att, int i)
{
	__render_ceiling(manager, i, att.wall_top_pix);
	__render_floor(manager, i, att.wall_bottom_pix);
}

void	rendering_3d_walls(t_cub_manager *manager)
{
	t_rend_attr	att;
	int			i;
	int			tex;

	init_rend_attr(&att);
	i = -1;
	while (++i < NUMBER_OF_RAYS)
	{
		tex = -1;
		calc_wall_att(manager, &att, i);
		att.x = 0;
		att.y = 0;
		if (manager->rays[i].was_vert_h)
			tex = get_ver_tex(manager, &att, i);
		else
			tex = get_hor_tex(manager, &att, i);
		if (tex != -1)
		{
			att.off_x = att.off_x / TILE_SIZE * \
			(double)manager->map->wall_textures[tex].wi;
			paint_wall(manager, &att, i, tex);
			render_celling_floor(manager, att, i);
		}
	}
	__render_gun(manager);
}

int	__mouse_move(int x, int y, t_cub_manager *manager)
{
	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
		return (EXIT_SUCCESS);
	manager->mouse_move = true;
	if (x < manager->mouse_x)
		controls(KEY_LEFT, manager);
	else if (x > manager->mouse_x)
		controls(KEY_RIGHT, manager);
	manager->mouse_x = x;
	return (0);
}

int	__destroy(void)
{
	exit(EXIT_SUCCESS);
}



int	__mouse_press(int button, int x, int y, t_cub_manager *manager)
{
	if (button == 1 && x > 0 && x < WIDTH && y > 0 && y < HEIGHT)
		controls(KEY_SPACE, manager);
	if (button == 2 && x > 0 && x < WIDTH && y > 0 && y < HEIGHT)
		controls(KEY_R, manager);
	return (0);
}

void	init_mlx(t_cub_manager *manager)
{
	manager->mlx_manager.mlx = mlx_init();
	manager->mlx_manager.mlx_window = \
		mlx_new_window(manager->mlx_manager.mlx, WIDTH, HEIGHT, "cub3D");
	manager->mlx_manager.img_data.img = \
		mlx_new_image(manager->mlx_manager.mlx, WIDTH, HEIGHT);
	manager->mlx_manager.img_data.addr = \
		mlx_get_data_addr(manager->mlx_manager.img_data.img, \
		&manager->mlx_manager.img_data.bits_per_pixel, \
		&manager->mlx_manager.img_data.line_length, \
		&manager->mlx_manager.img_data.endian);
}

int	render(t_map_manager *map_manager)
{
	t_cub_manager	manager;

	init_map(&manager, map_manager);
	init_player(&manager);
	init_mlx(&manager);
	load_door_textures(&manager);
	decoding_xpm_files(&manager);
	__load_gun_textures(&manager);
	init_manager_attr(&manager);
	mlx_loop_hook(manager.mlx_manager.mlx, draw, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, ON_KEYDOWN, 1L << 0, \
	controls, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, ON_KEYUP, 1L << 0, controls_up, \
	&manager);
	mlx_hook(manager.mlx_manager.mlx_window, 6, 1, __mouse_move, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, 17, 0, __destroy, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, 4, 1, __mouse_press, &manager);
	mlx_loop(manager.mlx_manager.mlx);
	return (0);
}
