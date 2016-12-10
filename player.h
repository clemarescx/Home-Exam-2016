#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "board.h"
#include "debug.h"


// struct to store player information
typedef struct Player {
    char *name;
    Field token;

    short signed score;
    Position log[64];

} Player;

#endif

/**
 * Initialise the players
 * @param player
 * @return
 */
int playerInit(Player *player);