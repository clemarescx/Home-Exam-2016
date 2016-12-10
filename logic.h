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

Neighbours findNeighbours(Position pos);

bool isValidMove(Player player, Position pos);


#endif //__LOGIC__H
