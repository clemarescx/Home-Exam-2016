#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "board.h"
#include "debug.h"

#define PLAYERNAME_SIZE 15


typedef enum PlayerType {
    HUMAN,
    COMPUTER
} PlayerType;


// struct to store player information
typedef struct Player {
    char *name;
    Field token;
    int score;
    PlayerType type;
} Player;

#endif

int initialisePlayer(Player *player, PlayerType type);