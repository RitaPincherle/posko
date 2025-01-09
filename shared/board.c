#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "board.h"

void initBoard(Board *board, int width, int height) {
    board->width = width;
    board->height = height;
    board->cells = malloc(board->height * sizeof(char *));

    for (int i = 0; i < height; i++) {
        board->cells[i] = malloc(board->width * sizeof(char));

        for (int j = 0; j < width; j++) {
            board->cells[i][j] = ' ';
        }
    }
}

int initBoardFile(Board *board, char *filePath) {
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        printf("Error opening file!\n");
        
        return -1;
    }

    fscanf(file, "%d %d", &board->width, &board->height);

    board->cells = malloc(board->height * sizeof(char *));

    for (int i = 0; i < board->height; i++) {
        board->cells[i] = malloc(board->width * sizeof(char));

        for (int j = 0; j < board->width; j++) {
            char ch;
            fscanf(file, "%c", &ch);

            while (ch == '\n') {
                fscanf(file, "%c", &ch);
            }

            board->cells[i][j] = ch;
        }
    }

    fclose(file);

    return 1;
}

void destroyBoard(Board *board) {
    for (int i = 0; i < board->height; i++) {
        free(board->cells[i]);
    }

    free(board->cells);
}

void displayBoard(Board *board) {
    printf("\e[1;1H\e[2J");
    
    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {
            putchar(board->cells[i][j]);
        }

        printf("\n");
    }

    printf("\n");
}

void placeFruit(Board *board) {
    int limit = board->height * board->width;
    int generated;
    int placed = 0;

    generated = rand() % limit;
    int start = generated;

    do {
        char *c = &board->cells[generated / board->width][generated++ % board->height];

        generated = generated % limit;

        if (*c == ' ') {
            *c = '@';
            break;
        }
    } while (generated != start);
}