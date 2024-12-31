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

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point *body;
    int length;
    char direction;
} Snake;

typedef struct {
    int width, height;
    char **cells;
} Board;

typedef struct {
    Point position;
} Fruit;

void initBoard(Board *board, int width, int height) {
    board->width = width;
    board->height = height;
    board->cells = malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++) {
        board->cells[i] = malloc(width * sizeof(char));
        for (int j = 0; j < width; j++) {
            board->cells[i][j] = ' ';
        }
    }
}

void freeBoard(Board *board) {
    for (int i = 0; i < board->height; i++) {
        free(board->cells[i]);
    }
    free(board->cells);
}

void placeObstacles(Board *board) {
    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {
            if (i == 0 || i == board->height - 1 || j == 0 || j == board->width - 1) {
                board->cells[i][j] = '#';
            }
        }
    }
}

void initSnake(Snake *snake, int startX, int startY) {
    snake->length = 1;
    snake->body = malloc(sizeof(Point));
    snake->body[0].x = startX;
    snake->body[0].y = startY;
    snake->direction = RIGHT;
}

void freeSnake(Snake *snake) {
    free(snake->body);
}

void initFruit(Fruit *fruit, Board *board) {
    do {
        fruit->position.x = rand() % (board->width - 2) + 1;
        fruit->position.y = rand() % (board->height - 2) + 1;
    } while (board->cells[fruit->position.y][fruit->position.x] != ' ');
}

void render(Board *board, Snake *snake, Fruit *fruit) {
    //printf("\e[1;1H\e[2J");

    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {
            board->cells[i][j] = ' ';
        }
    }

    placeObstacles(board);

    for (int i = 0; i < snake->length; i++) {
        board->cells[snake->body[i].y][snake->body[i].x] = (i == 0) ? 'O' : 'o';
    }

    board->cells[fruit->position.y][fruit->position.x] = '@';

    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {
            putchar(board->cells[i][j]);
        }
        putchar('\n');
    }
}

void updateSnake(Snake *snake, Fruit *fruit, int *isGameOver, Board *board, char *c) {
    Point nextHead = snake->body[0];
    printf("%c dvojta bomba\n", *c);

    if ((*c == UP && snake->direction != DOWN) ||
               (*c == DOWN && snake->direction != UP) ||
               (*c == LEFT && snake->direction != RIGHT) ||
               (*c == RIGHT && snake->direction != LEFT)) 
               {
                snake->direction = *c;
                }

    switch (snake->direction) {
        case UP: nextHead.y--; break;
        case DOWN: nextHead.y++; break;
        case LEFT: nextHead.x--; break;
        case RIGHT: nextHead.x++; break;
    }

    if (nextHead.x <= 0 || nextHead.x >= board->width - 1 ||
        nextHead.y <= 0 || nextHead.y >= board->height - 1) {
        *isGameOver = 1;
        return;
    }

    for (int i = 0; i < snake->length; i++) {
        if (snake->body[i].x == nextHead.x && snake->body[i].y == nextHead.y) {
            *isGameOver = 1;
            return;
        }
    }

    if (nextHead.x == fruit->position.x && nextHead.y == fruit->position.y) {
        snake->length++;
        snake->body = realloc(snake->body, snake->length * sizeof(Point));
        initFruit(fruit, board);
    }

    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }

    snake->body[0] = nextHead;
    render(board, snake, fruit);
}

void handleInput(Snake *snake) {
    char c = getchar();

    if ((c == UP && snake->direction != DOWN) ||
               (c == DOWN && snake->direction != UP) ||
               (c == LEFT && snake->direction != RIGHT) ||
               (c == RIGHT && snake->direction != LEFT)) 
               {
                snake->direction = c;
               } else {
                
                return;
               }
}

void changeDir(char *dir){
    *dir = getchar();
    printf("%c\n", *dir);
}

int main() {
    srand(time(NULL));

    int width, height;
    double duration;

    printf("Enter board width: ");
    scanf("%d", &width);
    printf("Enter board height: ");
    scanf("%d", &height);
    printf("Enter game duration (seconds): ");
    scanf("%lf", &duration);

    Board board;
    Snake snake;
    Fruit fruit;

    initBoard(&board, width, height);
    initSnake(&snake, width / 2, height / 2);
    initFruit(&fruit, &board);

    struct timeval tv;
    int isGameOver = 0;

    gettimeofday(&tv, NULL);
    long long start = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
    long long end = start + duration * 1000;

    printf("%lld, %lld", (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000) - start, end - start);
    printf("%d", end >= (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));

    char *dir = mmap(NULL, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (dir == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    *dir = RIGHT;

    int id = fork();
    
    if (id == 0) {
        while (!isGameOver) {
            char input = getchar();

            if (input == UP || input == DOWN || input == LEFT || input == RIGHT) {
                *dir = input;
            }
        }

        return 0;
    } else {
        long long tick = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
        long long noTicks = 0;

        while (!isGameOver && end >= (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000)) {
            tick = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
            gettimeofday(&tv, NULL);

            if ((tick - start) / 500 > noTicks) {
                ++noTicks;

                char latestDir = *dir;
                printf("Parent reads direction: %c\n", latestDir);
                updateSnake(&snake, &fruit, &isGameOver, &board, dir);
            }
        }
    }

    if (isGameOver) {
        printf("Game Over!\n");
    } else {
        isGameOver = 1;
        printf("Time's up!\n");
    }

    freeSnake(&snake);
    freeBoard(&board);
    munmap(dir, sizeof(char));

    return 0;
}

