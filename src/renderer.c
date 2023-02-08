/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:26:42 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/02/09 00:35:51 by osallak          ###   ########.fr       */
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
	if (key == 124)
		manager->player.rotate = 1;
	else if (key == 123)
		manager->player.rotate = -1;
	else if (key == 125)
	{
		manager->player.walk_direction = -1;
		manager->player.move_y = true;
	}
	if (key == 126)
	{
		manager->player.move_y = true;
		manager->player.walk_direction = 1;
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
	double fx;
	double fy;

	if (key == 124)
	{
		manager->player.rotate = 0;
	}
	else if (key == 123)
	{
		manager->player.rotate = 0;
	}
	else if (key == 125)
	{
		manager->player.move_y = false;
		manager->player.walk_direction = 0;
		// printf("hooray\n");
		
	}
	else if (key == 126)
	{
		manager->player.walk_direction = 0;
		manager->player.move_y = false;
	}
	if (key == KEY_A || key == KEY_D)
		manager->__move_slideways = 0;
	if (key == KEY_SPACE)
	{
		manager->weapons.gun_state = STAND;
		manager->weapons.gun_frames = 0;
	}
	// draw(manager);
	return 0;
}


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

void	move_slideways(t_cub_manager* manager, double *nx, double *ny, int slide_direction)
{
	double	rotation_angle;

	normalize_angle(&rotation_angle);
	// printf ("slide_direction: %d\n", slide_direction);
	rotation_angle = manager->player.rotation_angle + radians(90) * slide_direction;
	normalize_angle(&rotation_angle);
	*nx = manager->player.x + cos(rotation_angle ) * manager->player.walk_speed * manager->time.delta_time ;
	*ny = manager->player.y + sin(rotation_angle) * manager->player.walk_speed * manager->time.delta_time ;
	printf (slide_direction == 1 ? "right" : "left");
	printf("\n");
}

void	move_player(t_cub_manager *manager)
{
	double nx = 0;
	double ny = 0;
	bool increment = true;
	if (manager->__move_slideways)
		move_slideways(manager ,&nx, &ny, manager->__move_slideways);
	else if (manager->player.move_y)
	{
		ny = manager->player.y + sin(manager->player.rotation_angle) * manager->player.walk_direction * manager->player.walk_speed * manager->time.delta_time;
		nx = manager->player.x + cos(manager->player.rotation_angle) * manager->player.walk_direction * manager->player.walk_speed * manager->time.delta_time;	
	}
	// printf(manager->__move_slideways == 0 ? "nothing" : manager->__move_slideways == 1 ? "right" : "left");
	// printf("nx: %f, ny: %f\n", nx, ny);
	double da = nx;
	double db = ny;
	double dx = nx - manager->player.x;
	double dy = ny - manager->player.y;
	double steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
	int st = round(steps);

	// Calculate the x and y increments for each step
	double xIncrement = dx / (steps);
	double yIncrement = dy / (steps);
  	for (int i = 0; i <= st; ++i) {	
		int x = nx / TILE_SIZE;
		int y = ny / TILE_SIZE;
		if (x >= 0 && x < manager->map->map_width && y >= 0 && y < manager->map->map_height && manager->map->map[y][x] == '1')
		{
			increment = false;
			false;
		}
		nx += xIncrement;
		ny += yIncrement;
	}
	if (increment)
	{
		manager->player.y = db;
		manager->player.x = da;
		// printf("%lf\n", (ny));
		// puts("here");
	
	}
	if (manager->player.rotate)
	{
		// puts("here\n");
		// normalize_angle(&manager->player.rotation_angle);
		manager->player.rotation_angle += (manager->player.rotation_speed * manager->time.delta_time * manager->player.rotate);
		normalize_angle(&manager->player.rotation_angle);
		// manager->mouse_move = false;
		// manager->player.rotate = false;
	}
	if (manager->mouse_move)
	{
		controls_up(123, manager);
		controls_up(124, manager);
		manager->mouse_move = false;
	}
}

