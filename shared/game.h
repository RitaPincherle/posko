#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "snake.h"

typedef struct {
    Snake snake;
    Board board;
} Game;

void initBoard(Snake *snake, int startX, int startY);
int checkSelfCollision(Snake *snake, Board *board);
void eraseSnake(Snake *snake, Board *board);
void moveSnake(Snake *snake, Board *board);
int checkCollision(Snake *snake, Board *board);
void placeSnake(Snake *snake, Board *board);
int validateDirection(Snake* snake, char direction, int height, int width);

#endif