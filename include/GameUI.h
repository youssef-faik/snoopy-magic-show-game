//
// Created by youss on 12/15/2023.
//

#ifndef SNOOPY_GAME_GAMEUI_H
#define SNOOPY_GAME_GAMEUI_H

#include "GameLogic.h"

// Display the main menu options to the user
void displayMenu();

// Retrieve the user's
int retrieveUserInput();

// Validate the user's choice against the available options
int validateUserChoice(int choice);

// Display the game rules to the user
void displayGameRules();

// Start a new game from the first level
void startGame(GameData* gameData);

// Load a saved game
void loadSavedGame(GameData* gameData);

// Play a level using a password
void playLevelByPassword(GameData* gameData);

// Display the score of the user
void displayScores(GameData* gameData);

// Quit the game
void quitGame();

#endif //SNOOPY_GAME_GAMEUI_H
