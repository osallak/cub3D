#include <mlx.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480

#define ROWS 11
#define COLS 15
#ifndef SIZE
#define SIZE 60
#endif

#define COLOR 0x0
// #define X 0
// #define Y 32
// Function for finding absolute value

#define HIGHT 720
#define WIDTH 680

int abs(int n) { return ((n > 0) ? n : (n * (-1))); }
 
float   distance(int x, int y, int ox, int oy)
{
    return (sqrt((ox - x) * (ox - x) + (oy - y) * (oy - y)));
}

void draw_circle(void *mlx, void *window, int ox,int oy,  int r)
{
    int color;

    for (int i = 0; i < HIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++){
            color = distance(i, j, ox, oy) <= r ? 0xffffff : 0x0;
            mlx_pixel_put(mlx, window, i, j, color);
        }
    }
}

// DDA Function for line generation
// void DDA(int X0, int Y0, int X1, int Y1, void* mlx, void* window)
// {
//     // calculate dx & dy
//     int dx = X1 - X0;
//     int dy = Y1 - Y0;
 
//     // calculate steps required for generating pixels
//     int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
 
//     // calculate increment in x & y for each steps
//     float Xinc = dx / (float)steps;
//     float Yinc = dy / (float)steps;
 
//     // Put pixel for each step
//     float X = X0;
//     float Y = Y0;
//          for (int i = 0; i <= steps; i++) {
//             mlx_pixel_put(mlx, window, round(X), round(Y), 0x87CEFA); // put pixel at (X,Y)
//             X += Xinc; // increment in x at each step
//             Y += Yinc;
//             printf("X: %f     Y: %f\n", X, Y); // increment in y at each step
//         }
// }
 
// Driver program

void draw_square(int X, int Y, int size, void *mlx,void *window)
{
    for (int i = 0;i < size;i += 1)
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

int main()
{
    void *mlx = mlx_init();
    // void *window = mlx_new_window(mlx, COLS * SIZE, ROWS * SIZE, "test");
    void *window = mlx_new_window(mlx, HIGHT, WIDTH, "test");

// int map[mapWidth][mapHeight]=
// {
//   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
//   {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
//   {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
// };

    // DDA(0, 0, 500, 600, mlx, window);
    // int j;
    // for (j = 0; j <= 300;j++)
    //     mlx_pixel_put(mlx, window, 100, j, 0x87CEFA);

    // for (int i = 100; i <= 300; i++)
    //     mlx_pixel_put(mlx, window, 100, j, 0x87CEFA);
    // for (int i = 1; i <= 1920; i++)
    // {
    //     for (int j = 1; j <= 1080; j++)
    //         mlx_pixel_put(mlx, window, i, j, 0x87CEFA);
    // }
    // for (int y = 128, x = 0; y <= 1024; y += 32, x += 32){
    //     draw_square(x, y, mlx, window);
    // }

    //     int map[ROWS][COLS] = {
    //     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
    //     {1, 1, 1, 1, 0, 0, 0, 0, 0, 1,1, 1, 1, 1, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1},
    //     {1, 1, 1, 1, 1, 1, 0, 1, 1, 1,1, 1, 1, 1, 1},
    //     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1}
    // };
    // int ty = 0;
    // int y = 0;
    // for (; ty < ROWS; ty += 1){
    //     int x = 0;
    //     int tx = 0;
    //     for (; tx < COLS ;tx += 1)
    //     {
    //         int color = (map[ty][tx] == 1 ? 0x0 : 0x00FFFF);
    //         fill_square(x, y, SIZE, mlx, window, color);
    //         draw_square(x, y, SIZE, mlx, window);
    //         x += SIZE;
    //     }
    //     y += SIZE;
    //     // printf("%d\n", y);
    // }

    draw_circle(mlx, window, HIGHT / 2, WIDTH / 2, 100);
    // draw_square(200, , mlx, window);
    // fill_square(100, 100, 100, mlx, window);
    mlx_loop(mlx);
    printf("Hello World!\n");
    return 0;
}