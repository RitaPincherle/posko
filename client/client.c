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

void printOptions(int gameIsRunning) {
    int i = 0;

    printf("Main menu\n");
    printf("%d - new game\n", ++i);
    
    if (gameIsRunning == 1) {
        printf("%d - continue\n", ++i);
    }
    
    printf("%d - end\n", ++i);
}

int playingOptions() {
    int choice = 0;

    while (1 == 1) {
        printf("\nGame type options:\n");
        printf("1 - standard (until you fail)\n");
        printf("2 - timed (until time runs out)\n");

        scanf(" %d", &choice);
        while(getchar() != '\n');

        printf("\n");

        switch (choice) {
            case 1:
                return 0;
            case 2:
                while (choice < 10 || choice > 300) {
                    printf("Time of game in seconds (10 - 300): ");

                    scanf("%d", &choice);
                    while(getchar() != '\n');

                    printf("\n");

                    if (choice < 10 || choice > 300) printf("Invalid time!\n");
                }

                return choice;
            default:
                printf("Invalid choice!\n");
                continue;
        }
    }
}

int runningGameWarning() {
    char choice = ' ';

    while (1) {
        printf("\nYou have an ongoing game! Are you sure you want to proceed and abandon it? y/n: ");

        scanf( "%c", &choice);
        while(getchar() != '\n');

        printf("\n");

        switch (choice) {
            case 'y':
                return 1;
            case 'n':
                return -1;
            default:
                printf("Invalid choice!\n");
                continue;
        }
    }
}

int boardOptions() {
    int choice = 0;

    while (1 == 1) {
        printf("\nMap loading options:\n");
        printf("1 - without obstacles (user defined width and height)\n");
        printf("2 - with obstacles (file defined)\n");

        scanf("%d", &choice);
        while(getchar() != '\n');

        printf("\n");

        switch (choice) {
            case 1:
            case 2:
                return choice;
            default:
                printf("Invalid choice!\n");
                continue;
        }
    }
}

int launchServer() {
    int po = playingOptions();
    int bo = boardOptions();

    if (bo == 1) {
        int height = 0;
        int width = 0;

        while (height > 30 || height < 3) {
            printf("\nHeight of board (3 - 30): ");

            scanf(" %d", &height);
            while(getchar() != '\n');

            printf("\n");

            if (height > 30 || height < 3) {
                printf("Invalid height size!\n");
            }
        }

        while (width > 30 || width < 3) {
            printf("\nWidth of board (3 - 30): ");

            scanf(" %d", &width);
            while(getchar() != '\n');

            printf("\n");

            if (width > 30 || width < 3) {
                printf("Invalid width size!\n");
            }
        }

        pid_t pid = fork();

        if (pid == -1) return -1;

        if (pid < 0) {
            
        } else if (pid == 0) {
            char command[100];
            snprintf(command, sizeof(command), "../server/server_exec %d %d %d", po, height, width);
            int ret = system(command);

            if (ret == -1) {
                perror("Failed to launch server");
            }

            exit(0);
        }
    } else {
        char buffer[50];

        while (1) {
            printf("Enter path to desired file: ");

            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                size_t length = strlen(buffer);
                if (length > 0 && buffer[length - 1] == '\n') {
                    buffer[length - 1] = '\0';
                }

                printf("You entered: \"%s\"\n", buffer);

                printf("Internal representation: ");
                for (size_t i = 0; i < strlen(buffer) + 1; i++) {
                    if (buffer[i] == '\0') {
                        printf("\\0");
                    } else {
                        printf("%c", buffer[i]);
                    }
                }

                printf("\n");
                break;
            } else {
                printf("Error reading input.\n");
            }
        }

        pid_t pid = fork();

        if (pid == -1) return -1;

        if (pid < 0) {
            
        } else if (pid == 0) {
            char command[100];
            snprintf(command, sizeof(command), "../server/server_exec %d %s", po, buffer);
            int ret = system(command);

            if (ret == -1) {
                perror("Failed to launch server");
            }


            exit(0);
        }
    }

    return 0;
}

