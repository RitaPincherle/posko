#include <stdio.h>
#include <stdlib.h>
#include "snake.h"
#include "board.h"

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

void initSnake(Snake *snake, int startX, int startY) {
    snake->length = 1;
    snake->body = malloc(sizeof(Point));
    snake->body[0].x = startX;
    snake->body[0].y = startY;
    snake->direction = RIGHT;
    snake->hasEaten = 0;
}

void destroySnake(Snake *snake) {
    free(snake->body);
}

void eraseSnake(Snake *snake, Board *board) {
    for (int i = 0; i < snake->length; ++i) {
        board->cells[snake->body[i].y][snake->body[i].x] = ' ';
    }
}

void moveSnake(Snake *snake, Board *board) {
    int deltaX = snake->body[0].x;
    int deltaY = snake->body[0].y;

    switch (snake->direction) {
        case UP: deltaY--; break;
        case DOWN: deltaY++; break;
        case LEFT: deltaX--; break;
        case RIGHT: deltaX++; break;
    }

    for (int i = snake->length - 1; i > 0; --i) {
        snake->body[i].x = snake->body[i - 1].x;
        snake->body[i].y = snake->body[i - 1].y;
    }

    snake->body[0].x = (deltaX + board->width) % board->width;
    snake->body[0].y = (deltaY + board->height) % board->height;
}

int simulateMove(Snake *snake, Board *board) {
    int deltaX = snake->body[0].x;
    int deltaY = snake->body[0].y;

    switch (snake->direction) {
        case UP: deltaY--; break;
        case DOWN: deltaY++; break;
        case LEFT: deltaX--; break;
        case RIGHT: deltaX++; break;
    }

    deltaX = (deltaX + board->width) % board->width;
    deltaY = (deltaY + board->height) % board->height;

    if (snake->hasEaten == 0) {
        if (deltaX == snake->body[snake->length - 1].x && deltaY == snake->body[snake->length - 1].y) {
            return 0;
        }
    } else {
        snake->hasEaten = 0;
        snake->length++;
        snake->body = realloc(snake->body, snake->length * sizeof(Point));

        snake->body[snake->length - 1].x = snake->body[snake->length - 2].x;
        snake->body[snake->length - 1].y = snake->body[snake->length - 2].y;
    }

    switch (board->cells[deltaY][deltaX]) {
        case 'o':
        case '#':
            return -1;
        case '@':
            snake->hasEaten = 1;
            return 1;
        default:
            return 0;
    }
}

void placeSnake(Snake *snake, Board *board) {
    board->cells[snake->body[0].y][snake->body[0].x] = 'O';

    for (int i = 1; i < snake->length; ++i) {
        board->cells[snake->body[i].y][snake->body[i].x] = 'o';
    }
}

int validateDirection(Snake* snake, char direction, int height, int width) {
    int deltaX = snake->body[0].x;
    int deltaY = snake->body[0].y;

    switch (direction) {
        case UP: deltaY--; break;
        case DOWN: deltaY++; break;
        case LEFT: deltaX--; break;
        case RIGHT: deltaX++; break;
        default: return -1;
    }

    deltaX = (deltaX + width) % width;
    deltaY = (deltaY + height) % height;

    if (snake->length != 1 && (deltaX == snake->body[1].x && deltaY == snake->body[1].y)) {
        return -1;
    }

    return 1;
}