int draw(t_cub_manager *manager)
{
	// double	angle;
	int		num_of_rays;
	static int frames;
	static long goal;
	t_draw_point_struct p;

	if (manager->time.lastTick == 0)
	{
	// 	// puts("here\n");
	// 	// exit(0);
		manager->time.lastTick = get_time(manager);
	}
	// while (get_time(manager) < manager->time.lastTick + (1000.0 / 30.0))
	// {
		
	// }
	if (!goal)
		goal = get_time(manager) + 1000;
	if (goal <= get_time(manager))
	{
		goal = get_time(manager) + 1000;
		printf("fps: %ld\n", frames);
		frames = 0;
	}
	manager->time.delta_time = (get_time(manager) - manager->time.lastTick) / 1000.0;
	manager->time.lastTick = get_time(manager);
	move_player(manager);
	cast_all_rays(manager);
	double mapsx = manager->player.x - 5.0 * TILE_SIZE;
	mapsx = mapsx / TILE_SIZE * mini_x;
	// double mapex = manager->player.x + 5.0 * TILE_SIZE;
	double mapsy = manager->player.y - 3.0 * TILE_SIZE;
	mapsy = mapsy / TILE_SIZE * mini_y;
	for (int i = 0;i < mini_y * 6;++i)
	{
		p.point.y = i;
		for (int j = 0;j < mini_x * 10;++j)
		{
			p.point.x = j;
			int x = (mapsy + i) / mini_y;
			int y = (mapsx + j) / mini_x;
			if (x >= 0 && x < manager->map->map_height && y >= 0 && y < manager->map->map_width && manager->map->map[x][y] == '1')
				p.color = 0x00ffffff;
			else if (x >= 0 && x < manager->map->map_height && y >= 0 && y < manager->map->map_width && (manager->map->map[x][y] == 'D' || manager->map->map[x][y] == 'O'))
				p.color = 0x00ff0000;
			else
				p.color = 0x00000000;
			cub_mlx_pixel_put(&manager->mlx_manager.img_data,p);
		}
	}
	double angle = manager->player.rotation_angle - (radians(FOV / 2.0));
	t_draw_lines_struct line;
	line.color = 0x0000ff00;
	line.start.x = round(mini_x * 10 / 2.0);
	line.start.y = round(mini_y * 6 / 2.0);
	double incr = radians(FOV) / (mini_x * 10.0);
	for (int i = 0;i < mini_x * 10;++i)
	{
		line.end.x = line.start.x + cos(angle) * 30;
		line.end.y = line.start.y + sin(angle) * 30;
		draw_line(manager, line);
		angle += incr;
		normalize_angle(&angle);
	}
	// draw_line(round(mini_x * 10 / 2.0), round(mini_y * 6 / 2.0), mini_x * 10 / 2.0 + cos(manager->player.rotation_angle) * 10, mini_y * 6 / 2.0 + sin(manager->player.rotation_angle) * 10, mini_x * 10, mini_y * 6, manager, 0x00ff0000);
	// // draw_line(mini_x * 15 / 2.0, mini_y * 10 / 2.0, mini_x * 15 / 2.0 + cos(manager->player.rotation_angle) * 15, mini_y * 10 / 2.0 + sin(manager->player.rotation_angle) * 15, manager, 0x00ff0000);
	// angle = manager->player.rotation_angle - (radians(FOV / 2.0));
	// num_of_rays = WIDTH / (double) (WALL_STRIP_WIDTH);
	mlx_put_image_to_window(manager->mlx_manager.mlx, manager->mlx_manager.mlx_window, manager->mlx_manager.img_data.img, 0, 0);
	++frames;
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
	double d = __distance(manager->player.x, manager->player.y, x, y);
	return (manager->map->map[y_index][x_index] == '1' || (manager->map->map[y_index][x_index] == 'D' && (d > TILE_SIZE)));
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
	double d = __distance(manager->player.x, manager->player.y, x, y);
	return (manager->map->map[y_index][x_index] == '1' || (manager->map->map[y_index][x_index] == 'D' && (d > TILE_SIZE)));
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
		horz_hit_distance = __distance(manager->player.x , manager->player.y ,horz_wall_hit_x, horz_wall_hit_y);
	// else
	// 	horz_hit_distance = 3.4E+38;//double max value
	// if (found_ver_hit)
		vert_hit_distance = __distance(manager->player.x , manager->player.y ,ver_hit_x, ver_hit_y);
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



double distance(double x, double y, double a, double b)
{
	return (sqrt((a - x) * (a - x) + (b - y) * (b - y)));
}

void	*xalloc(size_t size)
{
	void *ptr = malloc(size);
	if (!ptr)
		exit(1);
	return (ptr);
}

t_door	*create_door(int x, int y)
{
	t_door	*door;
	door = xalloc(sizeof(t_door));
	door->x = x;
	door->y = y;
	door->next = 0x0;
	door->prev = 0x0;
	return door;
}

void	add_door(t_door **head, t_door *door, t_door **tail)
{
	t_door *tmp;
	if (head == 0x0 || !door)
		exit(1);
	if (*head == 0x0 || *tail == 0x0)
	{
		*head = door;
		*tail = door;
		return ;
	}
	tmp = *head;
	while (tmp->next != 0x0)
		tmp = tmp->next;
	door->prev = tmp;
	tmp->next = door;
	*tail = door;
}

void delete_door(t_door **head, t_door **door, t_door *tail)
{
	// t_door *prev;
	// t_door *next;
	if (*door == *head)
		*head = (*head)->next;
	else if (*door == tail)
	{
		(*door)->prev->next = 0x0;
	}
	else
	{
		if ((*door)->prev && (*door)->next)
		{
			(*door)->prev->next = (*door)->next;
			(*door)->next->prev = (*door)->prev;
		}
	}
	free(*door);
    *door = 0x0;
}

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
		// ray_angle = manager->player.rotation_angle + atan(( i - NUMBER_OF_RAYS / 2)) / ((WIDTH / 2.0) / tan(radians(FOV) / 2.0));
		normalize_angle(&ray_angle);
		manager->rays[i].rayAngle =  ray_angle;
		__initialize_ray_attributes(&manager->rays[i]);
		manager->door = manager->head;
		while (manager->door)
		{
			// printf("%d\t%d\n", manager->door->x, manager->door->y);
			double dist = distance(manager->player.x, manager->player.y, manager->door->x * TILE_SIZE, manager->door->y * TILE_SIZE);
			manager->next = manager->door->next;
			if (dist > TILE_SIZE * 2)
			{
				// puts("here\n");
				manager->map->map[manager->door->y][manager->door->x] = 'D';
				delete_door(&manager->head, &manager->door, manager->tail);
			}
			manager->door = manager->next;
		}
		cast(&manager->rays[i], manager);
		ray_angle += angle_increment;


	}
	// render_3d_projected_walls(manager);
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
		int x = 0;
		int y = 0;
		if (manager->rays[i].wasHitVertical)
		{
			off_x = fmod(manager->rays[i].wallHitY, TILE_SIZE);
			// checking for vertical doors;
			y = manager->rays[i].wallHitY / TILE_SIZE;
			if (manager->rays[i].isRayFacingRight)
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
			if (manager->rays[i].isRayFacingDown)
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
		// printf("tex: %d\n", tex);
		if (tex != -1)
		{
			off_x = off_x / TILE_SIZE * (double)manager->map->wall_textures[tex].wi;
			for (int j = wallTopPixel; j < wallBottomPixel - 1;++j)
			{
				off_y = (j - (int)wallTopPixel) / wallStripHeight * manager->map->wall_textures[tex].hi;
				p.point.x = i * WALL_STRIP_WIDTH;
				p.point.y = j;
				p.color = *(((int *)manager->map->wall_textures[tex].tex_img_data.addr + ((int)off_y * manager->map->wall_textures[tex].wi + (int)off_x)));
				cub_mlx_pixel_put(&manager->mlx_manager.img_data, p);

			}
		}
    }
	__render_gun(manager);
}


