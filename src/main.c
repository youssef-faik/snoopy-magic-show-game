#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <unistd.h>

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

// typedef struct Balle
// {
//     int x;
//     int y;
//     int directionX;
//     int directionY;
// } t_balle;

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
void moveObject(int *, int, int, int, int, char[ROWS][COLS], char);

// main program
int main()
{
    startGame();
    return 0;
}

// main functions implementations
void startGame()
{
    int selectedOption;

    do
    {
        displayWelcomeBanner();
        selectedOption = displayMenuOption();

        switch (selectedOption)
        {
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

void displayGameRules()
{
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

void moveBallDiagonally(int *ballX, int *ballY, int *directionX, int *directionY, char boardGame[ROWS][COLS])
{
    // Update ball's position based on direction
    int nextX = *ballX + *directionX;
    int nextY = *ballY + *directionY;

    // Collision checks
    if (nextX < 0 || nextX >= ROWS)
    {
        *directionX = -*directionX;
    }
    if (nextY < 0 || nextY >= COLS)
    {
        *directionY = -*directionY;
    }
    if (boardGame[nextX][nextY] == BIRD || boardGame[nextX][nextY] == SNOOPY)
    {
        *directionX = -*directionX;
        *directionY = -*directionY;
    }

    // Update ball's position if no collision
    *ballX += *directionX;
    *ballY += *directionY;
}

void startNewGame()
{
    clearScreen();
    printf("Start New game\n\n");

    int score = 0;
    int snoopyX = 4, snoopyY = 9;
    int ballX = 7, ballY = 17;

    //? Initialize the entire matrix with zeros
    char boardGame[ROWS][COLS];
    int i, j;
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            boardGame[i][j] = EMPTY;
        }
    }

    //? snoopy
    boardGame[snoopyX][snoopyY] = SNOOPY;

    //? ball
    boardGame[ballX][ballY] = BALL;

    //? birds
    boardGame[0][0] = BIRD;
    boardGame[0][COLS - 1] = BIRD;
    boardGame[ROWS - 1][0] = BIRD;
    boardGame[ROWS - 1][COLS - 1] = BIRD;

    //? obstacles
    boardGame[4][4] = INVINCIBLE_BLOC;
    boardGame[4][5] = INVINCIBLE_BLOC;
    boardGame[5][6] = INVINCIBLE_BLOC;
    boardGame[6][7] = INVINCIBLE_BLOC;
    boardGame[0][1] = INVINCIBLE_BLOC;
    boardGame[0][COLS - 2] = INVINCIBLE_BLOC;
    boardGame[1][COLS - 2] = INVINCIBLE_BLOC;

    // Print the matrix in the specified format
    printGameBoard(boardGame);

    // Time-based ball movement variables
    int ballTimer = 0;
    int ballMoveInterval = 1; // Move every second
    int directionX = 1, directionY = 1;

    // Get user input for direction
    char key;
    do
    {
        printf("\nPlayer Score: %d", score);
        printf("\nEnter direction (UP/DOWN/LEFT/RIGHT/q): ");
        // key = waitForKeyHit();
        fflush(stdin);

        // Adjust the position based on user input
        switch (key)
        {
        case UP_ARROW:
            if (snoopyX > 0)
            {
                if (boardGame[snoopyX - 1][snoopyY] == BIRD)
                {
                    score++;
                }

                if (boardGame[snoopyX - 1][snoopyY] != INVINCIBLE_BLOC)
                {
                    boardGame[snoopyX][snoopyY] = EMPTY;
                    boardGame[--snoopyX][snoopyY] = SNOOPY;
                }
            }
            break;
        case DOWN_ARROW:
            if (snoopyX < ROWS - 1)
            {
                if (boardGame[snoopyX + 1][snoopyY] == BIRD)
                {
                    score++;
                }

                if (boardGame[snoopyX + 1][snoopyY] != INVINCIBLE_BLOC)
                {
                    boardGame[snoopyX][snoopyY] = EMPTY;
                    boardGame[++snoopyX][snoopyY] = SNOOPY;
                }
            }
            break;
        case LEFT_ARROW:
            if (snoopyY > 0)
            {
                if (boardGame[snoopyX][snoopyY - 1] == BIRD)
                {
                    score++;
                }

                if (boardGame[snoopyX][snoopyY - 1] != INVINCIBLE_BLOC)
                {
                    boardGame[snoopyX][snoopyY] = EMPTY;
                    boardGame[snoopyX][--snoopyY] = SNOOPY;
                }
            }
            break;
        case RIGHT_ARROW:
            if (snoopyY < COLS - 1)
            {
                if (boardGame[snoopyX][snoopyY + 1] == BIRD)
                {
                    score++;
                }

                if (boardGame[snoopyX][snoopyY + 1] != INVINCIBLE_BLOC)
                {
                    boardGame[snoopyX][snoopyY] = EMPTY;
                    boardGame[snoopyX][++snoopyY] = SNOOPY;
                }
            }
            break;
        default:
            printf("Invalid key. use the arrow keys to place the Snoopy character\n");
        }

        if (ballTimer % ballMoveInterval == 0)
        {
            // Update ball's position diagonally
            boardGame[ballX][ballY] = EMPTY;
            moveBallDiagonally(&ballX, &ballY, &directionX, &directionY, boardGame);
            boardGame[ballX][ballY] = BALL;
            clearScreen();
            printGameBoard(boardGame);
        }

        ballTimer++; // Increment timer
        sleep(1);
    } while (key != 'q' && score < 4);

    if (score == 4)
    {
        printf("Congrats! You Won this level !\n\n");
    }

    printf("Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void displayGameControls()
{
    clearScreen();
    printf("Controls\n\n");
    printf("Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void displayRecordedScores()
{
    clearScreen();
    printf("Recorded Scores\n\n");
    printf("Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void displayLevels()
{
    clearScreen();
    printf("Levels\n\n");
    printf("Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void loadSavedGame()
{
    clearScreen();
    printf("Load Saved Game\n\n");
    printf("Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void quitGame()
{
    clearScreen();
    printf("Press Any Key To Go Back to The close the game...");
    waitForKeyHit();
}

// helper functions implementations
void displayWelcomeBanner()
{
    clearScreen();
    printf("   _____                                   \n");
    printf("  / ___/ ____   ____   ____   ____   __  __\n");
    printf("  \\__ \\ / __ \\ / __ \\ / __ \\ / __ \\ / / / /\n");
    printf(" ___/ // / / // /_/ // /_/ // /_/ // /_/ / \n");
    printf("/____//_/ /_/ \\____/ \\____// .___/ \\__, /  \n");
    printf("                          /_/     /____/   \n");
    printf("----------------------------------------------\n");
    printf("Welcome To The Snoopy Game\n");
}

void clearScreen()
{
    system("cls");
}

int displayMenuOption()
{
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

char waitForKeyHit()
{
    int pressed;
    while (!kbhit())
        ;
    pressed = getch();
    // pressed = tolower(pressed);
    return ((char)pressed);
}

int menuSelector(int x, int y, int yStart)
{
    char key;
    int selectedOptionIndex = 0;
    x = x - 2;
    moveCursor(x, yStart);
    printf(">");
    moveCursor(1, 1);

    do
    {
        key = waitForKeyHit();
        // printf("%c %d", key, (int)key);
        if (key == (char)UP_ARROW)
        {
            moveCursor(x, yStart + selectedOptionIndex);
            printf(" ");

            if (yStart >= yStart + selectedOptionIndex)
                selectedOptionIndex = y - yStart - 2;
            else
                selectedOptionIndex--;
            moveCursor(x, yStart + selectedOptionIndex);
            printf(">");
        }
        else if (key == (char)DOWN_ARROW)
        {

            moveCursor(x, yStart + selectedOptionIndex);
            printf(" ");

            if (selectedOptionIndex + 2 >= y - yStart)
                selectedOptionIndex = 0;
            else
                selectedOptionIndex++;
            moveCursor(x, yStart + selectedOptionIndex);
            printf(">");
        }
        // moveCursor(1,1);
        //        printf("%d", selectedOptionIndex);
    } while (key != (char)ENTER_KEY);
    return (selectedOptionIndex);
}

void moveCursor(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

void printGameBoard(char boardGame[ROWS][COLS])
{
    int i, j;
    // Print the top border
    printf("\n");
    printf("+-----------------------------------------+\n");
    // Print the matrix
    for (i = 0; i < ROWS; i++)
    {
        printf("| ");
        for (j = 0; j < COLS; j++)
        {
            printf("%c ", boardGame[i][j]);
        }
        printf("|\n");
    }
    printf("+-----------------------------------------+\n");
}

void moveObject(int *score, int rows, int cols, int row_position, int col_position, char matrix[10][20], char direction)
{
    switch (direction)
    {
    case UP_ARROW:
        if (row_position > 0)
        {
            if (matrix[row_position - 1][col_position] == BIRD)
            {
                (*score)++;
            }

            if (matrix[row_position - 1][col_position] != INVINCIBLE_BLOC)
            {
                matrix[row_position][col_position] = EMPTY;
                matrix[--row_position][col_position] = SNOOPY;
            }
        }
        break;
    case DOWN_ARROW:
        if (row_position < rows - 1)
        {
            if (matrix[row_position + 1][col_position] == BIRD)
            {
                (*score)++;
            }

            if (matrix[row_position + 1][col_position] != INVINCIBLE_BLOC)
            {
                matrix[row_position][col_position] = EMPTY;
                matrix[++row_position][col_position] = SNOOPY;
            }
        }
        break;
    case LEFT_ARROW:
        if (col_position > 0)
        {
            if (matrix[row_position][col_position - 1] == BIRD)
            {
                (*score)++;
            }

            if (matrix[row_position][col_position - 1] != INVINCIBLE_BLOC)
            {
                matrix[row_position][col_position] = EMPTY;
                matrix[row_position][--col_position] = SNOOPY;
            }
        }
        break;
    case RIGHT_ARROW:
        if (col_position < cols - 1)
        {
            if (matrix[row_position][col_position + 1] == BIRD)
            {
                (*score)++;
            }

            if (matrix[row_position][col_position + 1] != INVINCIBLE_BLOC)
            {
                matrix[row_position][col_position] = EMPTY;
                matrix[row_position][++col_position] = SNOOPY;
            }
        }
        break;
    default:
        printf("Invalid direction. use the arrow keys to place the Snoopy character\n");
    }
}
