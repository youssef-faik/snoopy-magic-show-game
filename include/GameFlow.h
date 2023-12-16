//
// Created by youss on 12/16/2023.
//

#ifndef SNOOPY_GAME_GAMEFLOW_H
#define SNOOPY_GAME_GAMEFLOW_H

#include "GameData.h"
#include "Direction.h"

// Start a countdown timer for a specified duration in seconds
void startTimer(int seconds);

// Stop the timer
void stopTimer();

// Pause the game
void pauseGame();

// Resume the game
void resumeGame();

// Check if the game is over
int isGameOver(const GameData* gameData);

// Check if the current level objectives are met
int isLevelComplete(const GameData* gameData);

// Calculate the player's score based on game performance
int calculateScore(const GameData* gameData);

// Update the game state based on user input and other factors
void updateGame(GameData* gameData);

// Move Snoopy in the specified direction
void moveSnoopy(Direction direction, GameData* gameData);

// Handle collisions with enemy objects
void handleCollisionWithEnemy(GameData* gameData);

// Handle collisions with obstacles
void handleCollisionWithObstacle(GameData* gameData);

// Generate a password for a specific level
char* generateLevelPassword(int level);

#endif //SNOOPY_GAME_GAMEFLOW_H