void *readUserInput(void *arg) {

    int fd_write = open(PARENT_TO_CHILD_FIFO, O_WRONLY);

    if (fd_write == -1) {
        perror("Failed to open parent-to-child FIFO for writing");
    }

    char c = ' ';
    atomic_int *gameHasEnded = (atomic_int *)arg;

    while (1 == 1) {

        if (atomic_load(gameHasEnded) == 1) {
            c = 'e';

            close(fd_write);
            break;
        }

        if (c == 'p') {
            break;
        }

        scanf("%c", &c);
        while(getchar() != '\n');

        if (atomic_load(gameHasEnded) == 0 && (c == 'w' || c == 's' || c == 'a' || c == 'd' || c == 'p')) write(fd_write, &c, 1);
    }

    close(fd_write);

    return NULL;
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

int main() {
    int gameIsRunning = 0;
    int choice = 0;

    unlink(PARENT_TO_CHILD_FIFO);

    if (mkfifo(PARENT_TO_CHILD_FIFO, 0666) == -1) {
        perror("Failed to create parent-to-child FIFO");
    }

    unlink(CHILD_TO_PARENT_FIFO);

    if (mkfifo(CHILD_TO_PARENT_FIFO, 0666) == -1) {
        perror("Failed to create child-to-parent FIFO");
    }

    while ((choice != 2 && gameIsRunning == 0) || (choice != 3 && gameIsRunning == 1)) {
        printOptions(gameIsRunning);

        printf("\nChoice: ");
        scanf(" %d", &choice);
        while(getchar() != '\n');

        printf("\n");

        if (choice == 1) {
            choice = 0;
            printf("New game\n\n");

            if (gameIsRunning == 1) {
                int rgw = runningGameWarning();

                if (rgw == 1) terminateGame();
                else if (rgw == -1) continue;
            }

            if (launchServer() == -1) {
                printf("Failed to launch the server!\n");
                continue;
            }

            int fd_read = open(CHILD_TO_PARENT_FIFO, O_RDONLY);

            if (fd_read == -1) {
                perror("Failed to open child-to-parent FIFO for reading");

                continue;
            }

            char launched;
            read(fd_read, &launched, 1);
            if (launched == '1') printf("Successfully launched the game!\n");

            pthread_t input_thread;
            atomic_int gameHasEnded = ATOMIC_VAR_INIT(0);

            if (pthread_create(&input_thread, NULL, &readUserInput, (void *)&gameHasEnded) != 0) {
                perror("Failed to create thread");
                return 1;
            }

            char c = ' ';
            int bytes_read;

            while ((bytes_read = read(fd_read, &c, 1)) > 0) {
                if (c == 'b') {
                    printf("\e[1;1H\e[2J");

                    while ((bytes_read = read(fd_read, &c, 1)) > 0) {
                        if (c == '_') {
                            break;
                        } else {
                            printf("%c", c);
                        }
                    }
                } else if (c == 'e') {
                    printf("\e[1;1H\e[2J");
                    atomic_store(&gameHasEnded, 1);
                    gameIsRunning = 0;

                    while ((bytes_read = read(fd_read, &c, 1)) > 0) {
                        if (c == '_') {

                            printf("Game over!\n");

                            break;
                        } else {
                            printf("%c", c);
                        }
                    }

                    break;
                } else if (c == 'p') {
                    gameIsRunning = 1;
                    break;
                }
            }
            pthread_join(input_thread, NULL);

            close(fd_read);
        } else if (choice == 2 && gameIsRunning == 1) {
            choice = 0;
            int fd_write = open(PARENT_TO_CHILD_FIFO, O_WRONLY);

            if (fd_write == -1) {
                perror("Failed to open parent-to-child FIFO for writing");
            }

            char unpause = 'u';

            write(fd_write, &unpause, 1);

            sleep(3);

            close(fd_write);

            int fd_read = open(CHILD_TO_PARENT_FIFO, O_RDONLY);

            if (fd_read == -1) {
                perror("Failed to open child-to-parent FIFO for reading");

                continue;
            }

            pthread_t input_thread;
            atomic_int gameHasEnded = ATOMIC_VAR_INIT(0);

            if (pthread_create(&input_thread, NULL, &readUserInput, (void *)&gameHasEnded) != 0) {
                perror("Failed to create thread");
                return 1;
            }

            char c = ' ';
            int bytes_read;

            while ((bytes_read = read(fd_read, &c, 1)) > 0) {
                if (c == 'b') {
                    printf("\e[1;1H\e[2J");

                    while ((bytes_read = read(fd_read, &c, 1)) > 0) {
                        if (c == '_') {
                            break;
                        } else {
                            printf("%c", c);
                        }
                    }
                } else if (c == 'e') {
                    printf("\e[1;1H\e[2J");
                    atomic_store(&gameHasEnded, 1);
                    gameIsRunning = 0;

                    while ((bytes_read = read(fd_read, &c, 1)) > 0) {
                        if (c == '_') {

                            printf("Game over!\n");

                            break;
                        } else {
                            printf("%c", c);
                        }
                    }

                    break;
                } else if (c == 'p') {
                    gameIsRunning = 1;
                    break;
                }
            }

            pthread_join(input_thread, NULL);

            close(fd_read);
        } else if ((choice == 2 && gameIsRunning == 0) || (choice == 3 && gameIsRunning == 1)) {
            choice = 0;
            

            if (gameIsRunning == 1) {
                int rgw  = runningGameWarning();
                
                if (rgw == 1) {
                    terminateGame();
                    break;
                } else {
                    continue;
                }
            }
            printf("End\n\n");
            break;
        } else {
            printf("Wrong input! Try again!\n\n");
        }
    }

    return 0;
}