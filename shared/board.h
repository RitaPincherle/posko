#ifndef BOARD_H
#define BOARD_H

typedef struct {
    int width, height;
    char **cells;
} Board;

void initBoard(Board *board, int width, int height);
int initBoardFile(Board *board, char *filePath);
void destroyBoard(Board *board);
void displayBoard(Board *board);
void placeFruit(Board *board);

#endif