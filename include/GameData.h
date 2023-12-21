//
// Created by youss on 12/16/2023.
//

#ifndef SNOOPY_GAME_GAMEDATA_H
#define SNOOPY_GAME_GAMEDATA_H

#include "Player.h"
#include "Level.h"

typedef struct {
    Player player;
    Level currentLevel;
    char board[MAX_ROWS][MAX_COLS];
} GameData;

#endif //SNOOPY_GAME_GAMEDATA_H
