#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>
#include <string.h>
#include <sys/stat.h>
#include "../shared/board.h"
#include "../shared/snake.h"
#include "../shared/timer.h"

#define END_CHAR "_"
#define PARENT_TO_CHILD_FIFO "../shared/parent_to_child_fifo"
#define CHILD_TO_PARENT_FIFO "../shared/child_to_parent_fifo"
#define TICK_LENGTH 1000LL

typedef struct {
    atomic_int gameHasEnded;
    atomic_char command;
    int fd_write;
    long long timeInMs;
    int height;
    int width;
    char *filePath;
} Control;

void initControl(Control *control) {
    control->gameHasEnded = ATOMIC_VAR_INIT(0);
    control->command = ATOMIC_VAR_INIT(' ');
    control->fd_write = 0;
    control->timeInMs = 0;
    control->height = 0;
    control->width = 0;
}

void terminateGame() {
    int fd_write = open(PARENT_TO_CHILD_FIFO, O_WRONLY);

    if (fd_write == -1) {
        perror("Failed to open parent-to-child FIFO for writing");
    }

    int fd_read = open(CHILD_TO_PARENT_FIFO, O_RDONLY);

    if (fd_read == -1) {
        perror("Failed to open child-to-parent FIFO for reading");
    }

    char end = 'e';

    write(fd_write, &end, 1);

    int bytes_read = read(fd_read, &end, 1);

    close(fd_write);
    close(fd_read);
}

void sendBoard(Board *board, int fd_write, int score, long long elapsedTime) {
    char buffer[1152];
    int offset = 0;

    buffer[offset++] = 'b';

    buffer[offset++] = '+';

    for (int i = 0; i < board->width; i++) {
        buffer[offset++] = '-';
    }

    buffer[offset++] = '+';
    buffer[offset++] = '\n';

    for (int i = 0; i < board->height; ++i) {
        buffer[offset++] = '|';

        for (int j = 0; j < board->width; ++j) {
            buffer[offset++] = board->cells[i][j];
        }

        buffer[offset++] = '|';
        buffer[offset++] = '\n';
    }

    buffer[offset++] = '+';

    for (int i = 0; i < board->width; i++) {
        buffer[offset++] = '-';
    }

    buffer[offset++] = '+';
    buffer[offset++] = '\n';

    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "S: %d | T: %lld\n", score, elapsedTime);

    buffer[offset++] = '_';

    write(fd_write, buffer, offset);
}

void *standard(void *arg) {
    int fd_write = open(CHILD_TO_PARENT_FIFO, O_WRONLY);

    if (fd_write == -1) {
        perror("Failed to open child-to-parent FIFO for writing");
    }

    int noT = 0;
    int isPaused = 0;
    int score = 0;
    Control *control = (Control *)arg;
    atomic_int *gameHasEnded = &control->gameHasEnded;
    atomic_char *command = &control->command;

    Snake snake;
    Board board;

    if (control->height == 0) {
        char basePath[] = "../server/";
        char fullPath[256];
        strcpy(fullPath, basePath);
        strcat(fullPath, control->filePath);

        initBoardFile(&board, fullPath);

        for (int i = 0; i < board.height; i++) {
            for (int j = 0; j < board.width; j++) {
                if (board.cells[i][j] == 'O') {
                    initSnake(&snake, j, i);

                    break;
                }
            }
        }
    } else {
        initSnake(&snake, control->width / 2, control->height / 2);
        initBoard(&board, control->width, control->height);
        placeSnake(&snake, &board);
    }

    placeFruit(&board);

    Timer timer;
    initTimer(&timer);
    startTimer(&timer, 0);

    sendBoard(&board, fd_write, score, getElapsedTime(&timer));

    while (atomic_load(gameHasEnded) == 0) {
        char cmmnd = atomic_load(command);

        if (cmmnd == 'e') break;

        if (isPaused == 1) {
            if (atomic_load(command) == 'u') {
                sleep(3);
                unpauseTimer(&timer);
                isPaused = 0;
            }

            if (cmmnd != ' ') atomic_store(command, ' ');

            continue;
        }

        if (cmmnd == 'w' || cmmnd == 's' || cmmnd == 'a' || cmmnd == 'd') {
            if (validateDirection(&snake, cmmnd, board.height, board.width) == 1) {
                snake.direction = cmmnd;
            }
        } else if (cmmnd == 'p' && isPaused == 0) {
            write(fd_write, &cmmnd, 1);
            pauseTimer(&timer);
            isPaused = 1;

            atomic_store(command, ' ');

            continue;
        }
        
        if (cmmnd != ' ') atomic_store(command, ' ');

        if ((getElapsedTime(&timer) / TICK_LENGTH) > noT) {
            ++noT;

            int resultOfMove = simulateMove(&snake, &board);
            

            if (resultOfMove == -1) {
                atomic_store(gameHasEnded, 1);
                cmmnd = 'e';
                write(fd_write, &cmmnd, 1);

                cmmnd = '_';
                write(fd_write, &cmmnd, 1);

                continue;
            } else {
                eraseSnake(&snake, &board);
                
                moveSnake(&snake, &board);
                
                placeSnake(&snake, &board);
                
            }

            if (resultOfMove == 1) {
                ++score;
                
                placeFruit(&board);
               
            }

            sendBoard(&board, fd_write, score, getElapsedTime(&timer));
        }
    }

    destroySnake(&snake);
    destroyBoard(&board);
    return NULL;
}

