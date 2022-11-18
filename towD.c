#include <mlx.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

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

typedef struct s_player
{
    int x, y;
    float radius;
    int turnDirection;
    int walkDirection;
    double rotationAngle;
    float moveSpeed;
    float rotationSpeed;
}   t_player;

inline int max(int a, int b)
{
    return (a > b ? a : b);
}
void initPlayer(t_player *player)
{
    player->turnDirection = 0;
    player->walkDirection = 0;
    player->rotationAngle = M_PI / 2;
    player->moveSpeed = 3.0;
    player->rotationSpeed = 3.0 * (M_PI / 180);
}

float   distance(int x, int y, int ox, int oy)
{
    return (sqrt((ox - x) * (ox - x) + (oy - y) * (oy - y)));
}

//note that the x and y are passed by reference
void rotatePoint(int *x, int *y, float angleDegree, int hypotenuse)
{
    *x += cos(angleDegree) * hypotenuse;
    *y += sin(angleDegree) * hypotenuse;
}

float rad2degree(float rad)
{
    return (rad * 180 / M_PI);
}

float degree2rad(float degree)
{
    return (degree * M_PI / 180);
}

void draw_circle(void *mlx, void *window, int ox,int oy,  int r)
{

    for (int i = 0; i < ROWS * SIZE; i++)
    {
        for (int j = 0; j < COLS * SIZE; j++){

            //if the point position is less or equal to radius draw pixel
            if (distance(i, j, ox, oy) <= r)
                mlx_pixel_put(mlx, window, i, j, 0x0000ff);
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

int key_hook(int key, t_player* player)
{
    if (key == 123){
        player->turnDirection = -1;
    } else if (key == 124){
        player->turnDirection = 1;
    } else if (key == 125){
        player->walkDirection = -1;
    } else if (key == 126){
        player->walkDirection = 1;
    }
    return 0;
}


//ox stands for origin x
//same goes for oy
void drawLineDda(void *mlx, void *window, int  ox, int oy, int x, int y)
{
    int dx = x - ox;
    int dy = y - oy;

    int steps = max(abs(dx), abs(dy));
    float xinc = dx / steps;
    float yinc = dy / steps;

    for (int i = 0; i < steps; i++){
        mlx_pixel_put(mlx, window, ox, oy, 0x0000ff);
        ox += xinc;
        oy += yinc;
    }
}

void render(void *mlx, void* window, int map[ROWS][COLS], t_player *player)
{
  int ty = 0;
    int y = 0;

    bool __player = false;

    //set default position to the north

    for (; ty < ROWS; ty += 1){
        int x = 0;
        int tx = 0;
        for (; tx < COLS ;tx += 1)
        {
            int color = (map[ty][tx] == 1 ? 0x0 : map[ty][tx] == 0 ?  0x00FFFF  : 0xFF0000);
            fill_square(x, y, SIZE, mlx, window, color);
            draw_square(x, y, SIZE, mlx, window);
            if (map[ty][tx] != 0 && map[ty][tx] != 1){
                player->x = x;
                player->y = y;
                printf("-----> %d\n", map[ty][tx]);
                // draw_flesh(mlx, window, x + (SIZE / 2), y + (SIZE / 2), SIZE / 2, map[ty][tx]);
                draw_circle(mlx, window, tx * SIZE + SIZE / 2, ty * SIZE + SIZE / 2, 10);
                int directionX = x;
                int directionY = y;
                rotatePoint(&directionX, &directionX, 15, 20);
                printf("dX: %d     dY: %d\n", directionX, directionY);
                drawLineDda(mlx, window, x + (SIZE / 2) , y + (SIZE / 2), directionX,directionY);
            }
            // if (__player == true)
            //     draw_circle(mlx, window, COLS + SIZE / 2, ROWS + SIZE / 2, 30);
            x += SIZE;
        }
        y += SIZE;
    }
}

int main()
{
    srand((unsigned int)&main);
    t_player player;
    initPlayer(&player);

    int playerDir = (rand() % 4) + 2;
    int map[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 1,1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 0, 0, 1, 1, 1, 1, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 0, playerDir, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1}
    };

    void *mlx = mlx_init();
    void *window = mlx_new_window(mlx, COLS * SIZE + 1, ROWS * SIZE + 1, "2d Raycaster");

  
    drawLineDda(mlx, window, 0 ,0, 100, 50);
    // render(mlx, window, map, &player);
    mlx_key_hook(window, key_hook, &player);
    mlx_loop(mlx);
    return 0;
}