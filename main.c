#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>

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
    printf("\e[1;1H\e[2J");

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

void updateSnake(Snake *snake, Fruit *fruit, int *isGameOver, Board *board) {
    Point nextHead = snake->body[0];

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
    render(&board, &snake, &fruit);
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

    while (!isGameOver && end >= (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000)) {
        printf("Time left %lld millisecs to execute \n", end - (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
        gettimeofday(&tv, NULL);

        handleInput(&snake);
        updateSnake(&snake, &fruit, &isGameOver, &board);
    }

    if (isGameOver) {
        printf("Game Over!\n");
    } else {
        printf("Time's up!\n");
    }

    freeSnake(&snake);
    freeBoard(&board);

    return 0;
}

