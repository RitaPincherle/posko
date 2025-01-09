#ifndef SNAKE_H
#define SNAKE_H

#include "board.h"

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point *body;
    int length;
    int hasEaten;
    char direction;
} Snake;

void initSnake(Snake *snake, int startX, int startY);
void destroySnake(Snake *snake);
void eraseSnake(Snake *snake, Board *board);
void moveSnake(Snake *snake, Board *board);
void placeSnake(Snake *snake, Board *board);
int simulateMove(Snake *snake, Board *board);
int validateDirection(Snake* snake, char direction, int height, int width);

#endif