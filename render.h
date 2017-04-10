//
// Created by clement on 13/12/16.
//

#include "board.h"

#ifndef HOME_EXAM_2016_RENDER_H
#define HOME_EXAM_2016_RENDER_H

#endif //HOME_EXAM_2016_RENDER_H

#ifndef NCURSES
#define NCURSES
#endif

int initRender();

void initBoardWin();

void initLogWin();

void initInputWin();

void _printBoard(const Board *board);

void refreshLog();

void terminateRender();