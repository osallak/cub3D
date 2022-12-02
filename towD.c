#include <mlx.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>


#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480


#define ROWS 11
#define COLS 15

#ifndef SIZE
#define SIZE 60
#endif

#define NORTH 2
#define SOUTH 3
#define EAST 4
#define WEST 5

#define COLOR 0xFF00FF

double   distance(int x, int y, int ox, int oy);
double normalizeAngle(double angle);

//fov angle in radians
#define FOV_ANGLE ( 60.0 *  M_PI / 180.0)


//how many pixels per ray
#define W_STRIP 15.0

#define RAYS_NUMBER (COLS * SIZE /  W_STRIP)



/*

    //start first ray starting from half of the FOV
    rayAngle = rotationAngle - ( FOV_ANGLE / 2)

    basic for loop to travel over all rays and render'em
    for (int i = 0; i < RAYS_NUMBER; i++){
        //rayCast ...

        rayAngle += FOV_ANGLE / RAYS_NUMBER
    }
*/
typedef struct s_player
{
    double	x;
    double	y;
    double	radius;
    int		turnDirection;
    int		walkDirection;
    double	rotationAngle;
    double	moveSpeed;
    double	rotationSpeed;
}   t_player;



typedef struct s_data{
    t_player *player;
    void    *mlx;
    void    *window;
    int     **map;
} t_data;

void cast( t_data *data, double rayAngle);

bool    checkForWalls(double newX, double newY, int **map)
{
    // if (newX < 0 || newY < 0 || newX > (COLS * SIZE) || newY > (ROWS * COLS))
    //     return (true);
    int oY = floor(newY / SIZE);
    int oX = floor(newX / SIZE);


    int eY = floor(newY / SIZE);
    int eX = floor(newX / SIZE);


    if (map[oY][oX] == 1 || map[eY][eX] == 1)
        return (true);
    return (false);
}


inline int max(int a, int b)
{
    return (a > b ? a : b);
}
void initPlayer(t_player *player)
{
    player->turnDirection = 0;// -1 left, 1 right
    player->walkDirection = 0;// -1 back, -1 front
    player->rotationAngle = M_PI_2 ;
    player->moveSpeed = 15.0;
    player->rotationSpeed = 10.0 * (M_PI / 180);
    player->x = -1;
    player->y = -1;
}

double   distance(int x, int y, int ox, int oy)
{
    return (sqrt((ox - x) * (ox - x) + (oy - y) * (oy - y)));
}

double rad2degree(double rad)
{
    return (rad * 180 / M_PI);
}

double degree2rad(double degree)
{
    return (degree * M_PI / 180);
}

// note that the x and y are passed by reference
void rotatePoint(double *x, double *y, double angleDegree, int hypotenuse)
{
    double rad = degree2rad(angleDegree);
    *x += cos(rad) * hypotenuse;
    *y += sin(rad) * hypotenuse;
}


void draw_circle(void *mlx, void *window, double ox,double oy,  int r)
{

	double dis;
    for (int i = 0; i < ROWS * SIZE; i++)
    {
        for (int j = 0; j < COLS * SIZE; j++){

			dis = fabs(distance(i, j, ox, oy));
            //if the point position is less or equal to radius draw pixel
            if ( dis <= r){
				// printf("distance: %f\n", dis);
                mlx_pixel_put(mlx, window, i, j, 0x0);
			}
        }
    }
}

void draw_square(int X, int Y, int size, void *mlx,void *window)
{
    for (int i = 0;i <= size;i += 1)
    {
        mlx_pixel_put(mlx, window, X, Y + i, COLOR);
        mlx_pixel_put(mlx, window, X + size, Y + i, COLOR);
        mlx_pixel_put(mlx, window, X + i, Y, COLOR);
        mlx_pixel_put(mlx, window, X + i, Y + size, COLOR);
    }
}

void fill_square(int X, int Y,int length,  void *mlx, void *window, int color)
{
    int x = X;
    for (int i = 0; i < length; i++, Y++){
        for (int j = 0; j < length; j++, X++){
            mlx_pixel_put(mlx, window, X, Y, color);
        }
        X = x;
    }
}

void render(t_data *data, void *mlx, void* window, int **map, t_player *player);

void draw_flesh(void *mlx, void *window, int x, int y, int size, int flag)
{
    for (int i = 0; i < size; i++)
    {
        int tx, ty;
        if (flag == NORTH){
        //up
            tx = x;
            ty = y - i;
        } else if (flag == SOUTH){
        //down
            tx = x;
            ty = y + i;
        } else if (flag == EAST){
            tx = x - i;
            ty = y;
        } else{
        //left
            tx = x + i;
            ty = y;
        }
        mlx_pixel_put(mlx, window, tx, ty, 0XFFFF);
    }
}

