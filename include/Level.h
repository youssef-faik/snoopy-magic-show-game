//
// Created by youss on 12/16/2023.
//

#ifndef SNOOPY_GAME_LEVEL_H
#define SNOOPY_GAME_LEVEL_H

#define MAX_ROWS 10
#define MAX_COLS 10

typedef struct 
{
    int levelNumber;
    char password[4];
    char board[MAX_ROWS][MAX_COLS];
} Level;

#endif //SNOOPY_GAME_LEVEL_H
