#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define ENTER_KEY 13

#define ROWS 10
#define COLS 20

#define EMPTY ' '
#define SNOOPY 'S'
#define BIRD 'B'
#define BALL 'O'
#define INVINCIBLE_BLOC '#'

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define BIRD_SYMBOL "♫"
#define SNOOPY_SYMBOL "☺"
#define BALL_SYMBOL "♂"
#define INVINCIBLE_BLOC_SYMBOL "☼"

typedef struct {
    int x;
    int y;
    char newValue;
} Update;

// main functions declarations
void startGame();

void displayGameRules();

void startNewGame();

void loadSavedGame();

void displayLevels();

void displayRecordedScores();

void displayGameControls();

void quitGame();

// helper functions declarations
void displayWelcomeBanner();

void clearScreen();

char waitForKeyHit();

int displayMenuOption();

int menuSelector(int, int, int);

void moveCursor(int, int);

void printGameBoard(char[ROWS][COLS]);

void delay();

void printSymbol(char value);

// main program
int main() {
    // Set console to use UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    startGame();
    return 0;
}

// main functions implementations
void startGame() {
    int selectedOption;

    do {
        displayWelcomeBanner();
        selectedOption = displayMenuOption();

        switch (selectedOption) {
            case 0:
                displayGameRules();
                break;
            case 1:
                startNewGame();
                break;
            case 2:
                loadSavedGame();
                break;
            case 3:
                displayLevels();
                break;
            case 4:
                displayRecordedScores();
                break;
            case 5:
                displayGameControls();
                break;
            case 6:
                quitGame();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (selectedOption != 6);
}

void displayGameRules() {
    clearScreen();

    printf("Game rules\n\n");
    printf("Bienvenue dans Snoopy!\n");
    printf("Les regles sont les suivantes :\n");
    printf("- Utiliser les touches fleches pour deplacer Snoopy.\n");
    printf("- Collecter tous les oiseaux pour passer au niveau suivant.\n");
    printf("- Eviter la balle et ne vous faites pas toucher.\n");
    printf("- 120 secondes pour chaque niveau.\n");
    printf("\nPress Any Key To Go Back to The Menu Screen...");

    waitForKeyHit();
}

void moveBallDiagonally(int *ballX, int *ballY, int *directionX, int *directionY, char boardGame[ROWS][COLS]) {
    int nextX = *ballX + *directionX;
    int nextY = *ballY + *directionY;

    // Collision checks
    if (nextX < 0 || nextX >= ROWS) {
        *directionX = -*directionX;
    }
    if (nextY < 0 || nextY >= COLS) {
        *directionY = -*directionY;
    }

    if (boardGame[nextX][nextY] == BIRD || boardGame[nextX][nextY] == SNOOPY) {
        *directionX = -*directionX;
        *directionY = -*directionY;
    }

    if (boardGame[nextX - *directionX][nextY] == INVINCIBLE_BLOC &&
        boardGame[nextX][nextY - *directionY] == INVINCIBLE_BLOC) {
        *directionX = -*directionX;
        *directionY = -*directionY;
    }

    // Update ball's position if no collision
    *ballX += *directionX;
    *ballY += *directionY;
}

void initializeBoardGame(char board[ROWS][COLS], int snoopyX, int snoopyY, int ballX, int ballY) {
    int i, j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            board[i][j] = EMPTY;
        }
    }

    board[snoopyX][snoopyY] = SNOOPY;
    board[ballX][ballY] = BALL;

    //? birds
    board[0][0] = BIRD;
    board[0][COLS - 1] = BIRD;
    board[ROWS - 1][0] = BIRD;
    board[ROWS - 1][COLS - 1] = BIRD;

    //? obstacles
    board[4][4] = INVINCIBLE_BLOC;
    board[4][5] = INVINCIBLE_BLOC;
    board[5][6] = INVINCIBLE_BLOC;
    board[6][7] = INVINCIBLE_BLOC;
    board[0][1] = INVINCIBLE_BLOC;
    board[0][COLS - 2] = INVINCIBLE_BLOC;
    board[1][COLS - 2] = INVINCIBLE_BLOC;
}

void addUpdate(int x, int y, char newValue, int *index, Update updates[]) {
    updates[*index].newValue = newValue;
    updates[*index].x = x;
    updates[*index].y = y;

    (*index)++;
}

