//
// Created by youss on 12/16/2023.
//

#ifndef SNOOPY_GAME_GAME_INITIALIZATION_H
#define SNOOPY_GAME_GAME_INITIALIZATION_H

#include "GameData.h"

// Initialize the overall game state, e.g. load levels representation from the corresponding files
void initializeGame(GameData* gameData);

// Initialize the state for a specific game level
void initializeLevel(GameData* gameData);

// Launch a specific game level
void launchLevel(GameData* gameData);

#endif //SNOOPY_GAME_GAME_INITIALIZATION_H
