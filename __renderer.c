/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __renderer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:26:42 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/09 11:37:48 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"
void	rendering_3d_walls(t_cub_manager* manager);

void	cast_all_rays(t_cub_manager* manager);

double radians(double angle)
{
	return (angle * (M_PI / (double)180));
}

inline double dist(int x1, int x2, int y1, int y2)
{
    // Calculating distance
    return sqrt(pow(x2 - x1, 2)
                + pow(y2 - y1, 2) * 1.0);
}

void	cub_mlx_pixel_put(t_img_data *data, t_draw_point_struct p)
{
	char	*dst;

	if (p.point.x < 0 || p.point.x >= p.limits.x || p.point.y < 0 || p.point.y >= p.limits.y)
		return ;
	dst = data->addr + ((int)p.point.y * data->line_length + (int)p.point.x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = p.color;
}

int max(int a, int b)
{
    if (a > b)
        return a;
    else
	    return b;
}
int min(int a, int b)
{
    if (a > b)
        return b;
    else
	    return a;
}

void draw_line(t_cub_manager *manager, t_draw_lines_struct lines)
{
  // Calculate the difference between the starting and ending x and y coordinates
  double dx = lines.end.x - lines.start.x;
  double dy = lines.end.y - lines.start.y;

  // Determine the number of steps required to draw the line
  double steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
  int st = round(steps);

  // Calculate the x and y increments for each step
  double xIncrement = dx / (steps);
  double yIncrement = dy / (steps);

  // Set the starting x and y coordinates
  t_draw_point_struct coor;
  coor.point.x = lines.start.x;
  coor.point.y = lines.start.y;
  coor.limits = lines.limits;
  coor.color = lines.color;
  // Loop through the number of steps required to draw the line
  for (int i = 0; i <= st; ++i) {
    // Set the color of the pixel at (x, y)
    cub_mlx_pixel_put(&manager->mlx_manager.img_data, coor);

    // Increment the x and y coordinates
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
	int tmp;

	t_draw_point_struct p;
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

void	draw_full_rect(t_cub_manager *manager, t_draw_lines_struct lines)
{
	int i;

	i = -1;
	(void) manager;//TODO: REMOVE THIS
	while(++i < lines.end.y)
	{
		// draw_line(x , y + i, x + width, y + i, lx, ly, manager, color);
		lines.start.y += i;
		// lines.end.x += width;
		lines.end.y += i;
	}
}

void	draw_empty_rect(t_cub_manager *manager, t_draw_lines_struct lines)
{
	draw_line(manager, lines);
	draw_line(manager, lines);
	draw_line(manager, lines);
	draw_line(manager, lines);
	// draw_line(x, y, x + width, y, lx, ly, manager, color);
	// draw_line(x, y, x, y + height, lx, ly, manager, color);
	// draw_line(x, y + height, x + width, y + height,lx, ly, manager, color);
	// draw_line(x + width, y, x + width, y + height,lx, ly, manager, color);
}

bool	__has_has_wall_at(double x, double y, t_cub_manager* manager) { int map_grid_index_x;
	int map_grid_index_y;

	map_grid_index_x = floor(x / TILE_SIZE);
	map_grid_index_y = floor(y / TILE_SIZE);
	if (map_grid_index_x < 0 || map_grid_index_x >= manager->map->map_width || map_grid_index_y < 0 || map_grid_index_y >= manager->map->map_height)
		return (true);
	return (manager->map->map[map_grid_index_y][map_grid_index_x] == '1');//add aditionatl check for doors etc...
}
void	__move_player(t_cub_manager* manager)
{
	double	new_x;
	double	new_y;

	new_x = manager->player.x + manager->player.walk_direction * manager->player.walk_speed * cos(manager->player.rotation_angle);
	new_y = manager->player.y + manager->player.walk_direction * manager->player.walk_speed * sin(manager->player.rotation_angle);
	if (!__has_has_wall_at(new_x, new_y, manager))
	{
		manager->player.x = new_x;
		manager->player.y = new_y;
	}
}

void	__rotate_player(t_cub_manager* manager)
{
	normalize_angle(&manager->player.rotation_angle);
	manager->player.rotation_angle += manager->player.turn_direction * manager->player.rotation_speed;
	normalize_angle(&manager->player.rotation_angle);
}

void __switch_guns(t_cub_manager *manager)
{
	if (manager->weapons.gun_type == PISTOL)
		manager->weapons.gun_type = SNIPER;
	else
		manager->weapons.gun_type = PISTOL;
}

void	__move_player_sideways(t_cub_manager* manager, int key)
{
	int		turn_direction;
	double	new_x;
	double	new_y;
	double	rotation_angle;

	if (key == KEY_A)
		turn_direction = -1;
	else
		turn_direction = 1;
	rotation_angle = manager->player.rotation_angle + (turn_direction * (M_PI / 2));
	normalize_angle(&rotation_angle);
	new_x = manager->player.x + manager->player.walk_direction * manager->player.walk_speed * cos(rotation_angle);
	new_y = manager->player.y + manager->player.walk_direction * manager->player.walk_speed * sin(rotation_angle);
	if (!__has_has_wall_at(new_x, new_y, manager))
	{
		manager->player.x = new_x;
		manager->player.y = new_y;
	}
}

int	controls(int key, t_cub_manager* manager)
{
	if (key == KEY_LEFT)
		manager->player.turn_direction = -1;
	if (key == KEY_RIGHT)
		manager->player.turn_direction = 1;
	if (key == KEY_W)
		manager->player.walk_direction = 1;
	if (key == KEY_S)
		manager->player.walk_direction = -1;
	if (key == KEY_SPACE)
	{
		manager->weapons.gun_state = SHOOT;
		manager->weapons.gun_frames = 3;
	}
	if (key == KEY_ESC)
		exit(EXIT_SUCCESS);
	if (key == KEY_W || key == KEY_S)
		__move_player(manager);
	if (key == KEY_LEFT || key == KEY_RIGHT)
		__rotate_player(manager);
	if (key == KEY_R)
		__switch_guns(manager);
	if (key == KEY_D || key == KEY_A)
		__move_player_sideways(manager, key);
	return 0;
}

int	__key_release(int key, t_cub_manager* manager)
{
	if (key == 124 || key == 123)
		manager->player.turn_direction = 0;
	if (key == 125 || key == 126)
		manager->player.walk_direction = 0;
	if (key == 49)
	{
		manager->weapons.gun_state = STAND;
		manager->weapons.gun_frames = 0;
	}
	return (0);
}


// void draw_minimap(int player_x, int player_y, int map[][MAP_WIDTH], t_cub_manager *manager)
// {
//     // Clear the mini-map
//     clear_window(manager, 0x000000);

//     // Calculate the starting and ending x and y coordinates for the mini-map
//     int start_x = max(player_x - MINIMAP_WIDTH / 2, 0);
//     int end_x = min(player_x + MINIMAP_WIDTH / 2, MAP_WIDTH);
//     int start_y = max(player_y - MINIMAP_HEIGHT / 2, 0);
//     int end_y = min(player_y + MINIMAP_HEIGHT / 2, MAP_HEIGHT);

//     // Draw the mini-map
//     for (int i = start_y; i < end_y; i++)
//     {
//         for (int j = start_x; j < end_x; j++)
//         {
//             if (map[i][j] == 1)
//                 cub_mlx_pixel_put(&manager->mlx_manager.img_data, MINIMAP_X + j - start_x, MINIMAP_Y + i - start_y, 0xFFFFFF);
//         }
//     }

//     // Draw the player on the mini-map
//     cub_mlx_pixel_put(&manager->mlx_manager.img_data, MINIMAP_X + player_x - start_x, MINIMAP_Y + player_y - start_y, 0xFF0000);
// }
# define mini_x (30) 
# define mini_y (30)

void	draw_empty_circle(t_cub_manager *manager, t_draw_circle c)
{
	t_draw_point_struct p;
	p.point.x = c.center.x;
	p.point.y = c.center.y;
	p.color = c.color;
	for (int i = 0;i < 360;++i)
	{
		// cub_mlx_pixel_put(&manager->mlx_manager.img_data, c.center.x + c.radius * cos(radians(i)), y + radius * sin(radians(i)), lx, ly, color);
		cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);
	}
}
void	__render_gun(t_cub_manager* manager);
int draw(t_cub_manager *manager)
{
	cast_all_rays(manager);
	mlx_put_image_to_window(manager->mlx_manager.mlx, manager->mlx_manager.mlx_window, manager->mlx_manager.img_data.img, 0, 0);
	clear_window(manager, 0x00000000, WIDTH, HEIGHT);

	return 0;
}

double 	__distance(double x, double y, double x1, double y1)
{
	return (sqrt((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y)));
}

bool	__inside_wall(int x, int y, bool isfacingup, t_cub_manager* manager)
{
	int	x_index;
	int	y_index;

	if (isfacingup == true)
		y -= 1;
	x_index = x / TILE_SIZE;
	y_index = y / TILE_SIZE;
	if (!(x_index >= 0 && x_index < manager->map->map_width && y_index >= 0 && y_index < manager->map->map_height))
		return (false);
	return (manager->map->map[y_index][x_index] == '1');
}
bool	__inside_wall_ver(int x, int y, bool isfacingleft, t_cub_manager* manager)
{
	int	x_index;
	int	y_index;

	if (isfacingleft == true)
		x -= 1;
	x_index = x / TILE_SIZE;
	y_index = y / TILE_SIZE;
	if (!(x_index >= 0 && x_index < manager->map->map_width && y_index >= 0 && y_index < manager->map->map_height))
		return (false);
	return (manager->map->map[y_index][x_index] == '1');
}

void	cast(t_ray* ray, t_cub_manager* manager)
{
	normalize_angle(&ray->rayAngle);
	double	xintercept;
	double	yintercept;
	double	xstep, ystep;

	bool found_horz_wall_hit = false;
	double	horz_wall_hit_x = 0;
	double	horz_wall_hit_y = 0;
	// double horz_wall_color = 0;
	
	//fid the y-coordinates of the closest horiz grid intersection
	yintercept = floor(manager->player.y / TILE_SIZE) * (double)TILE_SIZE;
	if (ray->isRayFacingDown)
		yintercept += TILE_SIZE;
	xintercept = .0;
	if (tan(ray->rayAngle) != 0.0)
	{
		xintercept = manager->player.x + (yintercept - manager->player.y) / tan(ray->rayAngle);//?radians or degrees
		ystep = TILE_SIZE;
		if (ray->isRayFacingUp)
			ystep *= -1;
		
		xstep = TILE_SIZE / tan(ray->rayAngle);//radians or degrees
		if (ray->isRayFacingLeft && xstep > 0)
			xstep *= -1;
		if (ray->isRayFacingRight && xstep < 0)
			xstep *= -1;
	}
		
	double next_horz_touch_x = xintercept;
	double next_horz_touch_y = yintercept;

	
    // Increment xstep and ystep until we find a wall
	while (next_horz_touch_x >= 0 && next_horz_touch_x < manager->map->map_width * TILE_SIZE && next_horz_touch_y >= 0 && next_horz_touch_y < manager->map->map_height * TILE_SIZE)
	{
		if (__inside_wall(next_horz_touch_x, next_horz_touch_y, ray->isRayFacingUp, manager))
		{
			found_horz_wall_hit = true;
			horz_wall_hit_x = next_horz_touch_x;
			horz_wall_hit_y = next_horz_touch_y;
			// horz_wall_color = content; //don't forget that
			// printf("found horizontal wall\n-----------------------------------\n\n");
			break ;
		}
		// printf("nexthx: %lf nexhy: %lf (%s:%d)\n-------------------\n", next_horz_touch_x, next_horz_touch_y, __FILE__, __LINE__);
		next_horz_touch_x += xstep;
		next_horz_touch_y += ystep;
	}

	//vertical ray-grid intersection code
	
	bool found_ver_hit = false;
	double ver_hit_x = 0;
	double ver_hit_y = 0;
	//don't forget the color
	
	xintercept = floor(manager->player.x / TILE_SIZE) * TILE_SIZE;
	if (ray->isRayFacingRight)
		xintercept += TILE_SIZE;
	yintercept = manager->player.y + (xintercept - manager->player.x) * tan(ray->rayAngle);
	
	xstep = TILE_SIZE;
	if (ray->isRayFacingLeft)
		xstep = -xstep;
	ystep = TILE_SIZE * tan(ray->rayAngle);
	if (ray->isRayFacingUp && ystep > 0)
		ystep *= -1;
	if (ray->isRayFacingDown && ystep < 0)
		ystep *= -1;
	
	double next_ver_touch_x = xintercept;
	double next_ver_touch_y = yintercept;
	
	while (next_ver_touch_x >= 0 && next_ver_touch_x <manager->map->map_width * TILE_SIZE && next_ver_touch_y >= 0 && next_ver_touch_y < manager->map->map_height * TILE_SIZE)
	{
		if (__inside_wall_ver(next_ver_touch_x, next_ver_touch_y, ray->isRayFacingLeft, manager))
		{
			found_ver_hit = true;
			ver_hit_x = next_ver_touch_x;
			ver_hit_y = next_ver_touch_y;
			break ;
		}
		next_ver_touch_x += xstep;
		next_ver_touch_y += ystep;
	}
	

	//calculate both hor and vert distances and choose the smallest value
	
	double horz_hit_distance = 0.0;
	double vert_hit_distance = 0.0;
	horz_hit_distance = hypot(manager->player.x - horz_wall_hit_x, manager->player.y - horz_wall_hit_y);
	vert_hit_distance = hypot(manager->player.x - ver_hit_x, manager->player.y - ver_hit_y);
	ray->wasHitVertical = false;
	if (found_horz_wall_hit && found_ver_hit)
	{
		if (horz_hit_distance < vert_hit_distance)
		{
			ray->wallHitX = horz_wall_hit_x;
			ray->wallHitY = horz_wall_hit_y;
			ray->distance = horz_hit_distance;
		}
		else
		{
			ray->wallHitX = ver_hit_x;
			ray->wallHitY = ver_hit_y;
			ray->distance = vert_hit_distance;	
			ray->wasHitVertical = true;
		}
	}
	else if (found_horz_wall_hit)
	{
		ray->wallHitX = horz_wall_hit_x;
		ray->wallHitY = horz_wall_hit_y;
		ray->distance = horz_hit_distance;
	}
	else if (found_ver_hit)
	{
		ray->wallHitX = ver_hit_x;
		ray->wallHitY = ver_hit_y;
		ray->distance = vert_hit_distance;
		ray->wasHitVertical = true;		
	}
}

void	__initialize_ray_attributes(t_ray *ray)
{
	ray->distance = 0;
	ray->wallHitX = 0;
	ray->wallHitY = 0;
	ray->wasHitVertical = false;
	//color here
	if (ray->rayAngle > 0 && ray->rayAngle < M_PI)
		ray->isRayFacingDown = true;
	else
		ray->isRayFacingDown = false;
	ray->isRayFacingUp = !ray->isRayFacingDown;
	if (ray->rayAngle < M_PI_2 || ray->rayAngle > 1.5 * M_PI)
		ray->isRayFacingRight = true;
	else
		ray->isRayFacingRight = false;
	ray->isRayFacingLeft = !ray->isRayFacingRight;
}

void	render_3d_projected_walls(t_cub_manager* manager);
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
		manager->rays[i].rayAngle =  ray_angle;
		__initialize_ray_attributes(&manager->rays[i]);
		cast(&manager->rays[i], manager);
		ray_angle+= angle_increment;
	}
	rendering_3d_walls(manager);
}

