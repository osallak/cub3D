/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:26:42 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/05 16:22:53 by yakhoudr         ###   ########.fr       */
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

long long get_time(t_cub_manager *manager)
{
	struct timeval t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
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

int controls(int key, t_cub_manager	*manager)
{
	double fx;
	double fy;
	// manager->player.walk_speed = W_SPEED * manager->time.delta_time;
	// printf("x  = -->%lf\n",manager->player.walk_speed * cos(manager->player.rotation_angle) * manager->player.walk_direction);
	// manager->player.rotation_speed = radians(R_SPEED) * manager->time.delta_time;
	if (key == 124)
	{
		manager->player.turn_direction = 1;
		normalize_angle(&manager->player.rotation_angle);
		manager->player.rotation_angle += (manager->player.turn_direction * manager->player.rotation_speed);
		normalize_angle(&manager->player.rotation_angle);
	}
	else if (key == 123)
	{
		manager->player.turn_direction = -1;
		normalize_angle(&manager->player.rotation_angle);
		manager->player.rotation_angle += (manager->player.turn_direction * manager->player.rotation_speed);
		normalize_angle(&manager->player.rotation_angle);
	}
	else if (key == 125)
	{
		manager->player.walk_direction = -1;
		fx = manager->player.x + manager->player.walk_speed * cos(manager->player.rotation_angle) * manager->player.walk_direction;
		fy = manager->player.y + manager->player.walk_speed * sin(manager->player.rotation_angle) * manager->player.walk_direction;
		if (fy >= 0 && fy < manager->map->map_height * TILE_SIZE && fx >= 0 && fx < manager->map->map_width * TILE_SIZE && manager->map->map[(int)(fy / TILE_SIZE)][(int)(fx / TILE_SIZE)] != '1')
		{	
			manager->player.x += manager->player.walk_speed * cos(manager->player.rotation_angle) * manager->player.walk_direction;
			manager->player.y += manager->player.walk_speed * sin(manager->player.rotation_angle) * manager->player.walk_direction;
			// printf("hop\n");
		}
		// printf("hooray\n");
		
	}
	else if (key == 126)
	{
		manager->player.walk_direction = 1;
		fx = manager->player.x + manager->player.walk_speed * cos(manager->player.rotation_angle) * manager->player.walk_direction;
		fy = manager->player.y + manager->player.walk_speed * sin(manager->player.rotation_angle) * manager->player.walk_direction;
		if (fy >= 0 && fy < manager->map->map_height * TILE_SIZE && fx >= 0 && fx < manager->map->map_width * TILE_SIZE && manager->map->map[(int)(fy / TILE_SIZE)][(int)(fx / TILE_SIZE)] != '1')
		{	
			manager->player.x += manager->player.walk_speed * cos(manager->player.rotation_angle) * manager->player.walk_direction;
			manager->player.y += manager->player.walk_speed * sin(manager->player.rotation_angle) * manager->player.walk_direction;
		}
	}
	if (key == 53)
		exit(EXIT_SUCCESS);
	// draw(manager);
	return 0;
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

int draw(t_cub_manager *manager)
{
	double	angle;
	int		num_of_rays;
	if (manager->time.lastTick == 0)
	{
	// 	// puts("here\n");
	// 	// exit(0);
		manager->time.lastTick = get_time(manager);
	}
	// while (get_time(manager) < manager->time.lastTick + (1000.0 / 120.0))
	// {
		
	// // }
	manager->time.delta_time = (get_time(manager) - manager->time.lastTick) / 1000.0;
	manager->time.lastTick = get_time(manager);
	// printf("draw delta time: %lf\n", manager->time.delta_time);
	// int		i;
	// double	dist;
	// double	height;
	cast_all_rays(manager);
	// draw_empty_rect(manager, (t_draw_lines_struct){{0,0},{10 * mini_x, 6 * mini_y}, {WIDTH, HEIGHT}, 0x00ffffff});
	// draw_empty_rect(0, 0, 10 * mini_x, 6 * mini_y, WIDTH, HEIGHT, manager, 0x0000ff00);
	double mapsx = manager->player.x - 5.0 * TILE_SIZE;
	// double mapex = manager->player.x + 5.0 * TILE_SIZE;
	double mapsy = manager->player.y - 3.0 * TILE_SIZE;
	// double mapey = manager->player.y + 3.0 * TILE_SIZE;
	// for (int i = 0;i < mini_y * 6;++i)
	// {
	// 	for (int j = 0;j < mini_x * 10;++j)
	// 	{
	// 		int x = (mapsy + i) / TILE_SIZE;
	// 		int y = (mapsx + j) / TILE_SIZE;
	// 		if (x >= 0 && x < manager->map->map_height && y >= 0 && y < manager->map->map_width && manager->map->map[x][y] == '1')
	// 			cub_mlx_pixel_put(&manager->mlx_manager.img_data, j, i, mini_x * 10, mini_y * 6, 0x0029af00);
	// 	}
	// }

	// draw_line(round(mini_x * 10 / 2.0), round(mini_y * 6 / 2.0), mini_x * 10 / 2.0 + cos(manager->player.rotation_angle) * 10, mini_y * 6 / 2.0 + sin(manager->player.rotation_angle) * 10, mini_x * 10, mini_y * 6, manager, 0x00ff0000);
	// // draw_line(mini_x * 15 / 2.0, mini_y * 10 / 2.0, mini_x * 15 / 2.0 + cos(manager->player.rotation_angle) * 15, mini_y * 10 / 2.0 + sin(manager->player.rotation_angle) * 15, manager, 0x00ff0000);
	// angle = manager->player.rotation_angle - (radians(FOV / 2.0));
	// num_of_rays = WIDTH / (double) (WALL_STRIP_WIDTH);
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
	return (manager->map->map[y_index][x_index] == '1' || manager->map->map[y_index][x_index] == 'D');
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
	return (manager->map->map[y_index][x_index] == '1' || manager->map->map[y_index][x_index] == 'D');
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
	// if (tan(ray->rayAngle) != 0.0)
	// {
		xintercept = manager->player.x + (yintercept - manager->player.y) / tan(ray->rayAngle);//?radians or degrees
		ystep = TILE_SIZE;
		if (ray->isRayFacingUp)
			ystep *= -1;
		
		xstep = TILE_SIZE / tan(ray->rayAngle);//radians or degrees
		if (ray->isRayFacingLeft && xstep > 0)
			xstep *= -1;
		if (ray->isRayFacingRight && xstep < 0)
			xstep *= -1;
	// }
		
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
			//get the color here
			// printf("found vertical wall\n-----------------------------------\n");
			break ;
		}
		// printf("nextvx: %lf nexvy: %lf (%s:%d)\n-------------------\n", next_ver_touch_x, next_ver_touch_y, __FILE__, __LINE__);
		next_ver_touch_x += xstep;
		next_ver_touch_y += ystep;
	}
	

	//calculate both hor and vert distances and choose the smallest value
	
	double horz_hit_distance = 0.0;
	double vert_hit_distance = 0.0;
	// if (found_horz_wall_hit)
		horz_hit_distance = hypot(manager->player.x - horz_wall_hit_x, manager->player.y - horz_wall_hit_y);
	// else
	// 	horz_hit_distance = 3.4E+38;//double max value
	// if (found_ver_hit)
		vert_hit_distance = hypot(manager->player.x - ver_hit_x, manager->player.y - ver_hit_y);
	// else
	// 	vert_hit_distance = 3.4E+38;//double max value
	
	// printf("ver_dis: %f\thor_dis: %f       (renderer.c:461)\n", vert_hit_distance, horz_hit_distance);
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
		// printf("bjoj\n");
	}
	else if (found_horz_wall_hit)
	{
		ray->wallHitX = horz_wall_hit_x;
		ray->wallHitY = horz_wall_hit_y;
		ray->distance = horz_hit_distance;
		// printf("horz\n");	
	}
	else if (found_ver_hit)
	{
		// printf("ver\n");
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
// int	render_frame(t_cub_manager* manager)
// {
	
// }
void	cast_all_rays(t_cub_manager* manager)
{
	double	ray_angle;
	int		i;
	double	angle_increment;

	// manager->rays = (t_ray *)malloc(NUMBER_OF_RAYS * sizeof(t_ray));//TODO allocate needed memory for each ray before calling this funcition
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
	// render_3d_projected_walls(manager);
	rendering_3d_walls(manager);
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
			// printf("distance: %f\n", manager->rays[i].distance);
        double perpDistance = manager->rays[i].distance * cos(manager->rays[i].rayAngle - manager->player.rotation_angle);
		// printf("%lf, %lf, (%s:%d)\n", manager->rays[i].distance, cos(manager->rays[i].rayAngle - manager->player.rotation_angle), __FILE__, __LINE__);
        double distanceProjPlane = (WIDTH / 2.0) / tan(radians(FOV) / 2.0);
        double projectedWallHeight = ((double)TILE_SIZE / perpDistance) * distanceProjPlane;

        double	wallStripHeight = projectedWallHeight;

        double wallTopPixel = (HEIGHT / 2.0) - (wallStripHeight / 2.0);
        // wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

        double wallBottomPixel = (HEIGHT / 2.0) + (wallStripHeight / 2.0);
		// rendrering of the doors is dependent on the player position
		int px = manager->player.x / TILE_SIZE;
		int py = manager->player.y / TILE_SIZE;
		int l = 0;
		int r = 0;
		int u = 0;
		int d = 0;
		if (px >= 0 && px < manager->map->map_width && py >= 0 && py < manager->map->map_height)
		{
			if (manager->map->map[py][px] == 'D')
			{
				// puts("inside door");
				if (manager->rays[i].wasHitVertical)
				{
					if (manager->rays[i].isRayFacingRight)
					{
						// puts("facing right");
							if (manager->rays[i].wallHitX + 1 >= 0 && manager->rays[i].wallHitX +1 < manager->map->map_width * TILE_SIZE && manager->rays[i].wallHitY >= 0 && manager->rays[i].wallHitY < manager->map->map_height * TILE_SIZE)
							{
								if (manager->map->map[(int)((manager->rays[i].wallHitY) / TILE_SIZE)][(int)((manager->rays[i].wallHitX + 1)/ TILE_SIZE)] == 'D')
								{
									tex = DOOR;
									// puts("wall on right");
									// continue;
								}
								else
									tex = EAST;
								// }
							}
					}
					else
					{
						if (manager->rays[i].wallHitX - 1 >= 0 && manager->rays[i].wallHitX -1 < manager->map->map_width * TILE_SIZE && manager->rays[i].wallHitY >= 0 && manager->rays[i].wallHitY < manager->map->map_height * TILE_SIZE)
						{
							if (manager->map->map[(int)((manager->rays[i].wallHitY) / TILE_SIZE)][(int)((manager->rays[i].wallHitX - 1)/ TILE_SIZE)] == 'D')
							// {
								tex = DOOR;
								// continue;
							// }
							else
								tex = WEST;
						}
						// tex = WEST;
					}
				}
				else
				{
					if (manager->rays[i].isRayFacingUp)
					{
						if (manager->rays[i].wallHitX >= 0 && manager->rays[i].wallHitX < manager->map->map_width * TILE_SIZE && manager->rays[i].wallHitY - 1 >= 0 && manager->rays[i].wallHitY - 1 < manager->map->map_height * TILE_SIZE)
						{
							if (manager->map->map[(int)((manager->rays[i].wallHitY - 1) / TILE_SIZE)][(int)((manager->rays[i].wallHitX)/ TILE_SIZE)] == 'D')
							// {
								tex = DOOR;
								// continue;
							// }
						}
					}	
					else
					{
						if (manager->rays[i].wallHitX  >= 0 && manager->rays[i].wallHitX < manager->map->map_width * TILE_SIZE && manager->rays[i].wallHitY + 1 >= 0 && manager->rays[i].wallHitY + 1 < manager->map->map_height * TILE_SIZE)
						{
							if (manager->map->map[(int)((manager->rays[i].wallHitY + 1) / TILE_SIZE)][(int)((manager->rays[i].wallHitX)/ TILE_SIZE)] == 'D')
							// {
								tex = DOOR;
								// continue;
							// }
						}	
						
					}
				}
				
			}
			else
			{
				if (manager->rays[i].wasHitVertical)
				{
					if (manager->rays[i].isRayFacingRight)
					{
						if (manager->rays[i].wallHitX + 1 >= 0 && manager->rays[i].wallHitX +1 < manager->map->map_width * TILE_SIZE && manager->rays[i].wallHitY >= 0 && manager->rays[i].wallHitY < manager->map->map_height * TILE_SIZE)
						{
							if (manager->map->map[(int)((manager->rays[i].wallHitY) / TILE_SIZE)][(int)((manager->rays[i].wallHitX + 1)/ TILE_SIZE)] == 'D' && manager->rays[i].distance > 50)
							// {
								tex = DOOR;
								// continue;
							// }
							else if (manager->map->map[(int)((manager->rays[i].wallHitY) / TILE_SIZE)][(int)((manager->rays[i].wallHitX + 1)/ TILE_SIZE)] == 'D' && manager->rays[i].distance <= 50)
							{
								manager->map->map[(int)((manager->rays[i].wallHitY + 1) / TILE_SIZE)][(int)((manager->rays[i].wallHitX)/ TILE_SIZE)] = 'O';	
							}
						}
					}	
					else
					{
						if (manager->rays[i].wallHitX - 1 >= 0 && manager->rays[i].wallHitX -1 < manager->map->map_width * TILE_SIZE && manager->rays[i].wallHitY >= 0 && manager->rays[i].wallHitY < manager->map->map_height * TILE_SIZE)
						{
							if (manager->map->map[(int)((manager->rays[i].wallHitY) / TILE_SIZE)][(int)((manager->rays[i].wallHitX - 1)/ TILE_SIZE)] == 'D'&& manager->rays[i].distance > 50)
							// {
								tex = DOOR;
								// continue;
							// }
							else if (manager->map->map[(int)((manager->rays[i].wallHitY) / TILE_SIZE)][(int)((manager->rays[i].wallHitX - 1)/ TILE_SIZE)] == 'D'&& manager->rays[i].distance <= 50)
							{
								manager->map->map[(int)((manager->rays[i].wallHitY) / TILE_SIZE)][(int)((manager->rays[i].wallHitX - 1)/ TILE_SIZE)] = 'O';
							}
						}
					}
				}
				if (manager->rays[i].isRayFacingUp)
				{
					if (manager->rays[i].wallHitX >= 0 && manager->rays[i].wallHitX < manager->map->map_width * TILE_SIZE && manager->rays[i].wallHitY - 1 >= 0 && manager->rays[i].wallHitY - 1 < manager->map->map_height * TILE_SIZE)
					{
						if (manager->map->map[(int)((manager->rays[i].wallHitY - 1) / TILE_SIZE)][(int)((manager->rays[i].wallHitX)/ TILE_SIZE)] == 'D'&& manager->rays[i].distance > 50)
						// {
							tex = DOOR;
							// continue;
						// }
						else if (manager->map->map[(int)((manager->rays[i].wallHitY - 1) / TILE_SIZE)][(int)((manager->rays[i].wallHitX)/ TILE_SIZE)] == 'D'&& manager->rays[i].distance <= 50)
						{
							manager->map->map[(int)((manager->rays[i].wallHitY - 1) / TILE_SIZE)][(int)((manager->rays[i].wallHitX)/ TILE_SIZE)] = 'O';	
						}
						
					}
				}	
				else
				{
					if (manager->rays[i].wallHitX  >= 0 && manager->rays[i].wallHitX < manager->map->map_width * TILE_SIZE && manager->rays[i].wallHitY + 1 >= 0 && manager->rays[i].wallHitY + 1 < manager->map->map_height * TILE_SIZE)
					{
						if (manager->map->map[(int)((manager->rays[i].wallHitY + 1) / TILE_SIZE)][(int)((manager->rays[i].wallHitX)/ TILE_SIZE)] == 'D'&& manager->rays[i].distance > 50)
						// {
							tex = DOOR;
							// continue;
						// }
						else if (manager->map->map[(int)((manager->rays[i].wallHitY + 1) / TILE_SIZE)][(int)((manager->rays[i].wallHitX)/ TILE_SIZE)] == 'D'&& manager->rays[i].distance <= 50)
							manager->map->map[(int)((manager->rays[i].wallHitY + 1) / TILE_SIZE)][(int)((manager->rays[i].wallHitX)/ TILE_SIZE)] = 'O';
					}	
					
				}
			}
		}
		// if (manager->rays[i].isRayFacingRight)
		// {
		// 	if (manager->rays[i].wallHitX >= 0 && manager->rays[i].wallHitX <manager->map->map_width * TILE_SIZE && manager->rays[i].wallHitY + 1 >= 0 && manager->rays[i].wallHitY + 1  < manager->map->map_height * TILE_SIZE)
		// 	{
		// 		if (manager->map->map[(int)((manager->rays[i].wallHitY + 1) / TILE_SIZE)][(int)((manager->rays[i].wallHitX)/ TILE_SIZE)] == 'D')
		// 			continue;
		// 	}
		// }
		// else
		// {
		// 	if (manager->rays[i].wallHitX >= 0 && manager->rays[i].wallHitX <manager->map->map_width * TILE_SIZE && manager->rays[i].wallHitY >= 0 && manager->rays[i].wallHitY < manager->map->map_height * TILE_SIZE)
		// 	{
		// 		if (manager->map->map[(int)(manager->rays[i].wallHitY - 1) / TILE_SIZE][(int)((manager->rays[i].wallHitX)/ TILE_SIZE)] == 'D')
		// 			continue;
		// 		// if (manager->map->map[(int)(manager->rays[i].wallHitY / TILE_SIZE)][(int)(manager->rays[i].wallHitX / TILE_SIZE)] == 'D')
		// 		// {
		// 		// 	// printf("%d\n", i);
		// 		// 	tex = DOOR;
		// 		// }
		// 	}
			
		// }
		if (manager->rays[i].wasHitVertical)
		{
			if (manager->rays[i].isRayFacingLeft && tex == -1)
				tex = WEST;
			else if (tex == -1)
				tex = EAST;
			off_x = fmod(manager->rays[i].wallHitY, TILE_SIZE);
		}	
		// if (manager->rays[i].isRayFacingUp)
		// {
		// 	if (manager->rays[i].wallHitX + 1 >= 0 && manager->rays[i].wallHitX + 1 <manager->map->map_width * TILE_SIZE && manager->rays[i].wallHitY >= 0 && manager->rays[i].wallHitY < manager->map->map_height * TILE_SIZE)
		// 	{
		// 		if (manager->map->map[(int)((manager->rays[i].wallHitY) / TILE_SIZE)][(int)((manager->rays[i].wallHitX + 1) / TILE_SIZE)] == 'D')
		// 			continue;
		// 		// if (manager->map->map[(int)(manager->rays[i].wallHitY / TILE_SIZE)][(int)(manager->rays[i].wallHitX / TILE_SIZE)] == 'D')
		// 		// {
		// 		// 	// printf("%d\n", i);
		// 		// 	tex = DOOR;
		// 		// }
		// 	}
		// }
		// else
		// {
		// 	if (manager->rays[i].wallHitX - 1>= 0 && manager->rays[i].wallHitX - 1 <manager->map->map_width * TILE_SIZE && manager->rays[i].wallHitY >= 0 && manager->rays[i].wallHitY < manager->map->map_height * TILE_SIZE)
		// 	{
		// 		if (manager->map->map[(int)(manager->rays[i].wallHitY / TILE_SIZE)][(int)(manager->rays[i].wallHitX - 1) / TILE_SIZE] == 'D')
		// 			continue;
		// 		// if (manager->map->map[(int)(manager->rays[i].wallHitY / TILE_SIZE)][(int)(manager->rays[i].wallHitX / TILE_SIZE)] == 'D')
		// 		// {
		// 		// 	// printf("%d\n", i);
		// 		// 	tex = DOOR;
		// 		// }
		// 	}
		// }
		// if (!manager->rays[i].wasHitVertical)
		else
		{

			if (manager->rays[i].isRayFacingDown && tex == -1)
				tex = SOUTH;
			else if (tex == -1)
				tex = NORTH;
			off_x = fmod(manager->rays[i].wallHitX, TILE_SIZE);
		}
        // wallBottomPixel = wallBottomPixel > HEIGHT ? HEIGHT : wallBottomPixel;
		// tex = 3;
		off_x = off_x / TILE_SIZE * (double)manager->map->wall_textures[tex].wi;
		if (off_x < 0 || off_x >= manager->map->wall_textures[tex].wi)
			panic("off_x must be bounded");
        // render the wall from wallTopPixel to wallBottomPixel
		for (int j = wallTopPixel; j < wallBottomPixel - 1;++j)
		{
			off_y = (j - (int)wallTopPixel) / wallStripHeight * manager->map->wall_textures[tex].hi;
			if (off_y < 0 || off_y >= manager->map->wall_textures[tex].hi)
			{
				panic("off_y must be bounded");
			}
			// cub_mlx_pixel_put(&manager->mlx_manager.img_data, i * WALL_STRIP_WIDTH, j, WIDTH, HEIGHT, 0x00ffffff);
			p.point.x = i * WALL_STRIP_WIDTH;
			p.point.y = j;
			// printf("-->%d\n", off_y);
			// printf("ray %d\n", i);
			p.color = *(((int *)manager->map->wall_textures[tex].tex_img_data.addr + ((int)off_y * manager->map->wall_textures[tex].wi + (int)off_x)));
			// puts("fd");
			cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);

		}
			
    }
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
	char **map = malloc(sizeof(char *) * manager.map->map_height);
	for (int i = 0; i < manager.map->map_height; ++i)
	{
		map[i] = malloc(manager.map->map_width);
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
	manager.time.delta_time = 0;
	manager.time.lastTick = 0;
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
	manager.map->wall_textures[NORTH].img = mlx_xpm_file_to_image(manager.mlx_manager.mlx, "/Users/yakhoudr/cube3d/assets/wall_north.xpm", &manager.map->wall_textures[NORTH].wi, &manager.map->wall_textures[NORTH].hi);
	manager.map->wall_textures[NORTH].tex_img_data.addr = mlx_get_data_addr(manager.map->wall_textures[NORTH].img, &manager.map->wall_textures[NORTH].tex_img_data.bits_per_pixel,
	&manager.map->wall_textures[NORTH].tex_img_data.line_length, &manager.map->wall_textures[NORTH].tex_img_data.endian);
	// printf("%p\n", manager.map->wall_textures[NORTH].tex_img_data.addr);
	//
	manager.map->wall_textures[SOUTH].img = mlx_xpm_file_to_image(manager.mlx_manager.mlx, "/Users/yakhoudr/cube3d/assets/wall_south.xpm", &manager.map->wall_textures[SOUTH].wi, &manager.map->wall_textures[SOUTH].hi);
	// printf("%p\n", manager.map->wall_textures[SOUTH].tex_img_data.addr);
	manager.map->wall_textures[SOUTH].tex_img_data.addr = mlx_get_data_addr(manager.map->wall_textures[SOUTH].img, &manager.map->wall_textures[SOUTH].tex_img_data.bits_per_pixel,
	&manager.map->wall_textures[SOUTH].tex_img_data.line_length, &manager.map->wall_textures[SOUTH].tex_img_data.endian);
	//
	manager.map->wall_textures[EAST].img = mlx_xpm_file_to_image(manager.mlx_manager.mlx, "/Users/yakhoudr/cube3d/assets/wall_east.xpm", &manager.map->wall_textures[EAST].wi, &manager.map->wall_textures[EAST].hi);
	manager.map->wall_textures[EAST].tex_img_data.addr = mlx_get_data_addr(manager.map->wall_textures[EAST].img, &manager.map->wall_textures[EAST].tex_img_data.bits_per_pixel,
	&manager.map->wall_textures[EAST].tex_img_data.line_length, &manager.map->wall_textures[EAST].tex_img_data.endian);
	// printf("%p\n", manager.map->wall_textures[EAST].tex_img_data.addr);
	//
	manager.map->wall_textures[WEST].img = mlx_xpm_file_to_image(manager.mlx_manager.mlx, "/Users/yakhoudr/cube3d/assets/wall_west.xpm", &manager.map->wall_textures[WEST].wi, &manager.map->wall_textures[WEST].hi);
	manager.map->wall_textures[WEST].tex_img_data.addr = mlx_get_data_addr(manager.map->wall_textures[WEST].img, &manager.map->wall_textures[WEST].tex_img_data.bits_per_pixel,
	&manager.map->wall_textures[WEST].tex_img_data.line_length, &manager.map->wall_textures[WEST].tex_img_data.endian);
	// printf("hre%d\n", manager.map->wall_textures[1].tex_img_data.addr);
	// printf("%p\n", manager.map->wall_textures[WEST].tex_img_data.addr);
	//
	manager.map->wall_textures[DOOR].img = mlx_xpm_file_to_image(manager.mlx_manager.mlx, "/Users/yakhoudr/cube3d/assets/door1.xpm", &manager.map->wall_textures[DOOR].wi, &manager.map->wall_textures[DOOR].hi);
	manager.map->wall_textures[DOOR].tex_img_data.addr = mlx_get_data_addr(manager.map->wall_textures[DOOR].img, &manager.map->wall_textures[DOOR].tex_img_data.bits_per_pixel,
	&manager.map->wall_textures[DOOR].tex_img_data.line_length, &manager.map->wall_textures[DOOR].tex_img_data.endian);
	manager.rays = malloc(NUMBER_OF_RAYS * sizeof(t_ray));
	// draw(&manager);
	mlx_loop_hook(manager.mlx_manager.mlx, draw, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, ON_KEYDOWN, 1L<<0, controls, &manager);
	mlx_loop(manager.mlx_manager.mlx);
	return (0);
}