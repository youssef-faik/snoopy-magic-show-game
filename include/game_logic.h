//
// Created by Youssef Faik on 1/2/2024.
//

#ifndef SNOOPY_GAME_GAME_LOGIC_H
#define SNOOPY_GAME_GAME_LOGIC_H

// Headers
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "game_structs.h"

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

// Constants and Macros
// Game
#define ROWS 10
#define COLS 20

#define AVAILABLE_LEVELS 3
#define MAX_NUM_PASSWORDS AVAILABLE_LEVELS
#define MAX_PASSWORD_LENGTH 6

// Enums
enum LeveLResult {
    WON, LOST, QUIT
};


// Function prototypes
//      Main functions declarations
void runGameApp();

void displayGameRules();

void startGame(int level, int *globalScore, int *highestScore);

void loadSavedGame();

void displayLevels(int *globalScore, int *highestScore);

void displayRecordedScores(int highestScore);

void displayGameControls();

void quitGame();

//      Helper functions declarations
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

void displayLevelResult(enum LeveLResult isLevelWon, int level, int *score);

void addUpdate(int x, int y, char newValue, int *index, Update updates[]);

void updateElementsDisplay(char boardGame[ROWS][COLS], Update updates[], int numberUpdates);

void
moveSnoopy(char board[10][20], int *snoopyX, int *snoopyY, char key, int *score, enum LeveLResult *isLevelWon,
           int *numberUpdates, Update updates[]);

void
updateBallPlacement(char boardGame[10][20], int *ballX, int *ballY, int *directionX, int *directionY,
                    enum LeveLResult *isLevelWon, Update updates[100000], int *numberUpdates);

enum LeveLResult playLevel(int level, int *globalScore, int *highestScore, int remainingLives);

void readGameBoardElementsFromFile(int level, char boardGame[ROWS][COLS], int *ballX, int *ballY, int *snoopyX,
                                   int *snoopyY);

int isLevelAvailable(int level);

int validatePassword(int level, const char *enteredPassword);

void displayAvailableLevels();

void loadPasswordsFile(char passwords[MAX_NUM_PASSWORDS][MAX_PASSWORD_LENGTH]);

void updateScore(int score);

int readHighestScore();

void saveNewHighestScore(int score);


#endif //SNOOPY_GAME_GAME_LOGIC_H