int	__get_x_offset(t_ray ray, int width)
{
	double x_offset;

	if (ray.wasHitVertical)
		x_offset = fmod(ray.wallHitY, TILE_SIZE);
	else
		x_offset = fmod(ray.wallHitX, TILE_SIZE);
	return ((int) (x_offset * width / TILE_SIZE));
}

int	__get_texture_id(t_ray ray)
{
	if (ray.wasHitVertical && ray.isRayFacingLeft)
		return (EAST);
	if (ray.wasHitVertical && ray.isRayFacingRight)
		return (WEST);
	if (!ray.wasHitVertical && ray.isRayFacingUp)
		return (SOUTH);
	return (NORTH);
}

int	__get_y_offset(double wall_strip_height, int j, int hi)
{
	double y_offset;
	int		distance_from_top;

	distance_from_top = j + (wall_strip_height / 2) - (HEIGHT / 2);
	y_offset = distance_from_top / wall_strip_height * hi;
	return ((int)y_offset);
}

int	__get_pixel_color(t_cub_manager* manager, double wall_strip_height, int j, int ray_id)
{
	int			y_offset;
	int			x_offset;
	t_texture	texture;
	int			color;

	texture = manager->map->wall_textures[__get_texture_id(manager->rays[ray_id])];	
	y_offset = __get_y_offset(wall_strip_height, j, texture.hi);
	x_offset = __get_x_offset(manager->rays[ray_id], texture.wi);
	color  = *(((int *)texture.tex_img_data.addr + (y_offset * texture.wi + x_offset)));
	return (color);
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

void	__render_floor(t_cub_manager* manager, int x, int wallBottomPixel)
{
	t_draw_point_struct p;

	p.limits.x = WIDTH;
	p.limits.y = HEIGHT;
	p.color = manager->map->f;
	p.point.x = x;
	for (int y = wallBottomPixel; y < HEIGHT; y++)
	{
		p.point.y = y;
		cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);
	}
}

