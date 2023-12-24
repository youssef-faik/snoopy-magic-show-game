#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define ENTER_KEY 13

#define AVAILABLE_LEVELS 3
#define MAX_NUM_PASSWORDS AVAILABLE_LEVELS
#define MAX_PASSWORD_LENGTH 6

#define ROWS 10
#define COLS 20

#define DEFAULT_GAME_DURATION 60

#define EMPTY ' '
#define SNOOPY 'S'
#define BIRD 'B'
#define BALL 'O'
#define INVINCIBLE_BLOC '#'
#define PUSHABLE_BLOC 'P'

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define BIRD_SYMBOL "♫"
#define SNOOPY_SYMBOL "☺"
#define BALL_SYMBOL "♂"
#define STAR_SYMBOL "★"
#define INVINCIBLE_BLOC_SYMBOL "☼"
#define PUSHABLE_BLOC_SYMBOL "▬"

#define UP_ARROW_SYMBOL "↑"
#define DOWN_ARROW_SYMBOL "↓"
#define LEFT_ARROW_SYMBOL "←"
#define RIGHT_ARROW_SYMBOL "→"

enum LeveLResult {
    WON, LOST, QUIT
};

typedef struct {
    int x;
    int y;
    char newValue;
} Update;

// main functions declarations
void runGameApp();

void displayGameRules();

void startGame(int level, int *globalScore);

void loadSavedGame();

void displayLevels(int *globalScore);

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

void printGameBoard(char[ROWS][COLS], int score, int remainingLives);

void delay();

void printSymbol(char value);

void initializeCountdownTimer(int *remainingTime);

void checkRemainingTimeAndUpdate(int *remainingTime, clock_t *lastCheckTime);

void displayLevelResult(enum LeveLResult isLevelWon, int level);

void addUpdate(int x, int y, char newValue, int *index, Update updates[]);

void updateElementsDisplay(char boardGame[ROWS][COLS], Update updates[], int numberUpdates);

void
moveSnoopy(char board[10][20], int *snoopyX, int *snoopyY, char key, int *score, enum LeveLResult *isLevelWon,
           int *numberUpdates, Update updates[]);

void
updateBallPlacement(char boardGame[10][20], int *ballX, int *ballY, int *directionX, int *directionY,
                    enum LeveLResult *isLevelWon, Update updates[100000], int *numberUpdates);

enum LeveLResult playLevel(int level, int *globalScore, int remainingLives);

void readGameBoardElementsFromFile(int level, char boardGame[ROWS][COLS], int *ballX, int *ballY, int *snoopyX,
                                   int *snoopyY);

int isLevelAvailable(int level);

int validatePassword(int level, const char *enteredPassword);

void displayAvailableLevels();

void loadPasswordsFile(char passwords[MAX_NUM_PASSWORDS][MAX_PASSWORD_LENGTH]);

// main program
int main() {
    // Set console to use UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    runGameApp();

    return 0;
}

