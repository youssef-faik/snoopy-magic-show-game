//
// Created by youss on 12/16/2023.
//

#ifndef SNOOPY_GAME_GAMESAVELOAD_H
#define SNOOPY_GAME_GAMESAVELOAD_H

#include "GameData.h"

// Save the current game state to a specified file
void saveGame(const char* filename, const GameData* gameData);

// Load a saved game state from a specified file
void loadGame(const char* filename, GameData* gameData);

#endif //SNOOPY_GAME_GAMESAVELOAD_H
