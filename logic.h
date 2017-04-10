//
// Created by clement on 07/12/16.
//

#ifndef __LOGIC__H
#define __LOGIC__H

#include "board.h"
#include "debug.h"
//#include "player.h"

typedef struct PositionList {
    Position list[8];
    short count;
} PositionList;


//int isValidMove(Player player, Position pos);
PositionList getFlippableTokens(Position *inputPos, Player *player);

int flipDirection(Position *currentPosition, Position *direction, Player *player);

int getValidMoves(Player *player, Position *possibleMoves);

#endif //__LOGIC__H
