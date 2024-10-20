#ifndef SNAKE_H_
#define SNAKE_H_
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string.h>
#define SNAKE_SIZE 16
#define WINDOW_WIDTH 864
#define WINDOW_HEIGHT 560

typedef struct {
    int x, y;
} SnakeSegment;

SnakeSegment snake[50];
int snakeLength = 1;
int lines;

#endif 