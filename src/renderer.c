/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:26:42 by yakhoudr          #+#    #+#             */
/*   Updated: 2022/12/10 10:36:32 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"
#define WIDTH 1080
#define HEIGHT 720

inline double	radians(double angle)
{
	return (angle * (M_PI / 180));
}

void draw_line(double  ox, double oy, double x, double y, void *mlx, void *window, int color)
{
    double dx = x - ox;
    double dy = y - oy;

    double steps = fmax(fabs(dx), fabs(dy));
    double xinc = (double) dx / steps;
    double yinc = (double) dy /  steps;

    for (int i = 0; i < steps; i++){
        // if (checkForWalls(ox, oy, map) == true)
        //     break ;
        mlx_pixel_put(mlx, window, round(ox) , round(oy), color);
        ox += xinc;
        oy += yinc;
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

void	draw_empty_rect(int x, int y, int size, void *mlx, void *mlx_window, int color)
{
	long	i;

	i = -1;
	while (++i < size)
	{
		mlx_pixel_put(mlx, mlx_window, x + i, y, color);
		mlx_pixel_put(mlx, mlx_window, x + i, y + size, color);
		mlx_pixel_put(mlx, mlx_window, x, y + i, color);
		mlx_pixel_put(mlx, mlx_window, x + size, y + i, color);
	}
}

void	draw_full_rect(int x, int y, int size, void *mlx, void *mlx_window, int color)
{
	long	i;
	long	j;

	i = -1;
	while (++i < size)
	{
		j = -1;
		while (++j < size)
			mlx_pixel_put(mlx, mlx_window, x + j, y + i, color);
			// mlx_pixel_put(mlx, mlx_window, x + i, y + size, color);
			// mlx_pixel_put(mlx, mlx_window, x, y + i, color);
			// mlx_pixel_put(mlx, mlx_window, x + size, y + i, color);
	}
}

// void	draw_line(double x1, double y1, double x2, double y2, void *mlx, void *mlx_window, int color)
// {
// 	double	dx;
// 	double	dy;
// 	double	p;
// 	double	x;
// 	double	y;
// 	double	end;

// 	dx = fabs(x1 - x2);
// 	dy = fabs(y1 - y2);
// 	p = 2 * dy - dx;
// 	if (x1 > x2)	
// 	{
// 		x = x2;
// 		y = y2;
// 		end = x1;
// 	}
// 	else
// 	{
// 		x = x1;
// 		y = y1;
// 		end = x2;
// 	}
// 	mlx_pixel_put(mlx, mlx_window, x, y, color);
// 	while (x < end)
// 	{
// 		++x;
// 		if (p < 0)
// 			p = p + 2 * dy;
// 		else
// 		{
// 			++y;
// 			p = p + 2 * (dy - dx);
// 		}
// 		mlx_pixel_put(mlx, mlx_window, x, y, color);
// 	}
// }

#define NORMAL_TILE 30
#define MAP_TILE (NORMAL_TILE)
#define LINE_LENGTH 30




void	draw_circle(double ox, double oy, double radius, void * mlx, void * mlx_window, int color)
{
	for (double i = 0;i < 360; i+=0.1)
	{
		mlx_pixel_put(mlx, mlx_window, ox + radius * cos(radians(i)), oy + radius * sin(radians(i)), color);
	}
}


void	redraw(t_cub_manager *manager)
{
	clear_window(manager->mlx_manager.mlx, manager->mlx_manager.mlx_window, 0);
	long i = -1;
	while (++i < manager->map->map_height)
	{
		long j = -1;
		int ii = i * MAP_TILE;
		while (++j < ft_strlen(manager->map->map[i]))
		{
			double jj = j * MAP_TILE;
			if (manager->map->map[i][j] == '1')
				draw_full_rect(jj, ii, MAP_TILE, manager->mlx_manager.mlx, manager->mlx_manager.mlx_window, manager->map->wall_colors);
		}
	}
	draw_circle(manager->player.x , manager->player.y, MAP_TILE / 4, manager->mlx_manager.mlx, manager->mlx_manager.mlx_window, 0x00ff0000);
	draw_line(manager->player.x , manager->player.y, manager->player.x + LINE_LENGTH * cos(manager->player.rotation_angle)\
	,manager->player.y + LINE_LENGTH * sin(manager->player.rotation_angle), manager->mlx_manager.mlx, manager->mlx_manager.mlx_window, 0x00ff0000);
}
int controls(int keycode, t_cub_manager *manager)
{
	if (keycode == 123)
	{
		manager->player.turn_direction = -1;
		manager->player.rotation_angle += manager->player.rotation_speed * manager->player.turn_direction;
		//printf("%lf\n", manager->player.rotation_speed);
	}
	if (keycode == 124)
	{
		manager->player.turn_direction = 1;
		manager->player.rotation_angle += manager->player.rotation_speed * manager->player.turn_direction;
		//printf("%lf\n", manager->player.rotation_speed);
	}
	if (keycode == 125)
	{
		manager->player.walk_direction = -1;
		double fx = manager->player.x +(double)manager->player.walk_direction * (double)manager->player.walk_speed * cos(manager->player.rotation_angle);
		double fy =manager->player.y + (double)manager->player.walk_direction * (double)manager->player.walk_speed * sin(manager->player.rotation_angle);
		if ((int)fx/MAP_TILE >= 0 && fx / MAP_TILE < manager->map->map_width && fy / MAP_TILE>= 0 && fy / MAP_TILE < manager->map->map_height && manager->map->map[(int)floor(fy / (double)MAP_TILE)][(int)floor(fx / (double)MAP_TILE)] != '1')
		{
			printf("here\n");
			printf("b-->%lf\n", manager->player.x);
			manager->player.x += (double)manager->player.walk_direction * (double)manager->player.walk_speed * cos(manager->player.rotation_angle);
			printf("a-->%lf\n", manager->player.x);
			manager->player.y += (double)manager->player.walk_direction * (double)manager->player.walk_speed * sin(manager->player.rotation_angle);
		}
		printf("%d\n", (int)floor(fx / (double)MAP_TILE));
		printf("%d\n\n\n\n\n\n", (int)floor(fy / (double)MAP_TILE));
		printf("-->%c\n", manager->map->map[(int)floor(fx / (double)MAP_TILE)][(int)floor(fy / (double)MAP_TILE)]);
	}
	if (keycode == 126)
	{
		manager->player.walk_direction = 1;
		double fx = manager->player.x +(double)manager->player.walk_direction * (double)manager->player.walk_speed * cos(manager->player.rotation_angle);
		double fy =manager->player.y + (double)manager->player.walk_direction * (double)manager->player.walk_speed * sin(manager->player.rotation_angle);
		if ((int)fx/MAP_TILE >= 0 && fx / MAP_TILE < manager->map->map_width && fy / MAP_TILE>= 0 && fy / MAP_TILE < manager->map->map_height && manager->map->map[(int)floor(fy / (double)MAP_TILE)][(int)floor(fx / (double)MAP_TILE)] != '1')
		{
			printf("here\n");
			printf("b-->%lf\n", manager->player.x);
			manager->player.x += (double)manager->player.walk_direction * (double)manager->player.walk_speed * cos(manager->player.rotation_angle);
			printf("a-->%lf\n", manager->player.x);
			manager->player.y += (double)manager->player.walk_direction * (double)manager->player.walk_speed * sin(manager->player.rotation_angle);
		}
		// manager->player.x += (double)manager->player.walk_direction * (double)manager->player.walk_speed * cos(manager->player.rotation_angle);
		// manager->player.y += (double)manager->player.walk_direction * (double)manager->player.walk_speed * sin(manager->player.rotation_angle);
		printf("%d\n", (int)floor(fx / (double)MAP_TILE));
		printf("%d\n\n\n\n\n\n", (int)floor(fy / (double)MAP_TILE));
		printf("-->%c\n", manager->map->map[(int)floor(fx / (double)MAP_TILE)][(int)floor(fy / (double)MAP_TILE)]);
	}
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
	cub_manager->player.rotation_speed = radians(10);
	cub_manager->player.walk_speed = 4;
	while (++i < map_height)
	{
		j = -1;
		while (++j < ft_strlen(cub_manager->map->map[i]))
		{
			ii = MAP_TILE * i;
			jj = MAP_TILE * j;
			if (cub_manager->map->map[i][j] == '1')
				draw_full_rect(jj, ii, MAP_TILE, cub_manager->mlx_manager.mlx, cub_manager->mlx_manager.mlx_window, cub_manager->map->wall_colors);
			if (ft_strchr(PLAYER_CHAR, cub_manager->map->map[i][j]))
			{
				draw_circle(jj + MAP_TILE / 2, ii - MAP_TILE / 2, MAP_TILE / 4, cub_manager->mlx_manager.mlx, cub_manager->mlx_manager.mlx_window, 0x00ff0000);
				if (cub_manager->map->map[i][j] == 'S')
				{
					cub_manager->player.rotation_angle = radians(90);
					cub_manager->player.x = jj + MAP_TILE / 2;
					cub_manager->player.y = ii - MAP_TILE / 2;
					draw_line(cub_manager->player.x, cub_manager->player.y, cub_manager->player.x + LINE_LENGTH * cos(cub_manager->player.rotation_angle),  cub_manager->player.y + LINE_LENGTH * sin(cub_manager->player.rotation_angle), cub_manager->mlx_manager.mlx, cub_manager->mlx_manager.mlx_window, 0x00ff0000);
				}
				if (cub_manager->map->map[i][j] == 'N')
				{
					cub_manager->player.rotation_angle = radians(270);
					cub_manager->player.x = jj + MAP_TILE / 2;
					cub_manager->player.y = ii - MAP_TILE / 2;
					draw_line(cub_manager->player.x, cub_manager->player.y, cub_manager->player.x + LINE_LENGTH * cos(cub_manager->player.rotation_angle),  cub_manager->player.y + LINE_LENGTH * sin(cub_manager->player.rotation_angle), cub_manager->mlx_manager.mlx, cub_manager->mlx_manager.mlx_window, 0x00ff0000);
				}
				if (cub_manager->map->map[i][j] == 'E')
				{
					cub_manager->player.rotation_angle = 0;
					cub_manager->player.x = jj + MAP_TILE / 2;
					cub_manager->player.y = ii - MAP_TILE / 2;
					draw_line(cub_manager->player.x, cub_manager->player.y, cub_manager->player.x + LINE_LENGTH * cos(cub_manager->player.rotation_angle),  cub_manager->player.y + LINE_LENGTH * sin(cub_manager->player.rotation_angle), cub_manager->mlx_manager.mlx, cub_manager->mlx_manager.mlx_window, 0x00ff0000);
				}
				if (cub_manager->map->map[i][j] == 'W')
				{
					cub_manager->player.rotation_angle = M_PI;
					cub_manager->player.x = jj + MAP_TILE / 2;
					cub_manager->player.y = ii - MAP_TILE / 2;
					draw_line(cub_manager->player.x, cub_manager->player.y, cub_manager->player.x + LINE_LENGTH * cos(cub_manager->player.rotation_angle),  cub_manager->player.y + LINE_LENGTH * sin(cub_manager->player.rotation_angle), cub_manager->mlx_manager.mlx, cub_manager->mlx_manager.mlx_window, 0x00ff0000);
				}
			}
		}
	}
	// draw_line(200, 300, 100, 100, mlx, mlx_window, 0x00ffffff);

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
	map_width = get_map_width(cub_manager.map);
	// printf("%ld\n", map_width);
	map_height = get_map_height(cub_manager.map);
	// clear_window(mlx, mlx_window, create_trgb(0, 255, 255, 255));
	draw_grid(map_width, map_height, &cub_manager);
	// printf("%ld\n", map_height);
	mlx_hook(cub_manager.mlx_manager.mlx_window, 2, 1L<<0, controls, &cub_manager);
	mlx_loop(cub_manager.mlx_manager.mlx);
}