void	__render_gun(t_cub_manager* manager)
{
	int			x_start;
	int			y_start;
	t_texture	choosen_gun;

	choosen_gun = manager->weapons.gun[manager->weapons.gun_type + manager->weapons.gun_state];
	x_start = WIDTH / 2 - choosen_gun.wi / 2;
	y_start = HEIGHT - choosen_gun.hi;
	for (int y = y_start; y < y_start + choosen_gun.hi; y++)
	{
		for (int x = x_start; x < x_start + choosen_gun.wi; x++)
		{
			u_int32_t color = *(((int *)choosen_gun.tex_img_data.addr + ((y - y_start) * choosen_gun.wi + (x - x_start))));
			if (color != 0)
				cub_mlx_pixel_put(&manager->mlx_manager.img_data, (t_draw_point_struct){.point = {x, y}, .limits = {WIDTH, HEIGHT}, .color = color});
		}
	}
	if (manager->weapons.gun_frames == 0)
		manager->weapons.gun_state = STAND;
	else{
		manager->weapons.gun_frames--;
	}
}

void	rendering_3d_walls(t_cub_manager* manager)
{
	t_draw_point_struct p;
	p.limits.x = WIDTH;
	p.limits.y = HEIGHT;
	p.color = 0x00ffffff;
	for (int i = 0; i < NUMBER_OF_RAYS; i++) {
        double perpDistance = manager->rays[i].distance * cos(manager->rays[i].rayAngle - manager->player.rotation_angle);

        double distanceProjPlane = (WIDTH / 2.0) / tan(radians(FOV) / 2.0);

        double projectedWallHeight = ((double)TILE_SIZE / perpDistance) * distanceProjPlane;

        double	wallStripHeight = projectedWallHeight;

        double wallTopPixel = (HEIGHT / 2.0) - (wallStripHeight / 2.0);
		if (wallTopPixel < 0)
			wallTopPixel = 0;
        double wallBottomPixel = (HEIGHT / 2.0) + (wallStripHeight / 2.0);
		__render_ceiling(manager, i * WALL_STRIP_WIDTH, wallTopPixel);
		for (int j = wallTopPixel; j < wallBottomPixel - 1;++j)
		{
			p.point.x = i * WALL_STRIP_WIDTH;
			p.point.y = j;
			p.color = __get_pixel_color(manager, wallStripHeight, j, i);
			cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);

		}
		__render_floor(manager, i * WALL_STRIP_WIDTH, wallBottomPixel);
    }
	__render_gun(manager);
	// __render_target(manager);
}