int key_hook(int key, t_data* data)
{
    double newX;
    double newY;
    if (key == 53){
        exit(0);
    } else if (key == 123){
        data->player->turnDirection = -1;
    } else if (key == 124){
        data->player->turnDirection = 1;
    } else if (key == 125){
        data->player->walkDirection = -1;
    } else if (key == 126){
        data->player->walkDirection = 1;
    }

    data->player->rotationAngle += data->player->turnDirection * data->player->rotationSpeed; // it will add nothing if the key isn't pressed
    data->player->rotationAngle = normalizeAngle(data->player->rotationAngle); // reset angle if it out of bounds (2PI)
    newX = data->player->x + cos(data->player->rotationAngle) * (data->player->walkDirection * data->player->moveSpeed);
    newY = data->player->y + sin(data->player->rotationAngle) * (data->player->walkDirection * data->player->moveSpeed);
    // printf("x:  %f\ty:  %f\n", data->player->x, data->player->y);

    if (!checkForWalls(newX, newY, data->map)){
        data->player->x = newX;
        data->player->y = newY;
        // printf("newX: %f   newY: %f    mapElement:   %d\n", newX, newY, data->map[(int)(newY / SIZE)][(int) (newX / SIZE)]);
    }
    render(data, data->mlx, data->window, data->map, data->player);
    return 0;
}


//ox stands for origin x
//same goes for oy
void drawLineDda(void *mlx, void *window, double  ox, double oy, double x, double y, int color, int **map)
{
    double dx = x - ox;
    double dy = y - oy;

    double steps = max(fabs(dx), fabs(dy));
    double xinc = (double) dx / steps;
    double yinc = (double) dy /  steps;

    for (int i = 0; i < steps; i++){
        if (checkForWalls(ox, oy, map) == true)
            break ;
        mlx_pixel_put(mlx, window, round(ox) , round(oy), color);
        ox += xinc;
        oy += yinc;
    }
}

void    resetPlayer(t_player *player)
{
    player->turnDirection = 0;
    player->walkDirection = 0;
}

void castRays(t_data *data, void *mlx, void* window, int **map, t_player *player)
{
    double rayAngle = player->rotationAngle - (FOV_ANGLE / 2);
    for (int i = 0; i < RAYS_NUMBER; i++){
        // drawLineDda(mlx, window, player->x, player->y, player->x + cos(rayAngle) * 100, player->y + sin(rayAngle) * 100, 0x9aff0000, map);
        rayAngle = normalizeAngle(rayAngle);
        cast(data, rayAngle);
        rayAngle += (double) FOV_ANGLE / (double) RAYS_NUMBER ;

        // printf("%f    %f\n", rayAngle, FOV_ANGLE / RAYS_NUMBER);
    }
}

void render(t_data *data, void *mlx, void* window, int **map, t_player *player)
{
  int ty = 0;
    int y = 0;


    //set default position to the north

    for (; ty < ROWS; ty += 1){
        int x = 0;
        int tx = 0;
        for (; tx < COLS ;tx += 1)
        {
            int color = (map[ty][tx] == 1 ? 0x0 : 0xffffff);
            fill_square(x, y, SIZE, mlx, window, color);
            draw_square(x, y, SIZE, mlx, window);
            if (map[ty][tx] * 2 > 2 && player->x == -1 && player->y == -1){ //wall = 1// 1 * 2=2
                player->x = x;
                player->y = y;
            }
            x += SIZE;
        }
        y += SIZE;
    }
    // printf("X: %f,     Y: %f\n", player->x, player->y);
    draw_square(player->x, player->y, 10, mlx, window);
    castRays(data, mlx, window, map, player);
    drawLineDda(mlx, window, player->x, player->y, player->x + cos(player->rotationAngle) * 40 , player->y + sin(player->rotationAngle) * 40, 0x0, map);
    resetPlayer(player);
}


double normalizeAngle(double angle)
{
    if (angle < 0)
        angle += (M_PI * 2.0);
    else if (angle > (M_PI * 2.0))
        angle -= (M_PI * 2.0);
    return (angle);
}

//returns true if it's a wall
bool    itIsWall(double x, double y, int **map)
{
    // printf("%d    %d\n", (int)(x / SIZE), (int)(y / SIZE));
    return (map[(int)(y / SIZE)][(int)(x / SIZE)] == 1);
}

//cast Function