// main functions implementations
void runGameApp() {
    int globalScore = 0;
    int selectedOption;

    do {
        displayWelcomeBanner();
        selectedOption = displayMenuOption();

        switch (selectedOption) {
            case 0:
                displayGameRules();
                break;
            case 1:
                startGame(1, &globalScore);
                break;
            case 2:
                loadSavedGame();
                break;
            case 3:
                displayLevels(&globalScore);
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

void startGame(int level, int *globalScore) {
    int remainingLives = 3;

    while (remainingLives > 0) {
        enum LeveLResult isLevelWon = playLevel(level, globalScore, remainingLives);

        if (isLevelWon == LOST) {
            remainingLives--;
        } else if (isLevelWon == WON) {
            if (level == AVAILABLE_LEVELS) {
                moveCursor(0, 17);

                printf(ANSI_COLOR_YELLOW"★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");
                printf(STAR_SYMBOL ANSI_COLOR_RESET " YOU HAVE SUCCESSFULLY COMPLETED ALL LEVELS. YOU ARE A GAMING MASTER! " ANSI_COLOR_YELLOW STAR_SYMBOL);
                printf("\n★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n\n"ANSI_COLOR_RESET);

                printf("Press \"enter\" to Go Back to The Menu Screen....");
                while ((int) waitForKeyHit() != ENTER_KEY);

                return;
            }

            level++;
        } else {
            break;
        }
    }

    printf("\nPress Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void displayGameControls() {
    clearScreen();
    printf(ANSI_COLOR_GREEN".......................Controls.......................\n");
    printf(ANSI_COLOR_RESET"Movement:\n");
    printf("  ↑ : Move Up\n");
    printf("  ↓ : Move Down\n");
    printf("  ← : Move Left\n");
    printf("  → : Move Right\n");
    printf("\n");
    printf("Game Actions:\n");
    printf("  P : Pause/Unpause the game\n");
    printf("  Special keys to interact with blocks\n");
    printf("\n");
    printf("Winning Conditions:\n");
    printf("  - Collect all 4 birds without getting hit by the ball\n");
    printf("  - Finish the level within 120 seconds.\n");
    printf("Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void displayRecordedScores() {
    clearScreen();
    printf("Recorded Scores\n\n");
    printf("Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void displayLevels(int *globalScore) {
    clearScreen();
    printf("Levels\n\n");

    char input[20];

    while (1) {
        displayAvailableLevels();

        printf("Enter the level number (or 'q' to quit): ");
        scanf("%s", input);

        if (input[0] == 'q' || input[0] == 'Q') {
            break;
        }

        int level = atoi(input);

        if (isLevelAvailable(level)) {
            char password[20];

            while (1) {
                printf("Enter the password for level %d (or 'q' to quit): ", level);
                scanf("%s", password);

                if (password[0] == 'q' || password[0] == 'Q') {
                    printf("\nQuitting the game. Press Any Key To Go Back to The Menu Screen...");
                    waitForKeyHit();
                    return;
                }

                if (validatePassword(level, password)) {
                    printf(ANSI_COLOR_GREEN"Password correct!  Press Any Key To Start...\n"ANSI_COLOR_RESET);
                    waitForKeyHit();
                    startGame(level, globalScore);
                    return;
                } else {
                    printf(ANSI_COLOR_RED"Wrong password. Please try again.\n\n"ANSI_COLOR_RESET);
                }
            }
        } else {
            printf(ANSI_COLOR_RED "Invalid level. Please choose a valid level.\n\n"ANSI_COLOR_RESET);
        }
    }

    printf("\nQuitting the game. Press Any Key To Go Back to The Menu Screen...");
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

void delay() {
    struct timespec delay;
    delay.tv_sec = 0;
    // 100,000,000 nanoseconds = 0.1 seconds
    delay.tv_nsec = 100000000 / 4;

    nanosleep(&delay, NULL);
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
    }
    else if (value == PUSHABLE_BLOC) {
        printf(ANSI_COLOR_BLUE "%s"ANSI_COLOR_RESET, PUSHABLE_BLOC_SYMBOL);
    }
     else {
        printf("%c", value);
    }
}

void printGameBoard(char boardGame[ROWS][COLS], int score, int remainingLives) {
    printf(
            ANSI_COLOR_BLUE"♫" ANSI_COLOR_RESET ": %d  | "
            ANSI_COLOR_RED"♥" ANSI_COLOR_RESET": %d  | "
            ANSI_COLOR_GREEN "★"ANSI_COLOR_RESET": %d\n",
            score,
            remainingLives,
            DEFAULT_GAME_DURATION
    );

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

    printf("\nEnter direction (" UP_ARROW_SYMBOL"/" DOWN_ARROW_SYMBOL "/" RIGHT_ARROW_SYMBOL "/" LEFT_ARROW_SYMBOL "), \"p\" to pause or \"q\" to quit. ");
}

void initializeCountdownTimer(int *remainingTime) {
    *remainingTime = DEFAULT_GAME_DURATION;
}

void checkRemainingTimeAndUpdate(int *remainingTime, clock_t *lastCheckTime) {
    clock_t currentTime = clock();
    double timeElapsed = ((double) (currentTime - *lastCheckTime)) / CLOCKS_PER_SEC;
    const int TIME_X = 20;
    const int TIME_Y = 3;

    if (timeElapsed >= 1.0) {
        *lastCheckTime = currentTime;
        if (*remainingTime >= 0) {
            moveCursor(TIME_X, TIME_Y);
            printf("%d    ", *remainingTime);

            moveCursor(0, 18);
            (*remainingTime)--;
        }
    }
}

enum LeveLResult playLevel(int level, int *globalScore, int remainingLives) {
    int score = 0;
    enum LeveLResult isLevelWon = QUIT;

    int snoopyX;
    int snoopyY;

    int ballX;
    int ballY;

    int directionY = 1;
    int directionX = 1;
    printf("%d", level);
    // Time-based ball movement variables
    int ballTimer = 0;
    int ballMoveInterval = 10;

    int remainingTime;
    initializeCountdownTimer(&remainingTime);
    clock_t lastCheckTime = clock();

    char boardGame[ROWS][COLS];
    readGameBoardElementsFromFile(level, boardGame, &ballX, &ballY, &snoopyX, &snoopyY);

    clearScreen();
    printf("Level: %d\n\n", level);
    printGameBoard(boardGame, score, remainingLives);

    int numberUpdates = 0;
    Update updates[100000];

    // Get user input for direction
    while (score < 4 && remainingTime >= 0) {
        checkRemainingTimeAndUpdate(&remainingTime, &lastCheckTime);
        addUpdate(ballY, ballX, BALL, &numberUpdates, updates);

        numberUpdates = 0;

        if (kbhit()) {
            char key = (char) getch();

            if (key == 'q') { return QUIT; }
            else if (key == 'p') {
                moveCursor(0, 17);
                printf("GAME PAUSED PRESS \"p\" TO RESUME                                   \n\n");

                while ((int) waitForKeyHit() != 'p');

                moveCursor(0, 17);
                printf("Enter direction (" UP_ARROW_SYMBOL"/" DOWN_ARROW_SYMBOL "/" RIGHT_ARROW_SYMBOL "/" LEFT_ARROW_SYMBOL "), \"p\" to pause or \"q\" to quit. ");

            }

            moveSnoopy(boardGame, &snoopyX, &snoopyY, key, &score, &isLevelWon, &numberUpdates, updates);
            fflush(stdin);
        }

        if (ballTimer % ballMoveInterval == 0) {
            updateBallPlacement(boardGame, &ballX, &ballY, &directionX, &directionY, &isLevelWon, updates,
                                &numberUpdates);

            if (isLevelWon == LOST) {
                return LOST;
            }
        }

        ballTimer++;

        updateElementsDisplay(boardGame, updates, numberUpdates);

        delay();
    }

    if (score == 4) {
        isLevelWon = WON;
    }

    if (score < 4 && remainingTime < 0) {
        isLevelWon = LOST;
    }

    displayLevelResult(isLevelWon, level);

    return isLevelWon;
}

void displayLevelResult(enum LeveLResult isLevelWon, int level) {
    if (isLevelWon == WON && level < AVAILABLE_LEVELS) {
        moveCursor(0, 17);

        char passwords[MAX_NUM_PASSWORDS][MAX_PASSWORD_LENGTH];
        loadPasswordsFile(passwords);
        printf(ANSI_COLOR_GREEN "CONGRATS! YOU WON LEVEL %d!                                   \n" ANSI_COLOR_RESET,
               level);

        printf("LEVEL %d PASSWORD: " ANSI_COLOR_GREEN "%s                                   \n\n" ANSI_COLOR_RESET,
               level + 1, passwords[level]);

        printf("Press \"enter\" to continue.");
        while ((int) waitForKeyHit() != ENTER_KEY);
    }

    if (isLevelWon == LOST) {
        moveCursor(0, 17);
        printf(ANSI_COLOR_RED "TIME IS UP!                                                       \n\n" ANSI_COLOR_RESET);

        printf("Press \"enter\" to continue.");
        while ((int) waitForKeyHit() != ENTER_KEY);
    }


}

void moveBallDiagonally(char boardGame[10][20], int *ballX, int *ballY, int *directionX, int *directionY,
                        enum LeveLResult *isLevelWon) {
    int nextX = *ballX + *directionX;
    int nextY = *ballY + *directionY;

    // Collision checks
    if (nextX < 0 || nextX >= ROWS) {
        *directionX = -*directionX;
    } else if (nextY < 0 || nextY >= COLS) {
        *directionY = -*directionY;
    } else if (boardGame[nextX][nextY] == BIRD) {
        *directionX = -*directionX;
        *directionY = -*directionY;
    } else if (boardGame[nextX][nextY] == SNOOPY) {
        *isLevelWon = LOST;
        return;
    } else if (boardGame[nextX][nextY] == INVINCIBLE_BLOC) {
        if (boardGame[nextX - *directionX][nextY] == INVINCIBLE_BLOC ||
            boardGame[nextX + *directionX][nextY] == INVINCIBLE_BLOC)
            *directionY = -*directionY;

        else if (boardGame[nextX][nextY + *directionY] == INVINCIBLE_BLOC ||
                 boardGame[nextX][nextY - *directionY] == INVINCIBLE_BLOC)
            *directionX = -*directionX;

        else {
            *directionX = -*directionX;
            *directionY = -*directionY;
        }
    } else if (boardGame[nextX - *directionX][nextY] == INVINCIBLE_BLOC &&
               boardGame[nextX][nextY - *directionY] == INVINCIBLE_BLOC) {
        *directionX = -*directionX;
        *directionY = -*directionY;
    }
    else
    if (boardGame[nextX][nextY] == PUSHABLE_BLOC) 
{
        if (boardGame[nextX - *directionX][nextY] == PUSHABLE_BLOC ||
        boardGame[nextX+ *directionX][nextY] == PUSHABLE_BLOC)
        *directionY = -*directionY;

        else 
        if (boardGame[nextX][nextY+*directionY] == PUSHABLE_BLOC||
        boardGame[nextX][nextY-*directionY] == PUSHABLE_BLOC)
        *directionX = -*directionX;

        else{
        *directionX = -*directionX;
        *directionY = -*directionY;}
}
else
    if (boardGame[nextX - *directionX][nextY] == PUSHABLE_BLOC &&
        boardGame[nextX][nextY - *directionY] == PUSHABLE_BLOC) {
        *directionX = -*directionX;
        *directionY = -*directionY;
    }
    

    // Update ball's position if no collision
    *ballX += *directionX;
    *ballY += *directionY;
}

void
updateBallPlacement(char boardGame[10][20], int *ballX, int *ballY, int *directionX, int *directionY,
                    enum LeveLResult *isLevelWon, Update updates[100000], int *numberUpdates) {
    boardGame[(*ballX)][(*ballY)] = EMPTY;
    addUpdate((*ballY), (*ballX), EMPTY, numberUpdates, updates);

    moveBallDiagonally(boardGame, ballX, ballY, directionX, directionY, isLevelWon);

    boardGame[(*ballX)][(*ballY)] = BALL;
    addUpdate((*ballY), (*ballX), BALL, numberUpdates, updates);
}

void addUpdate(int x, int y, char newValue, int *index, Update *updates) {
    updates[*index].newValue = newValue;
    updates[*index].x = x;
    updates[*index].y = y;

    (*index)++;
}

void updateElementsDisplay(char (*boardGame)[20], Update *updates, int numberUpdates) {
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

void moveSnoopy(char (*board)[20], int *snoopyX, int *snoopyY, char key, int *score, enum LeveLResult *isLevelWon,
                int *numberUpdates, Update *updates) {
    int number;
    const int SCORE_X = 2;
    const int SCORE_Y = -2;

    switch (key) {
        case UP_ARROW:
            if (*snoopyX > 0) {
                if (board[*snoopyX - 1][*snoopyY] == BALL) {
                    *isLevelWon = LOST;
                    return;
                }
                else

                if (board[*snoopyX - 1][*snoopyY] == BIRD) {
                    (*score)++;

                    number = *score;
                    char charRepresentation = '0' + number;
                    addUpdate(SCORE_X, SCORE_Y, charRepresentation, numberUpdates, updates);
                }
                //:::::::::::::::::::::::::::::::::::::::::::::
                
                if (board[*snoopyX - 1][*snoopyY] == PUSHABLE_BLOC && *snoopyX-1>0&&board[*snoopyX - 2][*snoopyY] != INVINCIBLE_BLOC
                &&board[*snoopyX - 2][*snoopyY] != BIRD&&board[*snoopyX - 2][*snoopyY] != PUSHABLE_BLOC) {

                    board[*snoopyX][*snoopyY] = EMPTY;
                    addUpdate(*snoopyY, *snoopyX, EMPTY, numberUpdates, updates);
                    board[*snoopyX-1][*snoopyY] = EMPTY;
                    addUpdate(*snoopyY, *snoopyX-1, EMPTY, numberUpdates, updates);
                    board[--(*snoopyX)][*snoopyY] = SNOOPY;
                    addUpdate(*snoopyY, *snoopyX, SNOOPY, numberUpdates, updates);
                    board[*snoopyX-1][*snoopyY] = PUSHABLE_BLOC;
                    addUpdate(*snoopyY, *snoopyX-1, PUSHABLE_BLOC, numberUpdates, updates);
                 
                }
                
                if (board[*snoopyX - 1][*snoopyY] != INVINCIBLE_BLOC&&board[*snoopyX - 1][*snoopyY] != PUSHABLE_BLOC) {
                    board[*snoopyX][*snoopyY] = EMPTY;
                    addUpdate(*snoopyY, *snoopyX, EMPTY, numberUpdates, updates);
                    board[--(*snoopyX)][*snoopyY] = SNOOPY;
                    addUpdate(*snoopyY, *snoopyX, SNOOPY, numberUpdates, updates);
                }
            }
            break;
        case DOWN_ARROW:
            if (*snoopyX < ROWS - 1) {
                if (board[*snoopyX + 1][*snoopyY] == BALL) {
                    *isLevelWon = LOST;
                    return;
                }

                if (board[*snoopyX + 1][*snoopyY] == BIRD) {
                    (*score)++;

                    number = *score;
                    char charRepresentation = '0' + number;
                    addUpdate(SCORE_X, SCORE_Y, charRepresentation, numberUpdates, updates);
                }
                if (board[*snoopyX + 1][*snoopyY] == PUSHABLE_BLOC && *snoopyX+1<ROWS -1&&board[*snoopyX + 2][*snoopyY] != INVINCIBLE_BLOC
                &&board[*snoopyX + 2][*snoopyY] != BIRD&&board[*snoopyX + 2][*snoopyY] != PUSHABLE_BLOC) {

                    board[*snoopyX][*snoopyY] = EMPTY;
                    addUpdate(*snoopyY, *snoopyX, EMPTY, numberUpdates, updates);
                    board[*snoopyX+1][*snoopyY] = EMPTY;
                    addUpdate(*snoopyY-1, *snoopyX, EMPTY, numberUpdates, updates);
                    board[++(*snoopyX)][*snoopyY] = SNOOPY;
                    addUpdate(*snoopyY, *snoopyX, SNOOPY, numberUpdates, updates);
                    board[*snoopyX+1][*snoopyY] = PUSHABLE_BLOC;
                    // moveCursor(*snoopyY,*snoopyX-1) ;printSymbol(PUSHABLE_BLOC);
                    addUpdate(*snoopyY, *snoopyX+1, PUSHABLE_BLOC, numberUpdates, updates);
                    // addUpdate(*snoopyY, *snoopyX, SNOOPY, numberUpdates, updates);
                    // addUpdate(*snoopyY, *snoopyX, PUSHABLE_BLOC, numberUpdates, updates);
                }

                if (board[*snoopyX + 1][*snoopyY] != INVINCIBLE_BLOC&&board[*snoopyX + 1][*snoopyY] != PUSHABLE_BLOC) {
                    addUpdate(*snoopyY, *snoopyX, EMPTY, numberUpdates, updates);
                    board[*snoopyX][*snoopyY] = EMPTY;
                    board[++(*snoopyX)][*snoopyY] = SNOOPY;
                    addUpdate(*snoopyY, *snoopyX, SNOOPY, numberUpdates, updates);
                }
            }
            break;
        case LEFT_ARROW:
            if (*snoopyY > 0) {
                if (board[*snoopyX][*snoopyY - 1] == BALL) {
                    *isLevelWon = LOST;
                    return;
                }

                if (board[*snoopyX][*snoopyY - 1] == BIRD) {
                    (*score)++;

                    number = *score;
                    char charRepresentation = '0' + number;
                    addUpdate(SCORE_X, SCORE_Y, charRepresentation, numberUpdates, updates);
                }
                if (board[*snoopyX][*snoopyY-1] == PUSHABLE_BLOC && *snoopyY-1>0 &&board[*snoopyX][*snoopyY-2] != INVINCIBLE_BLOC
                &&board[*snoopyX][*snoopyY-2] != BIRD&&board[*snoopyX][*snoopyY-2] != PUSHABLE_BLOC) {

                    board[*snoopyX][*snoopyY] = EMPTY;
                    addUpdate(*snoopyY, *snoopyX, EMPTY, numberUpdates, updates);
                    board[*snoopyX][*snoopyY-1] = EMPTY;
                    addUpdate(*snoopyY-1, *snoopyX, EMPTY, numberUpdates, updates);
                    board[(*snoopyX)][--(*snoopyY)] = SNOOPY;
                    addUpdate(*snoopyY, *snoopyX, SNOOPY, numberUpdates, updates);
                    board[*snoopyX][*snoopyY-1] = PUSHABLE_BLOC;
                    // moveCursor(*snoopyY,*snoopyX-1) ;printSymbol(PUSHABLE_BLOC);
                    addUpdate(*snoopyY-1, *snoopyX, PUSHABLE_BLOC, numberUpdates, updates);
                    // addUpdate(*snoopyY, *snoopyX, SNOOPY, numberUpdates, updates);
                    // addUpdate(*snoopyY, *snoopyX, PUSHABLE_BLOC, numberUpdates, updates);
                }

                if (board[*snoopyX][*snoopyY - 1] != INVINCIBLE_BLOC&&board[*snoopyX][*snoopyY-1] != PUSHABLE_BLOC) {
                    addUpdate(*snoopyY, *snoopyX, EMPTY, numberUpdates, updates);
                    board[*snoopyX][*snoopyY] = EMPTY;
                    board[*snoopyX][--(*snoopyY)] = SNOOPY;
                    addUpdate(*snoopyY, *snoopyX, SNOOPY, numberUpdates, updates);
                }
            }
            break;
        case RIGHT_ARROW:
            if (*snoopyY < COLS - 1) {
                if (board[*snoopyX][*snoopyY + 1] == BALL) {
                    *isLevelWon = LOST;
                    return;
                }

                if (board[*snoopyX][*snoopyY + 1] == BIRD) {
                    (*score)++;

                    number = *score;
                    char charRepresentation = '0' + number;

                    addUpdate(SCORE_X, SCORE_Y, charRepresentation, numberUpdates, updates);
                }
                if (board[*snoopyX][*snoopyY+1] == PUSHABLE_BLOC && *snoopyY+1<COLS-1 &&board[*snoopyX][*snoopyY+2] != INVINCIBLE_BLOC
                &&board[*snoopyX][*snoopyY+2] != BIRD&&board[*snoopyX][*snoopyY+2] != PUSHABLE_BLOC) {

                    board[*snoopyX][*snoopyY] = EMPTY;
                    addUpdate(*snoopyY, *snoopyX, EMPTY, numberUpdates, updates);
                    board[*snoopyX][*snoopyY+1] = EMPTY;
                    addUpdate(*snoopyY+1, *snoopyX, EMPTY, numberUpdates, updates);
                    board[(*snoopyX)][++(*snoopyY)] = SNOOPY;
                    addUpdate(*snoopyY, *snoopyX, SNOOPY, numberUpdates, updates);
                    board[*snoopyX][*snoopyY+1] = PUSHABLE_BLOC;
                    // moveCursor(*snoopyY,*snoopyX-1) ;printSymbol(PUSHABLE_BLOC);
                    addUpdate(*snoopyY+1, *snoopyX, PUSHABLE_BLOC, numberUpdates, updates);
                    // addUpdate(*snoopyY, *snoopyX, SNOOPY, numberUpdates, updates);
                    // addUpdate(*snoopyY, *snoopyX, PUSHABLE_BLOC, numberUpdates, updates);
                }

                if (board[*snoopyX][*snoopyY + 1] != INVINCIBLE_BLOC&&board[*snoopyX][*snoopyY + 1] != PUSHABLE_BLOC) {
                    addUpdate(*snoopyY, *snoopyX, EMPTY, numberUpdates, updates);
                    board[*snoopyX][*snoopyY] = EMPTY;
                    board[*snoopyX][++(*snoopyY)] = SNOOPY;
                    addUpdate(*snoopyY, *snoopyX, SNOOPY, numberUpdates, updates);
                }
            }
            break;
    }
}

void readGameBoardElementsFromFile(int level, char boardGame[ROWS][COLS], int *ballX, int *ballY, int *snoopyX,
                                   int *snoopyY) {
    char element;
    char filename[20]; // Assuming the file names are like "level1.txt", "level2.txt", etc.
    sprintf(filename, "../level/level%d.txt", level); // Generating filename based on level

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }

    // Read values from the file and update the boardGame matrix and positions
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fscanf(file, " %c", &element);
            switch (element) {
                case '0':
                    element = EMPTY;
                    break;
                case '2':
                    element = PUSHABLE_BLOC;
                    break;
                case '9':
                    element = BIRD;
                    break;
                case '4':
                    element = INVINCIBLE_BLOC;
                    break;
                case '7':
                    element = SNOOPY;
                    *snoopyX = i;
                    *snoopyY = j;
                    break;
                case '8':
                    element = BALL;
                    *ballX = i;
                    *ballY = j;
                    break;
                default:
                    break;
            }
            boardGame[i][j] = element;
        }
    }
    fclose(file);
}

int isLevelAvailable(int level) {
    char filename[20];
    sprintf(filename, "../level/level%d.txt", level);

    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    } else {
        return 0;
    }
}

void displayAvailableLevels() {
    printf("Available Levels: ");
    for (int i = 1; i <= AVAILABLE_LEVELS; ++i) {
        if (isLevelAvailable(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

void loadPasswordsFile(char passwords[MAX_NUM_PASSWORDS][MAX_PASSWORD_LENGTH]) {
    int numPasswords = 0;

    const char *file_path = "../level/passwords.txt";

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    while (numPasswords < MAX_NUM_PASSWORDS && fscanf(file, "%5s", passwords[numPasswords]) == 1) {
        numPasswords++;
    }

    fclose(file);
}

int validatePassword(int level, const char *enteredPassword) {
    char passwords[MAX_NUM_PASSWORDS][MAX_PASSWORD_LENGTH];
    loadPasswordsFile(passwords);

    return strcmp(enteredPassword, passwords[level - 1]) == 0;
}