void	__geting_data_addr(t_cub_manager* manager)
{
	manager->map->wall_textures[NORTH].tex_img_data.addr = mlx_get_data_addr(manager->map->wall_textures[NORTH].img, &manager->map->wall_textures[NORTH].tex_img_data.bits_per_pixel,
	&manager->map->wall_textures[NORTH].tex_img_data.line_length, &manager->map->wall_textures[NORTH].tex_img_data.endian);
	manager->map->wall_textures[SOUTH].tex_img_data.addr = mlx_get_data_addr(manager->map->wall_textures[SOUTH].img, &manager->map->wall_textures[SOUTH].tex_img_data.bits_per_pixel,
	&manager->map->wall_textures[SOUTH].tex_img_data.line_length, &manager->map->wall_textures[SOUTH].tex_img_data.endian);
	manager->map->wall_textures[EAST].tex_img_data.addr = mlx_get_data_addr(manager->map->wall_textures[EAST].img, &manager->map->wall_textures[EAST].tex_img_data.bits_per_pixel,
	&manager->map->wall_textures[EAST].tex_img_data.line_length, &manager->map->wall_textures[EAST].tex_img_data.endian);
	manager->map->wall_textures[WEST].tex_img_data.addr = mlx_get_data_addr(manager->map->wall_textures[WEST].img, &manager->map->wall_textures[WEST].tex_img_data.bits_per_pixel,
	&manager->map->wall_textures[WEST].tex_img_data.line_length, &manager->map->wall_textures[WEST].tex_img_data.endian);
	if (!manager->map->wall_textures[NORTH].tex_img_data.addr || !manager->map->wall_textures[SOUTH].tex_img_data.addr || !manager->map->wall_textures[EAST].tex_img_data.addr || !manager->map->wall_textures[WEST].tex_img_data.addr)
		panic("mlx_get_data_addr failed");
}

