//
// Created by clement on 13/12/16.
//

#ifndef HOME_EXAM_2016_RECORDS_H
#define HOME_EXAM_2016_RECORDS_H

#include "board.h"
#include "player.h"

typedef struct Move {
    Position move;
    Player player;
} Move;

typedef struct Records {
    Move gameLog[64];
    int roundCount;
} Records;


#endif //HOME_EXAM_2016_RECORDS_H

void initRecords();

void addMoveToLog(Player player, Position move);

void printLogToFile();

void printGameLog();
