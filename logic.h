//
// Created by clement on 07/12/16.
//

#ifndef __LOGIC__H
#define __LOGIC__H

#include "board.h"
#include "debug.h"

typedef struct Neighbours {
    Position list[8];
    short count;
} Neighbours;


//int isValidMove(Player player, Position pos);
Neighbours findValidNeighbours(Position pos, Player player);

int flipDirection(Position cPos, Position direction, Player player);

int getValidMoves(Player player, Position *positions);



#endif //__LOGIC__H