void	__load_textures(t_cub_manager* manager)
{
	manager->map->wall_textures[NORTH].img = mlx_xpm_file_to_image(manager->mlx_manager.mlx, manager->map->no, &manager->map->wall_textures[NORTH].wi, &manager->map->wall_textures[NORTH].hi);
	manager->map->wall_textures[SOUTH].img = mlx_xpm_file_to_image(manager->mlx_manager.mlx, manager->map->so, &manager->map->wall_textures[SOUTH].wi, &manager->map->wall_textures[SOUTH].hi);
	manager->map->wall_textures[EAST].img = mlx_xpm_file_to_image(manager->mlx_manager.mlx,manager->map->ea, &manager->map->wall_textures[EAST].wi, &manager->map->wall_textures[EAST].hi);
	manager->map->wall_textures[WEST].img = mlx_xpm_file_to_image(manager->mlx_manager.mlx, manager->map->we, &manager->map->wall_textures[WEST].wi, &manager->map->wall_textures[WEST].hi);
	if (!manager->map->wall_textures[NORTH].img || !manager->map->wall_textures[SOUTH].img || !manager->map->wall_textures[EAST].img || !manager->map->wall_textures[WEST].img)
		panic("failed to load textures");
	__geting_data_addr(manager);
}

void	__load_gun_textures(t_cub_manager* manager)
{
	//decoding gun textures
	manager->weapons.gun[PISTOL].img = mlx_xpm_file_to_image(manager->mlx_manager.mlx, STANDING_PISTOL_PATH, &manager->weapons.gun[PISTOL].wi, &manager->weapons.gun[PISTOL].hi);
	manager->weapons.gun[SNIPER].img = mlx_xpm_file_to_image(manager->mlx_manager.mlx, STANDING_SNIPER_PATH, &manager->weapons.gun[SNIPER].wi, &manager->weapons.gun[SNIPER].hi);
	manager->weapons.gun[PISTOL + 1].img = mlx_xpm_file_to_image(manager->mlx_manager.mlx, SHOOTING_PISTOL_PATH, &manager->weapons.gun[PISTOL + 1].wi, &manager->weapons.gun[PISTOL + 1].hi);
	manager->weapons.gun[SNIPER + 1].img = mlx_xpm_file_to_image(manager->mlx_manager.mlx, SHOOTING_SNIPER_PATH, &manager->weapons.gun[SNIPER + 1].wi, &manager->weapons.gun[SNIPER + 1].hi);
	
	//protection
	if (!manager->weapons.gun[PISTOL].img || !manager->weapons.gun[SNIPER].img || !manager->weapons.gun[PISTOL + SHOOT].img || !manager->weapons.gun[SNIPER + SHOOT].img)
		panic("failed to load gun textures");
	//getting data addr
	manager->weapons.gun[PISTOL].tex_img_data.addr = mlx_get_data_addr(manager->weapons.gun[PISTOL].img, &manager->weapons.gun[PISTOL].tex_img_data.bits_per_pixel,\
	&manager->weapons.gun[PISTOL].tex_img_data.line_length, &manager->weapons.gun[PISTOL].tex_img_data.endian);
	manager->weapons.gun[SNIPER].tex_img_data.addr = mlx_get_data_addr(manager->weapons.gun[SNIPER].img, &manager->weapons.gun[SNIPER].tex_img_data.bits_per_pixel,\
	&manager->weapons.gun[SNIPER].tex_img_data.line_length, &manager->weapons.gun[SNIPER].tex_img_data.endian);
	manager->weapons.gun[PISTOL + 1].tex_img_data.addr = mlx_get_data_addr(manager->weapons.gun[PISTOL + 1].img, &manager->weapons.gun[PISTOL + 1].tex_img_data.bits_per_pixel,\
	&manager->weapons.gun[PISTOL + 1].tex_img_data.line_length, &manager->weapons.gun[PISTOL + 1].tex_img_data.endian);
	manager->weapons.gun[SNIPER + 1].tex_img_data.addr = mlx_get_data_addr(manager->weapons.gun[SNIPER + 1].img, &manager->weapons.gun[SNIPER + 1].tex_img_data.bits_per_pixel,\
	&manager->weapons.gun[SNIPER + 1].tex_img_data.line_length, &manager->weapons.gun[SNIPER + 1].tex_img_data.endian);

	//protection
	if (!manager->weapons.gun[PISTOL].tex_img_data.addr || !manager->weapons.gun[SNIPER].tex_img_data.addr || !manager->weapons.gun[PISTOL + 1].tex_img_data.addr || !manager->weapons.gun[SNIPER + 1].tex_img_data.addr)
		panic("failed to get data addr for gun textures");
}

