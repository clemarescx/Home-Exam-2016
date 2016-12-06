#ifndef __PLAYER_H__
#define __PLAYER_H__

// struct to store a position on the board

typedef struct Position {
    short signed x;
    short signed y;
} Position;

// struct to store player information
typedef struct Player {

    char *name;
    enum Field;
    short signed score;
    Position log[64];

} Player;

#endif