void moveSnoopy(char key, int *snoopyX, int *snoopyY, char board[ROWS][COLS], int *score, Update updates[],
                int *updateIndex) {
    int number;
    const int x = 6;
    const int y = -2;

    switch (key) {
        case UP_ARROW:
            if (*snoopyX > 0) {
                if (board[*snoopyX - 1][*snoopyY] == BIRD) {
                    (*score)++;

                    number = *score;
                    char charRepresentation = '0' + number;
                    addUpdate(x, y, charRepresentation, updateIndex, updates);
                }

                if (board[*snoopyX - 1][*snoopyY] != INVINCIBLE_BLOC) {
                    board[*snoopyX][*snoopyY] = EMPTY;
                    addUpdate(*snoopyY, *snoopyX, EMPTY, updateIndex, updates);
                    board[--(*snoopyX)][*snoopyY] = SNOOPY;
                    addUpdate(*snoopyY, *snoopyX, SNOOPY, updateIndex, updates);
                }
            }
            break;
        case DOWN_ARROW:
            if (*snoopyX < ROWS - 1) {
                if (board[*snoopyX + 1][*snoopyY] == BIRD) {
                    (*score)++;

                    number = *score;
                    char charRepresentation = '0' + number;
                    addUpdate(x, y, charRepresentation, updateIndex, updates);
                }

                if (board[*snoopyX + 1][*snoopyY] != INVINCIBLE_BLOC) {
                    addUpdate(*snoopyY, *snoopyX, EMPTY, updateIndex, updates);
                    board[*snoopyX][*snoopyY] = EMPTY;
                    board[++(*snoopyX)][*snoopyY] = SNOOPY;
                    addUpdate(*snoopyY, *snoopyX, SNOOPY, updateIndex, updates);
                }
            }
            break;
        case LEFT_ARROW:
            if (*snoopyY > 0) {
                if (board[*snoopyX][*snoopyY - 1] == BIRD) {
                    (*score)++;

                    number = *score;
                    char charRepresentation = '0' + number;
                    addUpdate(x, y, charRepresentation, updateIndex, updates);
                }

                if (board[*snoopyX][*snoopyY - 1] != INVINCIBLE_BLOC) {
                    addUpdate(*snoopyY, *snoopyX, EMPTY, updateIndex, updates);
                    board[*snoopyX][*snoopyY] = EMPTY;
                    board[*snoopyX][--(*snoopyY)] = SNOOPY;
                    addUpdate(*snoopyY, *snoopyX, SNOOPY, updateIndex, updates);
                }
            }
            break;
        case RIGHT_ARROW:
            if (*snoopyY < COLS - 1) {
                if (board[*snoopyX][*snoopyY + 1] == BIRD) {
                    (*score)++;

                    number = *score;
                    char charRepresentation = '0' + number;

                    addUpdate(x, y, charRepresentation, updateIndex, updates);
                }

                if (board[*snoopyX][*snoopyY + 1] != INVINCIBLE_BLOC) {
                    addUpdate(*snoopyY, *snoopyX, EMPTY, updateIndex, updates);
                    board[*snoopyX][*snoopyY] = EMPTY;
                    board[*snoopyX][++(*snoopyY)] = SNOOPY;
                    addUpdate(*snoopyY, *snoopyX, SNOOPY, updateIndex, updates);
                }
            }
            break;
    }
}

void updateElements(char boardGame[ROWS][COLS], Update updates[], int numberUpdates) {
    for (int i = 0; i < numberUpdates; i++) {
        int x = updates[i].x;
        int y = updates[i].y;
        char newValue = updates[i].newValue;
        boardGame[y][x] = newValue;

        int y_offset = 5;
        int x_offset = 2;
        moveCursor(x + x_offset, y + y_offset); // Adjust the coordinates based on your console's cursor positioning

        printSymbol(newValue);
    }

    moveCursor(0, 18);
}

void printSymbol(char value) {
    if (value == BIRD) {
        printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET, BIRD_SYMBOL);
    } else if (value == SNOOPY) {
        printf(ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET, SNOOPY_SYMBOL);
    } else if (value == BALL) {
        printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, BALL_SYMBOL);
    } else if (value == INVINCIBLE_BLOC) {
        printf(ANSI_COLOR_RED "%s"ANSI_COLOR_RESET, INVINCIBLE_BLOC_SYMBOL);
    } else {
        printf("%c", value);
    }
}

