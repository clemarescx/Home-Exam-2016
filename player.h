#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "board.h"
#include "debug.h"

#define PLAYERNAME_SIZE 20

// struct to store player information
typedef struct Player {
    char *name;
    Field token;

    int score;
    Position log[64];   //bit of a wasted space, but since sizeof(Position) is not tremendous...
    int roundsPlayed;

} Player;

#endif

int playerInit(Player *player);