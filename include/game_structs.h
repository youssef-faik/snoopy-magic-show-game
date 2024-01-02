//
// Created by Youssef Faik on 1/2/2024.
//

#ifndef SNOOPY_GAME_GAME_STRUCTS_H
#define SNOOPY_GAME_GAME_STRUCTS_H

typedef struct {
    int x;
    int y;
    char newValue;
} Update;

typedef struct {
    int x;
    int y;
} Snoopy;

typedef struct {
    int x;
    int y;
    int directionX;
    int directionY;
} Ball;


#endif //SNOOPY_GAME_GAME_STRUCTS_H