void startNewGame() {
    clearScreen();
    printf("New game\n\n");

    int score = 0;
    printf("Score: %d |♥ : 0 | ★ :3\n", score);

    int snoopyX = 0, snoopyY = 2;
    int ballX = 6, ballY = 4;
    // Time-based ball movement variables
    int ballTimer = 0;
    int ballMoveInterval = 10;
    int directionX = 1, directionY = 1;

    char boardGame[ROWS][COLS];
    initializeBoardGame(boardGame, snoopyX, snoopyY, ballX, ballY);
    printGameBoard(boardGame);

    int numberUpdates = 0;
    Update updates[100000];

    // Get user input for direction
    char key;
    printf("\nEnter direction (UP/DOWN/LEFT/RIGHT) or tape the \"q\" to quit. ");
    do {
        numberUpdates = 0;

        if (kbhit()) {
            key = (char) getch();
            moveSnoopy(key, &snoopyX, &snoopyY, boardGame, &score, updates, &numberUpdates);
        }

        fflush(stdin);

        if (ballTimer % ballMoveInterval == 0) {
            boardGame[ballX][ballY] = EMPTY;
            addUpdate(ballY, ballX, EMPTY, &numberUpdates, updates);

            moveBallDiagonally(&ballX, &ballY, &directionX, &directionY, boardGame);

            boardGame[ballX][ballY] = BALL;
            addUpdate(ballY, ballX, BALL, &numberUpdates, updates);
        }

        ballTimer++;

        // Update specific elements in the matrix
        updateElements(boardGame, updates, numberUpdates);

        delay();
    } while (key != 'q' && score < 4);

    if (score == 4) {
        moveCursor(0, 17);
        printf("Congrats! You Won this level!                                   \n");
    }

    printf("\nPress Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void delay() {
    struct timespec delay;
    delay.tv_sec = 0;
    // 100,000,000 nanoseconds = 0.1 seconds
    delay.tv_nsec = 100000000 / 4;

    nanosleep(&delay, NULL);
}

void displayGameControls() {
    clearScreen();
    printf("Controls\n\n");
    printf("Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void displayRecordedScores() {
    clearScreen();
    printf("Recorded Scores\n\n");
    printf("Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void displayLevels() {
    clearScreen();
    printf("Levels\n\n");
    printf("Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void loadSavedGame() {
    clearScreen();
    printf("Load Saved Game\n\n");
    printf("Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void quitGame() {
    clearScreen();
    printf("Press Any Key To Go Back to The close the game...");
    waitForKeyHit();
}


// helper functions implementations
void displayWelcomeBanner() {
    clearScreen();
    printf("___________________________________________\n");
    printf("   _____                                         \n");
    printf("  / ___/ ____   ____   ____   ____   __  __      \n");
    printf("  \\__ \\ / __ \\ / __ \\ / __ \\ / __ \\ / / / /\n");
    printf(" ___/ // / / // /_/ // /_/ // /_/ // /_/ /       \n");
    printf("/____//_/ /_/ \\____/ \\____// .___/ \\__, /     \n");
    printf("                          /_/     /____/         \n");
    printf("___________________________________________\n");
}

void clearScreen() {
    system("cls");
}

int displayMenuOption() {
    int x = 4, y = 10;
    int yStart = y;
    int selectedOption;

    moveCursor(x, y++);
    printf("- Game rules\n");
    moveCursor(x, y++);
    printf("- Start game \n");
    moveCursor(x, y++);
    printf("- Load Saved Game\n");
    moveCursor(x, y++);
    printf("- Levels \n");
    moveCursor(x, y++);
    printf("- Recorded Scores  \n");
    moveCursor(x, y++);
    printf("- Game Controls \n");
    moveCursor(x, y++);
    printf("- Exit\n");
    moveCursor(x, y++);

    selectedOption = menuSelector(x, y, yStart);

    return (selectedOption);
}

char waitForKeyHit() {
    int pressed;
    while (!kbhit());
    pressed = getch();
    return ((char) pressed);
}

int menuSelector(int x, int y, int yStart) {
    char key;
    int selectedOptionIndex = 0;
    x = x - 2;
    moveCursor(x, yStart);
    printf(">");
    moveCursor(x + 1, yStart);

    do {
        key = waitForKeyHit();

        if (key == (char) UP_ARROW) {
            moveCursor(x, yStart + selectedOptionIndex);
            printf(" ");

            if (yStart >= yStart + selectedOptionIndex)
                selectedOptionIndex = y - yStart - 2;
            else
                selectedOptionIndex--;
            moveCursor(x, yStart + selectedOptionIndex);
            printf(">");
        } else if (key == (char) DOWN_ARROW) {

            moveCursor(x, yStart + selectedOptionIndex);
            printf(" ");

            if (selectedOptionIndex + 2 >= y - yStart)
                selectedOptionIndex = 0;
            else
                selectedOptionIndex++;
            moveCursor(x, yStart + selectedOptionIndex);
            printf(">");
        }

    } while (key != (char) ENTER_KEY);
    return (selectedOptionIndex);
}

void moveCursor(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

void printGameBoard(char boardGame[ROWS][COLS]) {
    int i, j;
    printf("╔════════════════════╗\n");
    for (i = 0; i < ROWS; i++) {
        printf("║");
        for (j = 0; j < COLS; j++) {
            printSymbol(boardGame[i][j]);
        }
        printf("║\n");
    }
    printf("╚════════════════════╝\n");
}
