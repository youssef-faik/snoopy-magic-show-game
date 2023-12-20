#include <conio.h>
#include <stdio.h>
#include<windows.h>
#include <time.h>

#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define ENTER_KEY 13

#define EMPTY 0
#define SNOOPY 7
#define BIRD 9
#define BALL 8
#define INVINCIBLE_BLOC 4

typedef struct Balle {
    int x;
    int y;
    int directionX;
    int directionY;
} t_balle;

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
void displayWelcomeBanner(void);

void clearScreen();

char waitForKeyHit(void);

int displayMenuOption();

int menuSelector(int x, int y, int yStart);

void moveCursor(int x, int y);

void printGameBoard(int rows, int cols, int matrix[rows][cols]);

void moveObject(int *score, int rows, int cols, int row_position, int col_position, int matrix[10][20], char direction);

char *getDisplaySymbol(int number);

void setup();

// main program
int main() {
    startGame();
    return 1;
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

void startNewGame() {
    clearScreen();
    printf("Start New game\n\n");

    int score = 0;
    int rows = 10;
    int cols = 20;
    int row_position = 2;
    int col_position = 2;
    t_balle balle = {1, 1, 1, 1};

    // Initialize the entire matrix with zeros
    int matrix[10][20] = {0};

    // snoopy
    matrix[row_position][col_position] = SNOOPY;

    // birds
    matrix[0][0] = BIRD;
    matrix[0][cols - 1] = BIRD;
    matrix[rows - 1][0] = BIRD;
    matrix[rows - 1][cols - 1] = BIRD;

    // obstacles
    matrix[2][4] = INVINCIBLE_BLOC;
    matrix[3][4] = INVINCIBLE_BLOC;
    matrix[4][4] = INVINCIBLE_BLOC;
    matrix[5][4] = INVINCIBLE_BLOC;
    matrix[6][4] = INVINCIBLE_BLOC;
    matrix[0][cols - 2] = INVINCIBLE_BLOC;
    matrix[1][cols - 2] = INVINCIBLE_BLOC;

    // Print the matrix in the specified format
    printGameBoard(rows, cols, matrix);

    // Get user input for direction
    char direction;
    do {
        printf("\nPlayer Score: %d", score);
        printf("\nEnter direction (UP/DOWN/LEFT/RIGHT/q): ");
        direction = waitForKeyHit();
        fflush(stdin);

        // Adjust the position based on user input
        switch (direction) {
            case UP_ARROW:
                if (row_position > 0) {
                    if (matrix[row_position - 1][col_position] == BIRD) {
                        score++;
                    }

                    if (matrix[row_position - 1][col_position] != INVINCIBLE_BLOC) {
                        matrix[row_position][col_position] = EMPTY;
                        matrix[--row_position][col_position] = SNOOPY;
                    }
                }
                break;
            case DOWN_ARROW:
                if (row_position < rows - 1) {
                    if (matrix[row_position + 1][col_position] == BIRD) {
                        score++;
                    }

                    if (matrix[row_position + 1][col_position] != INVINCIBLE_BLOC) {
                        matrix[row_position][col_position] = EMPTY;
                        matrix[++row_position][col_position] = SNOOPY;
                    }
                }
                break;
            case LEFT_ARROW:
                if (col_position > 0) {
                    if (matrix[row_position][col_position - 1] == BIRD) {
                        score++;
                    }

                    if (matrix[row_position][col_position - 1] != INVINCIBLE_BLOC) {
                        matrix[row_position][col_position] = EMPTY;
                        matrix[row_position][--col_position] = SNOOPY;
                    }
                }
                break;
            case RIGHT_ARROW:
                if (col_position < cols - 1) {
                    if (matrix[row_position][col_position + 1] == BIRD) {
                        score++;
                    }

                    if (matrix[row_position][col_position + 1] != INVINCIBLE_BLOC) {
                        matrix[row_position][col_position] = EMPTY;
                        matrix[row_position][++col_position] = SNOOPY;
                    }
                }
                break;
            default:
                printf("Invalid direction. use the arrow keys to place the Snoopy character\n");
        }

        clearScreen();
        printGameBoard(rows, cols, matrix);
    } while (direction != 'q' && score < 4);

    if (score == 4) {
        printf("Congrats! You Won this level !\n\n");
    }

    printf("Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
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
void displayWelcomeBanner(void) {
    clearScreen();
    printf("\n");
    printf("----------------------------------------------\n");
    printf("     _____ _____ _____ _____ ____ __   __    \n");
    printf("     |  ___|   | |  _  |  _  | __ |\\   //    \n");
    printf("     |___  | | | | |_| | |_| |  __| \\ //     \n");
    printf("     |_____|_|___|_____|_____|_|-----|_|------\n");
    printf("----------------------------------------------\n");
    printf("Welcome To The Snoopy Game\n");
}

void clearScreen() {
    system("cls");
}

int displayMenuOption() {
    int x = 10, y = 10;
    int yStart = y;
    int selected;
//    clearScreen();
    moveCursor(x, y++);
    printf("1. Game rules\n");
    moveCursor(x, y++);
    printf("2. Start game \n");
    moveCursor(x, y++);
    printf("3. Load Saved Game\n");
    moveCursor(x, y++);
    printf("4. Levels \n");
    moveCursor(x, y++);
    printf("5. Recorded Scores  \n");
    moveCursor(x, y++);
    printf("6. Controles \n");
    moveCursor(x, y++);
    printf("7. Exit\n");
    moveCursor(x, y++);
    selected = menuSelector(x, y, yStart);
    // printf("%d",selected); testing
    return (selected);
}

char waitForKeyHit(void) {
    int pressed;
    while (!kbhit());
    pressed = getch();
    //pressed = tolower(pressed);
    return ((char) pressed);
}

int menuSelector(int x, int y, int yStart) {
    char key;
    int selectedOptionIndex = 0;
    x = x - 2;
    moveCursor(x, yStart);
    printf(">");
    moveCursor(1, 1);

    do {
        key = waitForKeyHit();
        //printf("%c %d", key, (int)key);
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
        //moveCursor(1,1);
//        printf("%d", selectedOptionIndex);
    } while (key != (char) ENTER_KEY);
    return (selectedOptionIndex);
}

void moveCursor(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

void printGameBoard(int rows, int cols, int matrix[rows][cols]) {
    // Print the top border
    printf("+--------------------+\n");

    // Print the matrix
    for (int i = 0; i < rows; ++i) {
        printf("|");
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] == 0) {
                printf("%c", ' ');
            } else {
                printf("%d", matrix[i][j]);
            }
        }
        printf("|\n");
    }

    printf("+--------------------+\n");
}

void
moveObject(int *score, int rows, int cols, int row_position, int col_position, int matrix[10][20], char direction) {
    switch (direction) {
        case UP_ARROW:
            if (row_position > 0) {
                if (matrix[row_position - 1][col_position] == BIRD) {
                    (*score)++;
                }

                if (matrix[row_position - 1][col_position] != INVINCIBLE_BLOC) {
                    matrix[row_position][col_position] = EMPTY;
                    matrix[--row_position][col_position] = SNOOPY;
                }
            }
            break;
        case DOWN_ARROW:
            if (row_position < rows - 1) {
                if (matrix[row_position + 1][col_position] == BIRD) {
                    (*score)++;
                }

                if (matrix[row_position + 1][col_position] != INVINCIBLE_BLOC) {
                    matrix[row_position][col_position] = EMPTY;
                    matrix[++row_position][col_position] = SNOOPY;
                }
            }
            break;
        case LEFT_ARROW:
            if (col_position > 0) {
                if (matrix[row_position][col_position - 1] == BIRD) {
                    (*score)++;
                }

                if (matrix[row_position][col_position - 1] != INVINCIBLE_BLOC) {
                    matrix[row_position][col_position] = EMPTY;
                    matrix[row_position][--col_position] = SNOOPY;
                }
            }
            break;
        case RIGHT_ARROW:
            if (col_position < cols - 1) {
                if (matrix[row_position][col_position + 1] == BIRD) {
                    (*score)++;
                }

                if (matrix[row_position][col_position + 1] != INVINCIBLE_BLOC) {
                    matrix[row_position][col_position] = EMPTY;
                    matrix[row_position][++col_position] = SNOOPY;
                }

            }
            break;
        default:
            printf("Invalid direction. use the arrow keys to place the Snoopy character\n");
    }
}

