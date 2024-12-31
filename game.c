#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'


//          BOARD

typedef struct {
    int width, height;
    char **cells;
} Board;

void initBoard(Board *board, int width, int height) {
    board->width = width;
    board->height = height;
    board->cells = malloc(board->height * sizeof(char *));

    for (int i = 0; i < height; i++) {
        board->cells[i] = malloc(board->width * sizeof(char));
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == board->height - 1 || j == 0 || j == board->width - 1) {
                board->cells[i][j] = '#';
            } else {
                board->cells[i][j] = ' ';
            }

            
        }
    }
}

int initBoardFile(Board *board, char* filePath) {
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

void displayBoard(Board *board) {
    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {
            putchar(board->cells[i][j]);
        }

        printf("\n");
    }

    printf("\n");
}

//          POINT

typedef struct {
    int x, y;
} Point;

//          SNAKE

typedef struct {
    Point *body;
    int length;
    int hasEaten;
    char direction;
} Snake;

void initSnake(Snake *snake, int startX, int startY) {
    snake->length = 1;
    snake->body = malloc(sizeof(Point));
    snake->body[0].x = startX;
    snake->body[0].y = startY;
    snake->direction = RIGHT;
}

//          METHODS

int checkSelfCollision(Snake *snake, Board* board) {
    int deltaX = snake->body[0].x;
    int deltaY = snake->body[0].y;

    switch (snake->direction) {
        case UP: deltaY--; break;
        case DOWN: deltaY++; break;
        case LEFT: deltaX--; break;
        case RIGHT: deltaX++; break;
    }

    Point last = snake->body[snake->length - 1];

    if (board->cells[deltaY][deltaX] == 'o' && !(last.x == deltaX && last.y == deltaY)) {
        return -1;
    }

    return 1;
}

void eraseSnake(Snake *snake, Board* board) {
    for (int i = 0; i < snake->length; ++i) {
        board->cells[snake->body[i].y][snake->body[i].x] = ' ';
    }
}

void moveSnake(Snake *snake, Board *board) {
    if (snake->hasEaten == 1) {
        snake->hasEaten = 0;
        snake->body = realloc(snake->body, (snake->length + 1) * sizeof(Point));
        snake->length++;
    }

    int deltaX = 0;
    int deltaY = 0;

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

    snake->body[0].x = (snake->body[0].x + deltaX + board->width) % board->width;
    snake->body[0].y = (snake->body[0].y + deltaY + board->height) % board->height;
}

int checkCollision(Snake *snake, Board *board) {
    switch (board->cells[snake->body[0].y][snake->body[0].x]) {
        case '#':
            return -1;
        case '@':
            snake->hasEaten = 1;
            placeFruit(board);
        default:
            return 1;
    }
}

void placeSnake(Snake *snake, Board *board) {
    board->cells[snake->body[0].y][snake->body[0].x] = 'O';

    for (int i = 1; i < snake->length; ++i) {
        board->cells[snake->body[i].y][snake->body[i].x] = 'o';
    }
}

void printBoard(Board *board) {
    for (int i = 0; i < board->height; ++i) {
        for (int j = 0; board->width; ++j) {
            putchar(board->cells[i][j]);
        }

        putchar('\n');
    }

    putchar('\n');
}

void placeFruit(Board *board) {
    int limit = board->height * board->width;

    int generated;

    while (1 == 1) {
        generated = rand() % limit;
        char* c = &board->cells[generated / board->width][generated % board->height];

        if (*c == ' ') {
            *c = '@';
            break;
        };
    }
}

//          TIMER

//          

int main() {
    srand(time(NULL));

    while (1 == 1) {
        int i = 0;

        printf("Hlavné menu\n");
        printf("%d - nová hra\n", ++i);
        
        if (1 == 1) { // if (hraPrebieha == 1)
            printf("%d - pokračovať v hre\n", ++i);
        }
        
        printf("%d - koniec\n", ++i);

        printf("\nVýber: \n");

        int c;
        scanf("%d", &c);

        printf("\n");

        if (c == 1) {
            // vytvor hru
            printf("Nová hra\n\n");

            int width, height;
            double duration;
            Board board;
            Snake snake;

            //printf("Enter board width: \n");
            //scanf("%d", &width);
            //printf("Enter board height: \n");
            //scanf("%d", &height);
            //printf("Enter game duration (seconds): \n");
            //scanf("%lf", &duration);

            initBoardFile(&board, "boba.txt");
            initSnake(&snake, 3, 4);

            placeSnake(&snake, &board);
            displayBoard(&board);

            snake.hasEaten = 1;

            int isGameOver = 0;
            char direction;
            
            while (1 == 1) {
                scanf(" %c", &direction);

                snake.direction = direction;

                if (checkSelfCollision(&snake, &board) == -1) {
                    printf("Najebal do seba! \n");
                    break;
                }

                eraseSnake(&snake, &board);
                moveSnake(&snake, &board);

                if (checkCollision(&snake, &board) == -1) {
                    printf("Najebal! \n");
                    break;
                }

                placeSnake(&snake, &board);
                displayBoard(&board);

                sleep(1);
            }

            //newGame();
        } else if (c == 2 && 1 == 1) { // if (hraPrebieha == 1)
            // pokračuj v hre
            printf("Pokračuje hra\n\n");
        } else if ((c == 2 && 1 == 0) || (c == 3 && 1 == 1)) { // if (hraPrebieha == 0)
            // koniec
            printf("Koniec\n\n");
            break;
        } else {
            printf("Zlý vstup! Vyber ešte raz\n\n");
        }
    }
    //struct timeval tv;
    //gettimeofday(&tv, NULL);
    //long long start = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
    //char *dir = mmap(NULL, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    //if (dir == MAP_FAILED) {
    //    perror("mmap failed");
    //    exit(1);
    //}

    return 0;
}