void cast( t_data *data, double rayAngle)
{
    puts("before");//debug
    printf("rayAngle: %f\n", rayAngle);
    printf("px: %f\tpy: %f\n", data->player->x, data->player->y);
    //from here to end this should be initialized before entering this function
    double wallHitX = .0;
    double wallHitY = .0;
    double __distance = 0.0;
    bool wasHitVertical = false;

    bool isFacingDown = rayAngle > 0 && rayAngle < M_PI;
    bool isFacingUp = !isFacingDown;

    bool isFacingRight = rayAngle < M_PI_2 || rayAngle > (M_PI + M_PI_2);
    bool isFacingLeft = !isFacingRight;

    // the end

    double xintercept = .0;
    double yintercept = .0;
    double xstep = .0;
    double ystep = .0;

    bool foundHorizHit = false;

    double horizWallHitX = .0;
    double horizWallHitY = .0;

    yintercept = floor(data->player->y / SIZE) * SIZE;
    printf("yintercept: %f\n", yintercept);

    if (isFacingDown)
        yintercept += SIZE;
    
    xintercept = data->player->x + (yintercept - data->player->y) / tan(rayAngle);//rayAngle should be in radians

    ystep = SIZE;
    if (isFacingUp)
        ystep = -ystep;
    
    xstep = SIZE / tan(rayAngle);

    if ((isFacingLeft && xstep > 0) || (isFacingRight && xstep < 0))
        xstep = -xstep;

    double nextHorizTouchX = xintercept;
    double nextHorizTouchY = yintercept;

    if (isFacingUp)
        nextHorizTouchY--;
    
    // printf(isFacingUp ? "true\n" : "false\n");
    
    //increment xstep and ystep untill we found a wall
    while ( nextHorizTouchX > 0 && nextHorizTouchY > 0 && nextHorizTouchX < (SIZE * COLS) && nextHorizTouchY < (SIZE * ROWS) )
    {
        printf("be-itiswall: %f    %f\n", nextHorizTouchX, nextHorizTouchY);
        if (itIsWall(nextHorizTouchX, nextHorizTouchY, data->map)){
            foundHorizHit = true;
            horizWallHitX = nextHorizTouchX;
            horizWallHitY = nextHorizTouchY;
            break ;
        }
        nextHorizTouchX += xstep;
        nextHorizTouchY += ystep;
    }
    puts("after");//debug

    // VERTICAL RAY-GRID INTERSECTION CODE

    bool foundVertWallHit = false;

    double vertWallHitX = .0;
    double vertWallHitY = .0;

    xintercept = floor(data->player->x / SIZE) * SIZE;
    
    if (isFacingRight)
        xintercept += SIZE;

    yintercept = data->player->y + (xintercept - data->player->x) * tan(rayAngle);

    xstep = SIZE;

    if (isFacingLeft)
        xstep = -xstep;
    
    ystep = SIZE * tan(rayAngle);

    if ((isFacingUp && ystep > 0) || (isFacingDown && ystep < 0))
        ystep = -ystep;
    
    double nextVertTouchX = xintercept;
    double nextVertTouchY = yintercept;

    if (isFacingLeft)
        nextVertTouchX--;
    
    while (nextVertTouchX > 0 && nextVertTouchY > 0 && nextVertTouchX < (SIZE * COLS) && nextVertTouchY < (SIZE * ROWS))
    {
        if (itIsWall(nextVertTouchX, nextVertTouchY, data->map)){
            foundVertWallHit = true;
            vertWallHitX = nextVertTouchX;
            vertWallHitY = nextVertTouchY;
            break;
        }
        nextVertTouchX += xstep;
        nextVertTouchY += ystep;

    }
    
    // Calculate both horizontal and vertical distances and choose the smallest value

    double  horizHitDistance = INT_MAX;
    double vertHitDistance = INT_MAX;

    if (foundHorizHit)
        horizHitDistance = distance(data->player->x, data->player->y, horizWallHitX, horizWallHitY);
    if (foundVertWallHit)
        vertHitDistance = distance(data->player->x, data->player->y, vertWallHitX, vertWallHitY);

    if ( horizHitDistance < vertHitDistance){
        wallHitX = horizWallHitX;
        wallHitY = horizWallHitY;
    } else {
        wallHitX = vertWallHitX;
        wallHitY = vertWallHitY;
    }

    drawLineDda(data->mlx, data->window, data->player->x, data->player->y, wallHitX, wallHitY, 0xff0000, data->map);
}


int main()
{
    t_player player;
    initPlayer(&player);

    int map[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 1, 1, 1, 0, 1, 0, 0, 0, 1,1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 0, 0, 1, 1, 1, 1, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 0, 2, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1}
    };

    t_data *data;

    data = malloc(sizeof(t_data));
    data->mlx = mlx_init();
    data->window = mlx_new_window(data->mlx, COLS * SIZE + 1, ROWS * SIZE + 1, "2d Raycaster");


    data->player = &player;
    data->map = (int **) malloc(ROWS * sizeof(int *));

    for (int j = 0; j < ROWS; j++){
        data->map[j] = malloc(COLS * sizeof(int));
        for (int i = 0; i < COLS; i++){
            data->map[j][i] = map[j][i];
        }
    }
    render(data, data->mlx, data->window,data->map, &player);
    // mlx_key_hook(data->window, key_hook, data);
    mlx_hook(data->window, 2, 0, key_hook, data);

    // drawLineDda(data->mlx, data->window, 400, 400, 300, 350, 0xffffff);
    mlx_loop(data->mlx);
    return 0;
}