int	__mouse_move(int x, int y, t_cub_manager *manager)
{
	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
		return (EXIT_SUCCESS);
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
int render(t_map_manager *map_manager)
{
	int	i;
	int	j;
	int	found_player;
	t_cub_manager manager;

	manager.mlx_manager.mlx = mlx_init();
	manager.map = map_manager;
	manager.map->map_width = get_map_width(manager.map);
	manager.map->map_height = get_map_height(manager.map);
	char **map = xalloc(sizeof(char *) * manager.map->map_height);
	for (int i = 0; i < manager.map->map_height; ++i)
	{
		map[i] = xalloc(manager.map->map_width);
	}
	for (int i = 0;i < manager.map->map_height; ++i)
	{
		for (int j = 0;j < manager.map->map_width;++j)
		{
			if (j < (int) ft_strlen(manager.map->map[i]))
				map[i][j] = manager.map->map[i][j];
			else
				map[i][j] = ' ';
		}
	}
	manager.map->map = map;
	found_player = 0;
	i = -1;
	manager.player.rotation_speed = radians(R_SPEED);
	manager.player.walk_speed = 5;
	while (++i < manager.map->map_height)
	{
		j = -1;
		while (++j < manager.map->map_width)
		{
			if (ft_strchr(PLAYER_CHAR, manager.map->map[i][j]))
			{
				manager.player.x = j * TILE_SIZE + TILE_SIZE / 2.0;
				manager.player.y = i * TILE_SIZE + TILE_SIZE / 2.0;
				if (manager.map->map[i][j] == 'S')
					manager.player.rotation_angle = radians(90);
				if (manager.map->map[i][j] == 'N')
					manager.player.rotation_angle = radians(270);
				if (manager.map->map[i][j] == 'W')
					manager.player.rotation_angle = radians(180);
				if (manager.map->map[i][j] == 'E')
					manager.player.rotation_angle = 0;
				found_player = true;
				break;
			}
		}
		if (found_player)
			break;
	}
	manager.mlx_manager.mlx_window = mlx_new_window(manager.mlx_manager.mlx, WIDTH, HEIGHT, "cub3D");
	manager.mlx_manager.img_data.img = mlx_new_image(manager.mlx_manager.mlx, WIDTH, HEIGHT);
	manager.mlx_manager.img_data.addr = mlx_get_data_addr(manager.mlx_manager.img_data.img, &manager.mlx_manager.img_data.bits_per_pixel, &manager.mlx_manager.img_data.line_length, &manager.mlx_manager.img_data.endian);
	__load_textures(&manager);
	__load_gun_textures(&manager);
	manager.rays = malloc(NUMBER_OF_RAYS * sizeof(t_ray));
	manager.weapons.gun_state = STAND;
	manager.weapons.gun_frames = 0;
	manager.weapons.gun_type = PISTOL;
	manager.mouse_x = WIDTH / 2;
	mlx_loop_hook(manager.mlx_manager.mlx, draw, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, ON_KEYDOWN, 0, controls, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, 3, 2, __key_release, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, 6, 1, __mouse_move, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, 17, 0, __destroy, &manager);
	mlx_loop(manager.mlx_manager.mlx);
	return (0);
}