#ifndef SNAKE_H_
#define SNAKE_H_
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

typedef struct {
    int x, y;
} SnakeSegment;

SnakeSegment snake[100]; // Stores up to 100 segments
int snakeLength = 1;  // Initial length of the snake

void update_snake_position() {
    // Move the body of the snake
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];  // Shift each segment to the position of the previous one
    }
    // Move the head
    if (currentDirection == 0) snake[0].y -= SNAKE_SIZE; // Up
    else if (currentDirection == 1) snake[0].y += SNAKE_SIZE; // Down
    else if (currentDirection == 2) snake[0].x -= SNAKE_SIZE; // Left
    else if (currentDirection == 3) snake[0].x += SNAKE_SIZE; // Right
}

#endif 