//
// Created by clement on 16/03/17.
//

#include "board.h"
#include "player.h"
#include "logic.h"

#ifndef HOME_EXAM_2016_AI_LOGIC_H
#define HOME_EXAM_2016_AI_LOGIC_H

Position findBestMove(Player, Player, Position *validMoves, int validMovesCount);
/**
 * - get all valid moves for Player
 * - for each move
 * -    for 100 times
 * -        simulate entire game randomly with this move
 * -            ...(describe how)
 * -        if Player wins, increase move weight
 * - return move with max weight
 *
 */



#endif //HOME_EXAM_2016_AI_LOGIC_H