//TODO initialize mouse x
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

int	__leave_notify(t_cub_manager *manager)
{
	(void)manager;
	puts("leave notify");
	return (0);
}
int	__destroy(void)
{
	exit(EXIT_SUCCESS);
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

int	__mouse_press(int button, int x, int y, t_cub_manager *manager)
{
	if (button == 1 && x > 0 && x < WIDTH && y > 0 && y < HEIGHT)
		controls(KEY_SPACE, manager);
	if (button == 2 && x > 0 && x < WIDTH && y > 0 && y < HEIGHT)
		controls(KEY_R, manager);
	return (0);
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
	manager.player.walk_speed = W_SPEED;
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
	manager.map->wall_textures[NORTH].img = mlx_xpm_file_to_image(manager.mlx_manager.mlx, "/Users/osallak/Desktop/normed-version-cub3d/assets/high_quality_wall.xpm", &manager.map->wall_textures[NORTH].wi, &manager.map->wall_textures[NORTH].hi);
	manager.map->wall_textures[NORTH].tex_img_data.addr = mlx_get_data_addr(manager.map->wall_textures[NORTH].img, &manager.map->wall_textures[NORTH].tex_img_data.bits_per_pixel,
	&manager.map->wall_textures[NORTH].tex_img_data.line_length, &manager.map->wall_textures[NORTH].tex_img_data.endian);
	// printf("%p\n", manager.map->wall_textures[NORTH].tex_img_data.addr);
	//
	manager.map->wall_textures[SOUTH].img = mlx_xpm_file_to_image(manager.mlx_manager.mlx, "/Users/osallak/Desktop/normed-version-cub3d/assets/high_quality_wall.xpm", &manager.map->wall_textures[SOUTH].wi, &manager.map->wall_textures[SOUTH].hi);
	// printf("%p\n", manager.map->wall_textures[SOUTH].tex_img_data.addr);
	manager.map->wall_textures[SOUTH].tex_img_data.addr = mlx_get_data_addr(manager.map->wall_textures[SOUTH].img, &manager.map->wall_textures[SOUTH].tex_img_data.bits_per_pixel,
	&manager.map->wall_textures[SOUTH].tex_img_data.line_length, &manager.map->wall_textures[SOUTH].tex_img_data.endian);
	//
	manager.map->wall_textures[EAST].img = mlx_xpm_file_to_image(manager.mlx_manager.mlx, "/Users/osallak/Desktop/normed-version-cub3d/assets/high_quality_wall.xpm", &manager.map->wall_textures[EAST].wi, &manager.map->wall_textures[EAST].hi);
	manager.map->wall_textures[EAST].tex_img_data.addr = mlx_get_data_addr(manager.map->wall_textures[EAST].img, &manager.map->wall_textures[EAST].tex_img_data.bits_per_pixel,
	&manager.map->wall_textures[EAST].tex_img_data.line_length, &manager.map->wall_textures[EAST].tex_img_data.endian);
	// printf("%p\n", manager.map->wall_textures[EAST].tex_img_data.addr);
	//
	manager.map->wall_textures[WEST].img = mlx_xpm_file_to_image(manager.mlx_manager.mlx, "/Users/osallak/Desktop/normed-version-cub3d/assets/high_quality_wall.xpm", &manager.map->wall_textures[WEST].wi, &manager.map->wall_textures[WEST].hi);
	manager.map->wall_textures[WEST].tex_img_data.addr = mlx_get_data_addr(manager.map->wall_textures[WEST].img, &manager.map->wall_textures[WEST].tex_img_data.bits_per_pixel,
	&manager.map->wall_textures[WEST].tex_img_data.line_length, &manager.map->wall_textures[WEST].tex_img_data.endian);
	// printf("hre%d\n", manager.map->wall_textures[1].tex_img_data.addr);
	// printf("%p\n", manager.map->wall_textures[WEST].tex_img_data.addr);
	//
	manager.map->wall_textures[DOOR].img = mlx_xpm_file_to_image(manager.mlx_manager.mlx, "/Users/osallak/Desktop/normed-version-cub3d/assets/door1.xpm", &manager.map->wall_textures[DOOR].wi, &manager.map->wall_textures[DOOR].hi);
	manager.map->wall_textures[DOOR].tex_img_data.addr = mlx_get_data_addr(manager.map->wall_textures[DOOR].img, &manager.map->wall_textures[DOOR].tex_img_data.bits_per_pixel,
	&manager.map->wall_textures[DOOR].tex_img_data.line_length, &manager.map->wall_textures[DOOR].tex_img_data.endian);
	manager.rays = malloc(NUMBER_OF_RAYS * sizeof(t_ray));
	__load_gun_textures(&manager);
	manager.rays = malloc(NUMBER_OF_RAYS * sizeof(t_ray));
	manager.weapons.gun_state = STAND;
	manager.weapons.gun_frames = 0;
	manager.weapons.gun_type = PISTOL;
	// draw(&manager);
	manager.door = 0x0;
	manager.head = 0x0;
	manager.tail = 0x0;
	manager.next = 0x0;
	manager.player.move_x = false;
	manager.player.move_y = false;
	manager.player.rotate = false;
	// TODO: implement delta time
	// TODO: minimap with variable size
	// TODO: change the texture before rendering the walls
	manager.mouse_x = WIDTH / 2;
	manager.mouse_move = false;
	manager.__move_slideways = 0;
	mlx_loop_hook(manager.mlx_manager.mlx, draw, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, ON_KEYDOWN, 1L<<0, controls, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, ON_KEYUP, 1L<<0, controls_up, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, 6, 1, __mouse_move, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, 8,1, __leave_notify, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, 17, 0, __destroy, &manager);
	mlx_hook(manager.mlx_manager.mlx_window, 4, 1, __mouse_press, &manager);
	mlx_loop(manager.mlx_manager.mlx);
	return (0);
}