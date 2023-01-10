/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:26:42 by yakhoudr          #+#    #+#             */
/*   Updated: 2023/01/10 11:53:41 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"
#define WIDTH 1080
#define HEIGHT 720

inline 	double radians(double angle)

{
	return (angle * (M_PI / (double)180));
}

inline double dist(int x1, int x2, int y1, int y2)
{
    // Calculating distance
    return sqrt(pow(x2 - x1, 2)
                + pow(y2 - y1, 2) * 1.0);
}
int max(int a, int b)
{
    if (a > b)
        return a;
    else
	    return b;
}

void draw_line(double x0, double y0, double x1, double y1, void *mlx, void *window, int color)
{
  // Calculate the difference between the starting and ending x and y coordinates
  double dx = x1 - x0;
  double dy = y1 - y0;

  // Determine the number of steps required to draw the line
  double steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
  int st = round(steps);

  // Calculate the x and y increments for each step
  double xIncrement = dx / (steps);
  double yIncrement = dy / (steps);

  // Set the starting x and y coordinates
  double x = x0;
  double y = y0;

  // Loop through the number of steps required to draw the line
  for (int i = 0; i <= st; ++i) {
    // Set the color of the pixel at (x, y)
    mlx_pixel_put(mlx,window, round(x), round(y), color);

    // Increment the x and y coordinates
    x += xIncrement;
    y += yIncrement;
  }
}

void	clear_window(void	*mlx, void	*mlx_window, int color)
{
	int	i;
	int	j;

	i = -1;
	while (++i < HEIGHT)
	{
		j = -1;
		while (++j < WIDTH)
			mlx_pixel_put(mlx, mlx_window, j, i, color);
	}
}

void	draw_empty_rect(int x, int y, int width, int height, void *mlx, void *mlx_window, int color)
{
	long	i;

	i = -1;
	while (++i < width)
	{
		mlx_pixel_put(mlx, mlx_window, x + i, y, color);
		mlx_pixel_put(mlx, mlx_window, x + i, y + height, color);
	}
	i = -1;
	while (++i < height)
    {
		mlx_pixel_put(mlx, mlx_window, x, y + i, color);
		mlx_pixel_put(mlx, mlx_window,x + width, y + i, color);
	}
}

void	draw_full_rect(int x, int y, int width,int height, void *mlx, void *mlx_window, int color)
{
	long	i;
	long	j;

	i = -1;
	while (++i < height)
	{
		j = -1;
		while (++j < width)
			mlx_pixel_put(mlx, mlx_window, x + j, y + i, color);

	}
}
#define SCALING_FACTOR ((double)0.3)
#define NORMAL_TILE 30
#define MAP_TILE ((double) NORMAL_TILE)
#define LINE_LENGTH (30 )




void	draw_circle(double ox, double oy, double radius, void * mlx, void * mlx_window, int color)
{
	for (double i = 0;i < 360; ++i)
	{
		mlx_pixel_put(mlx, mlx_window, ox + radius * cos(radians(i)), oy + radius * sin(radians(i)), color);
	}
}

void	get_direction(int *facing_up, int *facing_left, double angle)
{
	if (angle > 0 && angle < M_PI)
		*facing_up = -1;
	if (angle < radians(90)|| angle > radians(270))
		*facing_left = -1;
}

double	calculate_line_intersection(double angle, t_cub_manager *manager, double *wall_hitx
, double *wall_hity)
{
	int hhit = 0;
	int vhit = 0;
	int facing_up = 1;
	int facing_left = 1;
	get_direction(&facing_up, &facing_left, angle);

	// find the horizontal first intersection point
	double fhintery = floor(manager->player.y / NORMAL_TILE) * NORMAL_TILE;
	if (facing_up == -1)
	{
		fhintery += NORMAL_TILE;
	}
	double fhinterx = manager->player.x + ((fhintery - manager->player.y) / tan(angle));
	// find the amount of increment each time
	double hdeltay = NORMAL_TILE;
	if (facing_up == 1)
		hdeltay = -hdeltay;
	double hdeltax = NORMAL_TILE / tan(angle);
	// modifying the deltas depending on the direction
	if (facing_left == 1 && hdeltax > 0)
	    hdeltax = -hdeltax;
	if (facing_left == -1 && hdeltax < 0)
		hdeltax = -hdeltax;
	double nexthx = fhinterx;
	double nexthy = fhintery;
	while (nexthy >= 0 && nexthy < manager->map->map_height * NORMAL_TILE && nexthx >= 0 && nexthx < (int) ft_strlen(manager->map->map[(int)(nexthy / NORMAL_TILE)]) * NORMAL_TILE)
	{
		if (facing_up == 1)
		{
			if (manager->map->map[(int)((nexthy - 1) / (double)NORMAL_TILE)][(int)(nexthx / (double)NORMAL_TILE)] == '1')
			{
                hhit = 1;
				break;
			}
		}	
		else if (manager->map->map[(int)((nexthy / (double)NORMAL_TILE))][(int)(nexthx / (double)NORMAL_TILE)] == '1')
		{
			hhit = 1;
			break;
		}
		nexthx += hdeltax;
		nexthy += hdeltay;
	}
	// check for vertical intersections
	// finding the first intersection
	double vinterx = floor(manager->player.x / (double)NORMAL_TILE) * (double)NORMAL_TILE;
	if (facing_left == -1)
		vinterx += NORMAL_TILE;
	double vintery = manager->player.y + (tan(angle) * (vinterx - manager->player.x));
	// calculating the deltas
	double vdeltax = NORMAL_TILE;
	if (facing_left == 1)
		vdeltax = -vdeltax;
	double vdeltay = tan(angle) * (double)NORMAL_TILE;
	// changing the deltas depeding on the deltas
	if (facing_up == 1 && vdeltay > 0)
		vdeltay = -vdeltay;
	if (facing_up == -1 && vdeltay < 0)
		vdeltay = -vdeltay;
	double nextvx = vinterx;
	double nextvy = vintery;
	while (nextvy >= 0 && nextvy < manager->map->map_height * NORMAL_TILE && nextvx >= 0 && nextvx < (int) ft_strlen(manager->map->map[(int)(nextvy / NORMAL_TILE)]) * NORMAL_TILE)
	{
		if (facing_left == 1)
		{
			if (manager->map->map[(int)((nextvy / (double)NORMAL_TILE))][(int)((nextvx - 1) / (double)NORMAL_TILE)] == '1')
			{
                vhit = 1;
				break;
			}
		}
		else 
		{	
			if (manager->map->map[(int)((nextvy / (double)NORMAL_TILE))][(int)(nextvx / (double)NORMAL_TILE)] == '1')
			{
                vhit = 1;
				break;
			}
		}
		nextvx += vdeltax;
		nextvy += vdeltay;
	}
	if (vhit && hhit)
	{
		double d1 = dist(manager->player.x , nexthx, manager->player.y , nexthy);
		double d2 = dist(manager->player.x , nextvx, manager->player.y , nextvy);
		if (d1 <= d2)
		{
			*wall_hitx = nexthx;
			*wall_hity = nexthy;
			return d1;
		}
		else
		{
			*wall_hitx = nextvx;
			*wall_hity = nextvy;
			return d2 ;
		}
	}
	else if (vhit)
	{
		double d = dist(manager->player.x , nextvx, manager->player.y , nextvy);
		*wall_hitx = nextvx;
		*wall_hity = nextvy;
		return d;
	}
	else if (hhit)
	{
		double d = dist(manager->player.x , nexthx, manager->player.y , nexthy);
		*wall_hitx = nexthx;
		*wall_hity = nexthy;
		return d;
	}
	return 0;
}

void normalize_angle(double *ang)
{
	if (*ang > 2 * M_PI)
		*ang -= 2 * M_PI;
	if (*ang < 0)
		*ang += 2 * M_PI;
}

typedef struct s_dpair
{
	double x;
	double y;
	struct s_dpair *next;
}	t_dpair;

t_dpair *create_dpair(double x, double y)
{
	t_dpair *dpair = (t_dpair *)malloc(sizeof(t_dpair));
	if (!dpair)
		exit(1);
	dpair->x = x;
    dpair->y = y;
	dpair->next = 0x0;
	return dpair;
}

void	dpair_add_back(t_dpair **head, t_dpair *new)
{
	if (!head || !new)
		return ;
	if (!*head)
		*head = new;
	else
	{
		t_dpair *p = *head;
		while (p->next)
		{
			p = p->next;
		}
		p->next = new;
	}	
}
#define WALL_STRIP_WIDTH 1

void	redraw(t_cub_manager *manager)
{
	t_dpair *wall_hits = 0x0;
	clear_window(manager->mlx_manager.mlx, manager->mlx_manager.mlx_window, 0);
	long i = -1;
	double angle = manager->player.rotation_angle - radians(FOV) / 2.;
	int num_of_rays = manager->map->map_width * NORMAL_TILE / WALL_STRIP_WIDTH; // divided by LEN_SINGLE_RAY
	double wall_hitx = 0, wall_hity = 0;
	double distProj = (manager->map->map_width * NORMAL_TILE * 1.0 / tan(radians((double)FOV / 2.0)));
	// for (int i = 0; i < 1; ++i)
	for (int i = 0; i < num_of_rays; ++i)
    {
		normalize_angle(&angle);
		double dist = calculate_line_intersection(angle, manager, &wall_hitx, &wall_hity);
		dist = dist * cos(angle - manager->player.rotation_angle);
		double height = (NORMAL_TILE / dist) * distProj;
		double alpha = dist / (double)(manager->map->map_width * NORMAL_TILE);
		// draw 3D
		draw_full_rect(i * WALL_STRIP_WIDTH, round(manager->map->map_height * NORMAL_TILE * 1.0 / 2.0) - (height / 2.0), WALL_STRIP_WIDTH, round(height), manager->mlx_manager.mlx, manager->mlx_manager.mlx_window, create_trgb((int)(alpha * 255), 255, 255, 255));
		dpair_add_back(&wall_hits, create_dpair((wall_hitx), (wall_hity)));	
		// draw_line(round(manager->player.x), round(manager->player.y), (wall_hitx), (wall_hity), manager->mlx_manager.mlx, manager->mlx_manager.mlx_window, 0x0000ff00);
		angle += (radians(FOV) / (double)num_of_rays);
	}
	while (++i < manager->map->map_height)
	{
		long j = -1;
		int ii = i * MAP_TILE * SCALING_FACTOR;
		while (++j < ft_strlen(manager->map->map[i]))
		{
			double jj = j * MAP_TILE * SCALING_FACTOR;
			if (manager->map->map[i][j] == '1')
				draw_full_rect(jj , ii, MAP_TILE * SCALING_FACTOR, MAP_TILE * SCALING_FACTOR, manager->mlx_manager.mlx, manager->mlx_manager.mlx_window, create_trgb(1, 0, 255,0));
				//  draw_empty_rect(jj, ii, MAP_TILE, MAP_TILE, manager->mlx_manager.mlx, manager->mlx_manager.mlx_window, manager->map->wall_colors);
		}
	}
	// draw_line( manager->player.x * SCALING_FACTOR,  manager->player.y * SCALING_FACTOR,
	// SCALING_FACTOR * manager->player.x + LINE_LENGTH * cos(manager->player.rotation_angle),
	// SCALING_FACTOR * manager->player.y + LINE_LENGTH * sin(manager->player.rotation_angle),
	// manager->mlx_manager.mlx, manager->mlx_manager.mlx_window, 0x00ff0000);
	for (int i = 0;i < num_of_rays;++i)
	// for (int i = 0;i < 1;++i)
	{
		// printf("%lf\n", round(wall_hits->x * SCALING_FACTOR));
		draw_line(round(manager->player.x * SCALING_FACTOR) , round(manager->player.y * SCALING_FACTOR), round(wall_hits->x * SCALING_FACTOR), round(wall_hits->y * SCALING_FACTOR), manager->mlx_manager.mlx, manager->mlx_manager.mlx_window, 0x00ab0000);
		wall_hits = wall_hits->next;
	}
}
int controls(int keycode, t_cub_manager *manager)
{
	if (keycode == 123)
	{
		manager->player.turn_direction = -1;
		manager->player.rotation_angle += manager->player.rotation_speed * manager->player.turn_direction;
		if (manager->player.rotation_angle > 2. * M_PI)
			manager->player.rotation_angle -= (2. * M_PI);
		if (manager->player.rotation_angle < 0)
			manager->player.rotation_angle += (2. * M_PI);
		//printf("%lf\n", manager->player.rotation_speed);
	}
	if (keycode == 124)
	{
		manager->player.turn_direction = 1;
		if (manager->player.rotation_angle > 2. * M_PI)
			manager->player.rotation_angle -= (2. * M_PI);
		if (manager->player.rotation_angle < 0)
			manager->player.rotation_angle += (2. * M_PI);
		manager->player.rotation_angle += manager->player.rotation_speed * manager->player.turn_direction;
		//printf("%lf\n", manager->player.rotation_speed);
	}
	if (keycode == 125)
	{
		manager->player.walk_direction = -1;
		double fx = manager->player.x +(double)manager->player.walk_direction * (double)manager->player.walk_speed * cos(manager->player.rotation_angle);
		double fy =manager->player.y + (double)manager->player.walk_direction * (double)manager->player.walk_speed * sin(manager->player.rotation_angle);
		if ((int)fx/NORMAL_TILE >= 0 && fx / NORMAL_TILE < manager->map->map_width && fy / NORMAL_TILE>= 0 && fy / NORMAL_TILE < manager->map->map_height && manager->map->map[(int)(fy / (double)NORMAL_TILE)][(int)(fx / (double)NORMAL_TILE)] != '1') // TODO: change depeding on the wall width
		{
			manager->player.x += (double)manager->player.walk_direction * (double)manager->player.walk_speed * cos(manager->player.rotation_angle);
			manager->player.y += (double)manager->player.walk_direction * (double)manager->player.walk_speed * sin(manager->player.rotation_angle);
		}
	}
	if (keycode == 126)
	{
		manager->player.walk_direction = 1;
		double fx = manager->player.x +(double)manager->player.walk_direction * (double)manager->player.walk_speed * cos(manager->player.rotation_angle);
		double fy =manager->player.y + (double)manager->player.walk_direction * (double)manager->player.walk_speed * sin(manager->player.rotation_angle);
		if ((int)fx/NORMAL_TILE >= 0 && fx / NORMAL_TILE < manager->map->map_width && fy / NORMAL_TILE>= 0 && fy / NORMAL_TILE < manager->map->map_height && manager->map->map[(int)(fy / (double)NORMAL_TILE)][(int)(fx / (double)NORMAL_TILE)] != '1')
		{
			manager->player.x += (double)manager->player.walk_direction * (double)manager->player.walk_speed * cos(manager->player.rotation_angle);
			manager->player.y += (double)manager->player.walk_direction * (double)manager->player.walk_speed * sin(manager->player.rotation_angle);
		}
	}
	if (keycode == 53)
		exit(EXIT_SUCCESS);
	redraw(manager);
	return 0;	
}

void	draw_grid(long map_width, long map_height, t_cub_manager * cub_manager)
{
	cub_manager->mlx_manager.mlx_window = mlx_new_window(cub_manager->mlx_manager.mlx, map_width * NORMAL_TILE, map_height * NORMAL_TILE, "cub3D");
	clear_window(cub_manager->mlx_manager.mlx, cub_manager->mlx_manager.mlx_window, 0);
	// draw_line(100, 100,130, 90, mlx, mlx_window, 0x00ffffff);
	long	i;
	long	j;
	double	ii;
	double	jj;
	i = -1;
	// printf("%d\n", TILE_SIZE);
	// t_player player;
	cub_manager->map->map_width = map_width;
	cub_manager->map->map_height = map_height;
	cub_manager->player.rotation_speed = radians(5);
	cub_manager->player.walk_speed = 10;
	while (++i < map_height)
	{
		j = -1;
		while (++j < ft_strlen(cub_manager->map->map[i]))
		{
			ii = MAP_TILE * i;
			jj = MAP_TILE * j;
			if (cub_manager->map->map[i][j] == '1')
				draw_empty_rect(jj, ii, MAP_TILE, MAP_TILE, cub_manager->mlx_manager.mlx, cub_manager->mlx_manager.mlx_window, cub_manager->map->wall_colors);
			if (ft_strchr(PLAYER_CHAR, cub_manager->map->map[i][j]))
			{
				draw_circle(jj , ii , MAP_TILE / 4, cub_manager->mlx_manager.mlx, cub_manager->mlx_manager.mlx_window, 0x00ff0000);
				if (cub_manager->map->map[i][j] == 'S')
				{
					cub_manager->player.rotation_angle = radians(90);
					cub_manager->player.x = jj ;
					cub_manager->player.y = ii + MAP_TILE / 2;
					draw_line(cub_manager->player.x, cub_manager->player.y, cub_manager->player.x + LINE_LENGTH * cos(cub_manager->player.rotation_angle),  cub_manager->player.y + LINE_LENGTH * sin(cub_manager->player.rotation_angle), cub_manager->mlx_manager.mlx, cub_manager->mlx_manager.mlx_window, 0x00ff0000);
				}
				if (cub_manager->map->map[i][j] == 'N')
				{
					cub_manager->player.rotation_angle = radians(270);
					cub_manager->player.x = jj ;
					cub_manager->player.y = ii + MAP_TILE / 2;
					draw_line(cub_manager->player.x, cub_manager->player.y, cub_manager->player.x + LINE_LENGTH * cos(cub_manager->player.rotation_angle),  cub_manager->player.y + LINE_LENGTH * sin(cub_manager->player.rotation_angle), cub_manager->mlx_manager.mlx, cub_manager->mlx_manager.mlx_window, 0x00ff0000);
				}
				if (cub_manager->map->map[i][j] == 'E')
				{
					cub_manager->player.rotation_angle = 0;
					cub_manager->player.x = jj ;
					cub_manager->player.y = ii + MAP_TILE / 2;
					draw_line(cub_manager->player.x, cub_manager->player.y, cub_manager->player.x + LINE_LENGTH * cos(cub_manager->player.rotation_angle),  cub_manager->player.y + LINE_LENGTH * sin(cub_manager->player.rotation_angle), cub_manager->mlx_manager.mlx, cub_manager->mlx_manager.mlx_window, 0x00ff0000);
				}
				if (cub_manager->map->map[i][j] == 'W')
				{
					cub_manager->player.rotation_angle = M_PI;
					cub_manager->player.x = jj ;
					cub_manager->player.y = ii+ MAP_TILE / 2 ;
					draw_line(cub_manager->player.x, cub_manager->player.y, cub_manager->player.x + LINE_LENGTH * cos(cub_manager->player.rotation_angle),  cub_manager->player.y + LINE_LENGTH * sin(cub_manager->player.rotation_angle), cub_manager->mlx_manager.mlx, cub_manager->mlx_manager.mlx_window, 0x00ff0000);
				}
			}
		}
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

void	render(t_map_manager	*map_manager)
{
	long	map_width;
	long	map_height;
	t_cub_manager	cub_manager;
	// void	*mlx_window;

	cub_manager.map = map_manager;
	cub_manager.mlx_manager.mlx = mlx_init();
    cub_manager.map->wall_colors = 0x00ffffff;
	// mlx_window = mlx_new_window(mlx, WIDTH, HEIGHT, "cub3D");
	cub_manager.map->map_width = map_width = get_map_width(cub_manager.map);
	// printf("%ld\n", map_width);
	map_height = get_map_height(cub_manager.map);
	// clear_window(mlx, mlx_window, create_trgb(0, 255, 255, 255));
	draw_grid(map_width, map_height, &cub_manager);
	// printf("%ld\n", map_height);
	mlx_hook(cub_manager.mlx_manager.mlx_window, 2, 1L<<0, controls, &cub_manager);
	mlx_loop(cub_manager.mlx_manager.mlx);
}
