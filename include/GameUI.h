//
// Created by youss on 12/15/2023.
//


#include<conio.h>
#ifndef SNOOPY_GAME_GAMEUI_H
#define SNOOPY_GAME_GAMEUI_H

#define UP_ARROW 72

	#define LEFT_ARROW 75
	#define RIGHT_ARROW 77
	#define DOWN_ARROW 80
	
	#define ENTER_KEY 13

#include "GameLogic.h"

// return the option in the menu 
int menuSelector(int x, int y, int yStart);

// representing the horizontal and vertical coordinates move the cursur
void gotoxy(int x,int y);

// clear screan 
void clrscr();

//first page 
void welcomeArt(void);

// Display the main menu options to the user
int displayMenu();

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