void *timed(void *arg) {
    int fd_write = open(CHILD_TO_PARENT_FIFO, O_WRONLY);

    if (fd_write == -1) {
        perror("Failed to open child-to-parent FIFO for writing");
    }

    int noT = 0;
    int isPaused = 0;
    int score = 0;
    Control *control = (Control *)arg;
    atomic_int *gameHasEnded = &control->gameHasEnded;
    atomic_char *command = &control->command;

    Snake snake;
    Board board;

    if (control->height == 0) {
        char basePath[] = "../server/";
        char fullPath[256];
        strcpy(fullPath, basePath);
        strcat(fullPath, control->filePath);

        initBoardFile(&board, fullPath);

        for (int i = 0; i < board.height; i++) {
            for (int j = 0; j < board.width; j++) {
                if (board.cells[i][j] == 'O') {
                    initSnake(&snake, j, i);

                    break;
                }
            }
        }
    } else {
        initSnake(&snake, control->width / 2, control->height / 2);
        initBoard(&board, control->width, control->height);
        placeSnake(&snake, &board);
    }

    placeFruit(&board);

    Timer timer;
    initTimer(&timer);
    startTimer(&timer, control->timeInMs);

    sendBoard(&board, fd_write, score, getElapsedTime(&timer));

    while (atomic_load(gameHasEnded) == 0) {
        char cmmnd = atomic_load(command);

        if (isPaused == 0 && getRemainingTime(&timer) <= 0) {
            atomic_store(gameHasEnded, 1);
            cmmnd = 'e';
            write(fd_write, &cmmnd, 1);

            cmmnd = '_';
            write(fd_write, &cmmnd, 1);

            continue;
        }

        if (cmmnd == 'e') break;

        if (isPaused == 1) {
            if (atomic_load(command) == 'u') {
                sleep(3);
                unpauseTimer(&timer);
                isPaused = 0;
            }

            if (cmmnd != ' ') atomic_store(command, ' ');

            continue;
        }

        if (cmmnd == 'w' || cmmnd == 's' || cmmnd == 'a' || cmmnd == 'd') {
            if (validateDirection(&snake, cmmnd, board.height, board.width) == 1) {
                snake.direction = cmmnd;
            }
        } else if (cmmnd == 'p' && isPaused == 0) {
            write(fd_write, &cmmnd, 1);
            pauseTimer(&timer);
            isPaused = 1;

            atomic_store(command, ' ');

            continue;
        }
        
        if (cmmnd != ' ') atomic_store(command, ' ');

        if ((getElapsedTime(&timer) / TICK_LENGTH) > noT) {
            ++noT;

            int resultOfMove = simulateMove(&snake, &board);
            

            if (resultOfMove == -1) {
                atomic_store(gameHasEnded, 1);
                cmmnd = 'e';
                write(fd_write, &cmmnd, 1);

                cmmnd = '_';
                write(fd_write, &cmmnd, 1);

                continue;
            } else {
                eraseSnake(&snake, &board);
                moveSnake(&snake, &board);
                placeSnake(&snake, &board);
            }

            if (resultOfMove == 1) {
                ++score;
                placeFruit(&board);
            }

            sendBoard(&board, fd_write, score, getElapsedTime(&timer));
        }
    }

    destroySnake(&snake);
    destroyBoard(&board);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (mkfifo(PARENT_TO_CHILD_FIFO, 0666) == -1) {
        perror("Failed to create parent-to-child FIFO");
    }

    if (mkfifo(CHILD_TO_PARENT_FIFO, 0666) == -1) {
        perror("Failed to create child-to-parent FIFO");
    }

    int fd_write = open(CHILD_TO_PARENT_FIFO, O_WRONLY);

    if (fd_write == -1) {
        perror("Failed to open child-to-parent FIFO for writing");
    }

    char c = '1';

    write(fd_write, &c, 1);

    pthread_t input_thread;

    Control control;
    initControl(&control);
    control.fd_write = fd_write;

    if (argc == 3) {
        control.timeInMs = atoi(argv[1]) * 1000;
        control.filePath = argv[2];

    } else if (argc == 4) {
        control.timeInMs = atoi(argv[1]) * 1000;
        control.height = atoi(argv[2]);
        control.width = atoi(argv[3]);
    }
    
    if (pthread_create(&input_thread, NULL, control.timeInMs == 0 ? &standard : &timed, (void *)&control) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    int fd_read = open(PARENT_TO_CHILD_FIFO, O_RDONLY);

    if (fd_read == -1) {
        perror("Failed to open parent-to-child FIFO for reading");
    }

    while (atomic_load(&control.gameHasEnded) == 0) {
        read(fd_read, &c, 1);

        while (atomic_load(&control.command) != ' ');

        atomic_store(&control.command, c);

        if (c == 'u') {
            sleep(3);
        } else if (c == 'e') {
            char c = 'e';

            write(fd_write, &c, 1);

            break;
        }
    }

    close(fd_read);
    close(fd_write);
    pthread_join(input_thread, NULL);

    exit(0);
}