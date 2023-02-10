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

double	radians(double angle)
{
	return (angle * (M_PI / (double)180));
}

inline double	dist(int x1, int x2, int y1, int y2)
{
	return (sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0));
}

void	cub_mlx_pixel_put(t_img_data *data, t_draw_point_struct p)
{
	char	*dst;

	if (p.point.x < 0 || p.point.x >= p.limits.x \
	|| p.point.y < 0 || p.point.y >= p.limits.y)
		return ;
	dst = data->addr + ((int)p.point.y * data->line_length \
		+ (int)p.point.x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = p.color;
}

long long get_time(void)
{
	struct timeval t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

void draw_line(t_cub_manager *manager, t_draw_lines_struct lines)
{
  double dx = lines.end.x - lines.start.x;
  double dy = lines.end.y - lines.start.y;

  double steps = fmax(fabs(dx), fabs(dy));
  int st = round(steps);
  double xIncrement = dx / (steps);
  double yIncrement = dy / (steps);
  t_draw_point_struct coor;
  coor.point.x = lines.start.x;
  coor.point.y = lines.start.y;
  coor.limits = lines.limits;
  coor.color = lines.color;
  for (int i = 0; i <= st; ++i) {
    cub_mlx_pixel_put(&manager->mlx_manager.img_data, coor);
    coor.point.x += xIncrement;
    coor.point.y += yIncrement;
  }
}

void normalize_angle(double *ang)
{
	if (*ang > 2 * M_PI)
		*ang -= 2 * M_PI;
	if (*ang < 0)
		*ang += 2 * M_PI;
}

void clear_window(t_cub_manager * manager, int color, int lx, int ly)
{
	int 				tmp;
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

int controls(int key, t_cub_manager	*manager)
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
	if (key == 53)
		exit(EXIT_SUCCESS);
	return 0;
}

int controls_up(int key, t_cub_manager	*manager)
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
	return 0;
}

//sd stands for slide direction
void	move_slideways(t_cub_manager* manager, double *nx, double *ny, int sd)
{
	double	rotation_angle;

	normalize_angle(&rotation_angle);
	rotation_angle = manager->player.rotation_angle\
	+ radians(90) * sd;
	normalize_angle(&rotation_angle);
	*nx = manager->player.x + cos(rotation_angle )\
	* manager->player.walk_speed * manager->time.delta_time ;
	*ny = manager->player.y + sin(rotation_angle)\
	* manager->player.walk_speed * manager->time.delta_time ;
}

void update_player_position(t_cub_manager* manager, t_pair_double d_b_a, bool flag)
{
	if (!flag)
		return ;
	manager->player.y = d_b_a.y;
	manager->player.x = d_b_a.x;
}

t_pair_double	get_x_y_increment(t_cub_manager* manager, double nx,\
double ny, int *st)
{
	t_pair_double	increment_xy;
	t_pair_double	dxy;
	double 			steps;

	dxy.x = nx - manager->player.x;
	dxy.y = ny - manager->player.y;
	steps = fmax(fabs(dxy.x), fabs(dxy.y));
	*st = round(steps);
	increment_xy.x = dxy.x / (steps);
	increment_xy.y = dxy.y / (steps);
	return (increment_xy);
}

void	move_player_dda(t_cub_manager* manager, double nx, double ny)
{
	t_pair_double d_b_a;
	bool increment;
	t_pair_double increment_xy;
	int 	st;
	int		i;

	d_b_a.x = nx;
	d_b_a.y = ny;
	increment = true;
	increment_xy = get_x_y_increment(manager, nx, ny, &st);
	i = -1;
  	while (++i < st)
	{
		if (nx / TILE_SIZE >= 0 && nx / TILE_SIZE < manager->map->map_width\
		&& ny / TILE_SIZE >= 0 && ny / TILE_SIZE < manager->map->map_height\
		&& manager->map->map[(int)ny / TILE_SIZE][(int)nx / TILE_SIZE] == '1')
			increment = false;
		nx += increment_xy.x;
		ny += increment_xy.y;
	}
	update_player_position(manager, d_b_a, increment);
}

void rotate_player(t_cub_manager *manager)
{
	if (manager->player.rotate)
	{
		manager->player.rotation_angle += manager->player.rotate\
		* manager->player.rotation_speed * manager->time.delta_time;
		normalize_angle(&manager->player.rotation_angle);
	}
}

void	move_player(t_cub_manager *manager)
{
	double nx;
	double ny;

	nx = 0;
	ny = 0;
	if (manager->__move_slideways)
		move_slideways(manager, &nx, &ny, manager->__move_slideways);
	else if (manager->player.move_y)
	{
		ny = manager->player.y + sin(manager->player.rotation_angle)\
		* manager->player.walk_direction * manager->player.walk_speed\
		* manager->time.delta_time;
		nx = manager->player.x + cos(manager->player.rotation_angle)\
		* manager->player.walk_direction * manager->player.walk_speed\
		* manager->time.delta_time;	
	}
	move_player_dda(manager, nx, ny);
	rotate_player(manager);
}

void time_utils(t_cub_manager *manager)
{
	if (manager->time.lastTick == 0)
		manager->time.lastTick = get_time();
	manager->time.delta_time = (get_time() - manager->time.lastTick) / 1000.0;
	manager->time.lastTick = get_time();
}

void	render_cell(t_cub_manager* manager, double mapsx, double mapsy, int i)
{
	t_draw_point_struct p;

		p.point.y = i;
		for (int j = 0;j < mini_x * 10;++j)
		{
			p.point.x = j;
			int x = (mapsy + i) / mini_y;
			int y = (mapsx + j) / mini_x;
			if (x >= 0 && x < manager->map->map_height\
				&& y >= 0 && y < manager->map->map_width\
				&& manager->map->map[x][y] == '1')
					p.color = 0x00ffffff;
			else if (x >= 0 && x < manager->map->map_height\
				&& y >= 0 && y < manager->map->map_width &&\
				(manager->map->map[x][y] == 'D' ||\
				manager->map->map[x][y] == 'O'))
					p.color = 0x00ff0000;
			else
				p.color = 0x00000000;
			cub_mlx_pixel_put(&manager->mlx_manager.img_data,p);
		}
}

void draw_minimap(t_cub_manager* manager)
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

void draw_player(t_cub_manager* manager)
{
	t_draw_lines_struct	line;
	double				incr;
	int					i;
	double			angle;

	incr = radians(FOV) / 3.0;
	angle = manager->player.rotation_angle -\
					(radians((double) FOV / 2.0) );
	line.color = 0x0000ff00;
	line.start.x = round(mini_x * 10 / 2.0);
	line.start.y = round(mini_y * 6 / 2.0);
	i  = -1;
	while (++i < 3)
	{
		line.end.x = line.start.x + cos(angle) * mini_x;
		line.end.y = line.start.y + sin(angle) * mini_x;
		draw_line(manager, line);
		angle += incr;
	}
}

int draw(t_cub_manager *manager)
{
	time_utils(manager);
	move_player(manager);
	cast_all_rays(manager);
	draw_minimap(manager);
	draw_player(manager);
	mlx_put_image_to_window(manager->mlx_manager.mlx,\
		manager->mlx_manager.mlx_window,\
		manager->mlx_manager.img_data.img, 0, 0);
	clear_window(manager, 0x00000000, WIDTH, HEIGHT);
	return 0;
}

double 	__distance(double x, double y, double x1, double y1)
{
	return (sqrt((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y)));
}

bool	__inside_wall(int x, int y, bool isfacingup, t_cub_manager* mn)
{
	int	x_index;
	int	y_index;

	if (isfacingup == true)
		y -= 1;
	x_index = x / TILE_SIZE;
	y_index = y / TILE_SIZE;
	if (!(x_index >= 0 && x_index < mn->map->map_width\
		&& y_index >= 0 && y_index < mn->map->map_height))
		return (false);
	double d = __distance(mn->player.x, mn->player.y, x, y);
	return (mn->map->map[y_index][x_index] == '1'\
		|| (mn->map->map[y_index][x_index] == 'D'\
		&& (d > TILE_SIZE)));
}
bool	__inside_wall_ver(int x, int y, bool isfacingleft, t_cub_manager* mn)
{
	int		x_index;
	int		y_index;
	double	d;

	if (isfacingleft == true)
		x -= 1;
	x_index = x / TILE_SIZE;
	y_index = y / TILE_SIZE;
	if (!(x_index >= 0 && x_index < mn->map->map_width\
		&& y_index >= 0 && y_index < mn->map->map_height))
			return (false);
	d = __distance(mn->player.x, mn->player.y, x, y);
	return (mn->map->map[y_index][x_index] == '1'\
	|| (mn->map->map[y_index][x_index] == 'D' && (d > TILE_SIZE)));
}

void intitialize_caster_var(t_cast_function *var)
{
	var->xintercept = 0.0;
	var->yintercept = 0.0;
	var->xstep = 0.0;
	var->ystep = 0.0;
	var->found_horz_wall_hit = false;
	var->horz_wall_hit_x = 0;
	var->horz_wall_hit_y = 0;
	var->next_horz_touch_x = 0.0;
	var->next_horz_touch_y = 0.0;
	var->found_ver_hit = false;
	var->ver_hit_x = 0;
	var->ver_hit_y = 0;
	var->next_ver_touch_x = 0.0;
	var->next_ver_touch_y = 0.0;
	var->horz_hit_distance = 0.0;
	var->vert_hit_distance = 0.0;
}

void	initialize_horizontal_check(t_ray *ray, t_cub_manager *manager, t_cast_function *var)
{
	var->yintercept = floor(manager->player.y / TILE_SIZE) * (double)TILE_SIZE;
	if (ray->is_fac_down)
		var->yintercept += TILE_SIZE;
	var->xintercept = manager->player.x + (var->yintercept - manager->player.y) / tan(ray->ray_angle);//?radians or degrees
	var->ystep = TILE_SIZE;
	if (ray->is_fac_up)
		var->ystep *= -1;
	var->xstep = TILE_SIZE / tan(ray->ray_angle);
	if (ray->is_fac_left && var->xstep > 0)
		var->xstep *= -1;
	if (ray->is_fac_right && var->xstep < 0)
		var->xstep *= -1;
	var->next_horz_touch_x = var->xintercept;
	var->next_horz_touch_y = var->yintercept;
}

void	check_horizontal_intersection(t_ray *ray, t_cub_manager *manager, t_cast_function *var)
{
	while (var->next_horz_touch_x >= 0 && var->next_horz_touch_x < manager->map->map_width\
		* TILE_SIZE && var->next_horz_touch_y >= 0 && var->next_horz_touch_y\
		< manager->map->map_height * TILE_SIZE)
	{
		if (__inside_wall(var->next_horz_touch_x, var->next_horz_touch_y, ray->is_fac_up, manager))
		{
			var->found_horz_wall_hit = true;
			var->horz_wall_hit_x = var->next_horz_touch_x;
			var->horz_wall_hit_y = var->next_horz_touch_y;
			break ;
		}
		var->next_horz_touch_x += var->xstep;
		var->next_horz_touch_y += var->ystep;
	}
}

void	initialize_vertical_check(t_ray *ray, t_cub_manager *manager, t_cast_function *var)
{
	var->xintercept = floor(manager->player.x / TILE_SIZE) * TILE_SIZE;
	if (ray->is_fac_right)
		var->xintercept += TILE_SIZE;
	var->yintercept = manager->player.y + (var->xintercept - manager->player.x) * tan(ray->ray_angle);
	
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

void	check_vertical_intersection(t_ray *ray, t_cub_manager *manager, t_cast_function *var)
{
	while (var->next_ver_touch_x >= 0 && var->next_ver_touch_x <manager->map->map_width\
	* TILE_SIZE && var->next_ver_touch_y >= 0\
	&& var->next_ver_touch_y < manager->map->map_height * TILE_SIZE)
	{
		if (__inside_wall_ver(var->next_ver_touch_x, var->next_ver_touch_y,\
			ray->is_fac_left, manager))
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

void calculate_ray_length(t_ray *ray, t_cub_manager *mn, t_cast_function *var)
{
	var->horz_hit_distance = __distance(mn->player.x ,\
	mn->player.y ,var->horz_wall_hit_x, var->horz_wall_hit_y);
	var->vert_hit_distance = __distance(mn->player.x ,\
	mn->player.y ,var->ver_hit_x, var->ver_hit_y);
	ray->was_vert_h = false;
}

void	choose_closest_intersection(t_ray *ray, t_cast_function *var)
{
	if (var->horz_hit_distance < var->vert_hit_distance)
	{
		ray->wallHitX = var->horz_wall_hit_x;
		ray->wallHitY = var->horz_wall_hit_y;
		ray->distance = var->horz_hit_distance;
	}
	else
	{
		ray->wallHitX = var->ver_hit_x;
		ray->wallHitY = var->ver_hit_y;
		ray->distance = var->vert_hit_distance;	
		ray->was_vert_h = true;
	}
}

void	choose_horz_intersection(t_ray *ray, t_cast_function *var)
{
	ray->wallHitX = var->horz_wall_hit_x;
	ray->wallHitY = var->horz_wall_hit_y;
	ray->distance = var->horz_hit_distance;
}
void	choose_ver_intersection(t_ray *ray, t_cast_function *var)
{
	ray->wallHitX = var->ver_hit_x;
	ray->wallHitY = var->ver_hit_y;
	ray->distance = var->vert_hit_distance;
	ray->was_vert_h = true;		
}

void	cast(t_ray* ray, t_cub_manager* manager)
{
	t_cast_function	var;

	intitialize_caster_var(&var);
	normalize_angle(&ray->ray_angle);
	initialize_horizontal_check(ray, manager, &var);
	check_horizontal_intersection(ray, manager, &var);
	initialize_vertical_check(ray, manager, &var);
	check_vertical_intersection(ray, manager, &var);	
	calculate_ray_length(ray, manager, &var);
	if (var.found_horz_wall_hit && var.found_ver_hit)
		choose_closest_intersection(ray, &var);
	else if (var.found_horz_wall_hit)
		choose_horz_intersection(ray, &var);
	else if (var.found_ver_hit)
		choose_ver_intersection(ray, &var);
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

double distance(double x, double y, double a, double b)
{
	return (sqrt((a - x) * (a - x) + (b - y) * (b - y)));
}

void	*xalloc(size_t size)
{
	void *ptr = malloc(size);
	if (!ptr)
		exit(EXIT_FAILURE);
	return (ptr);
}
void	cast_all_rays(t_cub_manager* manager)
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
		manager->rays[i].ray_angle =  ray_angle;
		__initialize_ray_attributes(&manager->rays[i]);
		cast(&manager->rays[i], manager);
		ray_angle += angle_increment;
	}
	rendering_3d_walls(manager);
}

void	__render_ceiling(t_cub_manager* manager, int x, int wallTopPixel)
{
	t_draw_point_struct p;

	p.limits.x = WIDTH;
	p.limits.y = HEIGHT;
	p.color = manager->map->c;
	p.point.x = x;
		for (int y = 0; y < wallTopPixel; y++)
		{
			p.point.y = y;
			cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);
		}
}

void	__render_floor(t_cub_manager* manager, int x, int y)
{
	t_draw_point_struct p;

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

void	put_color_into_wall(t_cub_manager* manager, int x, int y, int color)
{
	t_draw_point_struct p;

	if (color == 0)
		return ;
	p.limits.x = WIDTH;
	p.limits.y = HEIGHT;
	p.color = color;
	p.point.x = x;
	p.point.y = y;
	cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);
}

void	__render_gun(t_cub_manager* mn)
{
	int			x_start;
	int			y_start;
	t_texture	choosen_gun;

	choosen_gun = mn->weapons.gun[mn->weapons.gun_type + mn->weapons.gun_state];
	x_start = WIDTH / 2 - choosen_gun.wi / 2;
	y_start = HEIGHT - choosen_gun.hi;
	for (int y = y_start; y < y_start + choosen_gun.hi; y++)
	{
		for (int x = x_start; x < x_start + choosen_gun.wi; x++)
		{
			u_int32_t color = *(((int *)choosen_gun.tex_img_data.addr+\
			((y - y_start) * choosen_gun.wi + (x - x_start))));
			put_color_into_wall(mn, x, y, color);	
		}
	}
	if (mn->weapons.gun_frames == 0)
		mn->weapons.gun_state = STAND;
	else
		mn->weapons.gun_frames--;
}

void	rendering_3d_walls(t_cub_manager* manager)
{
	t_draw_point_struct p;
	p.limits.x = WIDTH;
	p.limits.y = HEIGHT;
	p.color = 0x00ffffff;
	double off_x = -1;
	double off_y = -1;
	for (int i = 0; i < NUMBER_OF_RAYS; i++) {
		int tex = -1;
        double perpDistance = manager->rays[i].distance * cos(manager->rays[i].ray_angle - manager->player.rotation_angle);
        double distanceProjPlane = (WIDTH / 2.0) / tan(radians(FOV) / 2.0);
        double projectedWallHeight = ((double)TILE_SIZE / perpDistance) * distanceProjPlane;

        double	wallStripHeight = projectedWallHeight;

        double wallTopPixel = (HEIGHT / 2.0) - (wallStripHeight / 2.0);

        double wallBottomPixel = (HEIGHT / 2.0) + (wallStripHeight / 2.0);
		int x = 0;
		int y = 0;
		if (manager->rays[i].was_vert_h)
		{
			off_x = fmod(manager->rays[i].wallHitY, TILE_SIZE);
			// checking for vertical doors;
			y = manager->rays[i].wallHitY / TILE_SIZE;
			if (manager->rays[i].is_fac_right)
			{
				x = (manager->rays[i].wallHitX + 1.) / TILE_SIZE;
				if (manager->map->map[y][x] != 'D')
					tex = EAST;
				else
					tex = DOOR;
			}
			else
			{
				x = (manager->rays[i].wallHitX - 1.) / TILE_SIZE;
				if (manager->map->map[y][x] != 'D')
					tex = WEST;
				else
					tex = DOOR;
			}
		}
		else
		{
			x = manager->rays[i].wallHitX / TILE_SIZE;
			if (manager->rays[i].is_fac_down)
			{
				y = (manager->rays[i].wallHitY + 1.) / TILE_SIZE;
				if (manager->map->map[y][x] != 'D')
					tex = SOUTH;
				else
					tex = DOOR;
			}
			else
			{
				y = (manager->rays[i].wallHitY - 1.) / TILE_SIZE;
				if (manager->map->map[y][x] != 'D')
					tex = NORTH;
				else
					tex = DOOR;
			}
			off_x = fmod(manager->rays[i].wallHitX, TILE_SIZE);
		}
		if (tex != -1)
		{
			off_x = off_x / TILE_SIZE * (double)manager->map->wall_textures[tex].wi;
			__render_ceiling(manager, i, wallTopPixel);
			for (int j = wallTopPixel; j < wallBottomPixel - 1;++j)
			{
				off_y = (j - (int)wallTopPixel) / wallStripHeight * manager->map->wall_textures[tex].hi;
				p.point.x = i * WALL_STRIP_WIDTH;
				p.point.y = j;
				p.color = *(((int *)manager->map->wall_textures[tex].tex_img_data.addr + ((int)off_y * manager->map->wall_textures[tex].wi + (int)off_x)));
				cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);

			}
			__render_floor(manager, i, wallBottomPixel);
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

void	protect_textures(t_cub_manager* manager, bool flag)
{
	int i;

	i = -1;
	while (++i < DOOR)
	{
		if (flag && manager->map->wall_textures[i].img == NULL)
			panic("Error: while decoding textures");
		if (!flag && manager->map->wall_textures[i].tex_img_data.addr == NULL)
			panic("Error\ncannot get texture data");
	}
}

void	getting_textures_data(t_cub_manager* manager)
{
	int i;

	i = -1;
	while (++i < DOOR)
	{
		manager->map->wall_textures[i].tex_img_data.addr = mlx_get_data_addr\
		(manager->map->wall_textures[i].img, &manager->map->wall_textures[i].\
		tex_img_data.bits_per_pixel, &manager->map->wall_textures[i].\
		tex_img_data.line_length, &manager->map->wall_textures[i].\
		tex_img_data.endian);
	}
	protect_textures(manager, false);
}

void	decoding_xpm_files(t_cub_manager* manager)
{
	manager->map->wall_textures[NORTH].img = mlx_xpm_file_to_image(manager->\
	mlx_manager.mlx, manager->map->no, &manager->map->wall_textures[NORTH].wi\
	, &manager->map->wall_textures[NORTH].hi);
	manager->map->wall_textures[SOUTH].img = mlx_xpm_file_to_image(manager->\
	mlx_manager.mlx, manager->map->so, &manager->map->wall_textures[SOUTH].wi\
	, &manager->map->wall_textures[SOUTH].hi);
	manager->map->wall_textures[EAST].img = mlx_xpm_file_to_image(manager->\
	mlx_manager.mlx, manager->map->ea, &manager->map->wall_textures[EAST].wi\
	, &manager->map->wall_textures[EAST].hi);
	manager->map->wall_textures[WEST].img = mlx_xpm_file_to_image(manager->\
	mlx_manager.mlx, manager->map->we,\
	 &manager->map->wall_textures[WEST].wi, &manager->map->\
	 wall_textures[WEST].hi);
	protect_textures(manager, true);
	getting_textures_data(manager);
}

void	protect_gun_textures(t_cub_manager* manager, int flag)
{
	int	i;

	i = -1;
	while (++i < 4)
	{
		if (flag && manager->weapons.gun[i].img == NULL)
			panic("Error: while decoding gun textures\n");
		if (!flag && manager->weapons.gun[i].tex_img_data.addr == NULL)
			panic("Error\ncannot get gun texture data\n");
	}
}

void	__get_gun_data(t_cub_manager* manager)
{
	manager->weapons.gun[PISTOL].tex_img_data.addr = mlx_get_data_addr\
	(manager->weapons.gun[PISTOL].img, &manager->weapons.gun[PISTOL].\
	tex_img_data.bits_per_pixel,\
	&manager->weapons.gun[PISTOL].tex_img_data.line_length, &manager->\
	weapons.gun[PISTOL].tex_img_data.endian);
	manager->weapons.gun[SNIPER].tex_img_data.addr = mlx_get_data_addr\
	(manager->weapons.gun[SNIPER].img, &manager->weapons.gun[SNIPER].\
	tex_img_data.bits_per_pixel,\
	&manager->weapons.gun[SNIPER].tex_img_data.line_length, &manager->\
	weapons.gun[SNIPER].tex_img_data.endian);
	manager->weapons.gun[PISTOL + 1].tex_img_data.addr = mlx_get_data_addr\
	(manager->weapons.gun[PISTOL + 1].img, &manager->weapons.gun[PISTOL + 1].\
	tex_img_data.bits_per_pixel,\
	&manager->weapons.gun[PISTOL + 1].tex_img_data.line_length,\
	&manager->weapons.gun[PISTOL + 1].tex_img_data.endian);
	manager->weapons.gun[SNIPER + 1].tex_img_data.addr = mlx_get_data_addr\
	(manager->weapons.gun[SNIPER + 1].img, &manager->weapons.gun[SNIPER + 1]\
	.tex_img_data.bits_per_pixel,\
	&manager->weapons.gun[SNIPER + 1].tex_img_data.line_length, \
	&manager->weapons.gun[SNIPER + 1].tex_img_data.endian);
	protect_gun_textures(manager, false);
}

void	__load_gun_textures(t_cub_manager* manager)
{
	manager->weapons.gun[PISTOL].img = mlx_xpm_file_to_image(manager->\
	mlx_manager.mlx, STANDING_PISTOL_PATH, &manager->weapons.gun[PISTOL].\
	wi, &manager->weapons.gun[PISTOL].hi);
	manager->weapons.gun[SNIPER].img = mlx_xpm_file_to_image(manager->\
	mlx_manager.mlx, STANDING_SNIPER_PATH, &manager->weapons.gun[SNIPER].\
	wi, &manager->weapons.gun[SNIPER].hi);
	manager->weapons.gun[PISTOL + 1].img = mlx_xpm_file_to_image(manager->\
	mlx_manager.mlx, SHOOTING_PISTOL_PATH, &manager->weapons.gun[PISTOL + 1].\
	wi, &manager->weapons.gun[PISTOL + 1].hi);
	manager->weapons.gun[SNIPER + 1].img = mlx_xpm_file_to_image(manager->\
	mlx_manager.mlx, SHOOTING_SNIPER_PATH, &manager->weapons.gun[SNIPER + 1]\
	.wi, &manager->weapons.gun[SNIPER + 1].hi);
	protect_gun_textures(manager, true);
	__get_gun_data(manager);
}


int	__mouse_press(int button, int x, int y, t_cub_manager *manager)
{
	if (button == 1 && x > 0 && x < WIDTH && y > 0 && y < HEIGHT)
		controls(KEY_SPACE, manager);
	if (button == 2 && x > 0 && x < WIDTH && y > 0 && y < HEIGHT)
		controls(KEY_R, manager);
	return (0);
}

void	init_mlx(t_cub_manager* manager)
{
	manager->mlx_manager.mlx = mlx_init();
	manager->mlx_manager.mlx_window =\
	mlx_new_window(manager->mlx_manager.mlx, WIDTH, HEIGHT, "cub3D");
	manager->mlx_manager.img_data.img =\
	mlx_new_image(manager->mlx_manager.mlx, WIDTH, HEIGHT);
	manager->mlx_manager.img_data.addr =\
	mlx_get_data_addr(manager->mlx_manager.img_data.img,\
	&manager->mlx_manager.img_data.bits_per_pixel, &manager->\
	mlx_manager.img_data.line_length, &manager->mlx_manager.img_data.endian);
}

void load_door_textures(t_cub_manager *manager)
{
	manager->map->wall_textures[DOOR].img = mlx_xpm_file_to_image(manager->mlx_manager.\
	mlx, DOOR_PATH, &manager->map->wall_textures[DOOR].wi, &manager->map->wall_textures[DOOR].hi);
	manager->map->wall_textures[DOOR].tex_img_data.addr = mlx_get_data_addr(manager->map->\
	wall_textures[DOOR].img, &manager->map->wall_textures[DOOR].tex_img_data.bits_per_pixel,\
	&manager->map->wall_textures[DOOR].tex_img_data.line_length, &manager->map->\
	wall_textures[DOOR].tex_img_data.endian);
}

void	init_map(t_cub_manager *manager, t_map_manager *mapm)
{
	char	**map;
	int		i;
	int		j;

	manager->map = mapm;
	manager->map->map_width = get_map_width(manager->map);
	manager->map->map_height = get_map_height(manager->map);
	map = xalloc(sizeof(char *) * manager->map->map_height);
	i = -1;
	while (++i < manager->map->map_height)
		map[i] = xalloc(manager->map->map_width);
	i = -1;
	while (++i < manager->map->map_height)
	{
		j = -1;
		while (++j < manager->map->map_width)
		{
			if (j < (int) ft_strlen(manager->map->map[i]))
				map[i][j] = manager->map->map[i][j];
			else
				map[i][j] = ' ';
		}
	}
	manager->map->map = map;
}

int	check_if_cell_has_player(t_cub_manager *manager, int i, int j)
{
	if (ft_strchr(PLAYER_CHAR, manager->map->map[i][j]))
	{
		manager->player.x = j * TILE_SIZE + TILE_SIZE / 2.0;
		manager->player.y = i * TILE_SIZE + TILE_SIZE / 2.0;
		if (manager->map->map[i][j] == 'S')
			manager->player.rotation_angle = radians(90);
		if (manager->map->map[i][j] == 'N')
			manager->player.rotation_angle = radians(270);
		if (manager->map->map[i][j] == 'W')
			manager->player.rotation_angle = radians(180);
		if (manager->map->map[i][j] == 'E')
			manager->player.rotation_angle = 0;
		return (1);
	}
	return (0);
}

void	init_player(t_cub_manager *manager)
{
	int		found_player;
	int		i;
	int		j;

	found_player = 0;
	i = -1;
	manager->player.rotation_speed = radians(R_SPEED);
	manager->time.delta_time = 0;
	manager->time.lastTick = 0;
	manager->player.walk_speed = W_SPEED;
	i = -1;
	while (++i < manager->map->map_height)
	{
		j = -1;
		while (++j < manager->map->map_width)
			if(check_if_cell_has_player(manager, i, j))
				break ;
	}
	manager->player.move_x = false;
	manager->player.move_y = false;
	manager->player.rotate = false;
}

void	init_manager_attr(t_cub_manager *manager)
{
	manager->rays = xalloc(NUMBER_OF_RAYS * sizeof(t_ray));
	manager->weapons.gun_state = STAND;
	manager->weapons.gun_frames = 0;
	manager->weapons.gun_type = PISTOL;
	manager->door = 0x0;
	manager->head = 0x0;
	manager->tail = 0x0;
	manager->next = 0x0;
	manager->mouse_x = WIDTH / 2;
	manager->mouse_move = false;
	manager->__move_slideways = 0;
}

int render(t_map_manager *map_manager)
{
	t_cub_manager manager;

	init_map(&manager, map_manager);
	init_player(&manager);
	init_mlx(&manager);
	load_door_textures(&manager);
	decoding_xpm_files(&manager);
	__load_gun_textures(&manager);
	init_manager_attr(&manager);
	mlx_loop_hook(manager.mlx_manager.mlx, draw, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, ON_KEYDOWN, 1L<<0, controls, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, ON_KEYUP, 1L<<0, controls_up, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, 6, 1, __mouse_move, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, 17, 0, __destroy, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, 4, 1, __mouse_press, &manager);
	mlx_loop(manager.mlx_manager.mlx);
	return (0);
}