//
// Created by clement on 13/12/16.
//

#include <ncurses.h>
#include <stdlib.h>
#include "render.h"

extern Board gameBoard;
const int height = 20,
        width = 50,
        starty = 0,
        startx = 0;

WINDOW *boardWin, *logWin, *inputWin;

typedef enum edgeType {
    TOP, MIDDLE, BOTTOM
} edgeType;


/**
 * The functions create_newwin, destoy_win and initRender were
 * all taken from the NCURSES tutorial:
 * http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html
 * They have been adapted to fit the project
 */
WINDOW *create_newwin(int height, int width, int starty, int startx, char *title);

void destroy_win(WINDOW *local_win);

void initBoardWin() {
    boardWin = create_newwin(LINES, width, starty, startx, "Board");
    clearok(boardWin, true);
}

void initLogWin() {
    logWin = create_newwin(height, COLS - width, starty, width, "Console Log");
    logWin->_begx++;
    logWin->_begy++;
    logWin->_maxx--;
    logWin->_maxy--;
    wrefresh(logWin);
}

void initInputWin() {
    inputWin = create_newwin(LINES - height, COLS - width, height, width, "Input");
    mvwprintw(inputWin, 1, 1, "> ");
    wrefresh(inputWin);
}

int initRender() {
    initBoardWin();
    initLogWin();
    initInputWin();
    return 0;
}

void setTitle(char *title, WINDOW *local_win) {
    wattron(local_win, A_BOLD);
    mvwprintw(local_win, 0, 1, title);
    wattroff(local_win, A_BOLD);
}

//Create an ncurses window inside the terminal
WINDOW *create_newwin(int height, int width, int starty, int startx, char *title) {

    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);

    box(local_win, 0, 0);        // draw the window frame
    setTitle(title, local_win);
    wrefresh(local_win);

    return local_win;
}


// Delete ( free() ) a window pointer
void destroy_win(WINDOW *local_win) {
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(local_win);
    delwin(local_win);
}


void drawBoardEdge(edgeType type);

/**
 * This is the same code from the original board.c class
 * it has been adapted to work in an ncurses temrinal
 * @param board
 */
void _printBoard(const Board *board) {

    int starty = 3, startx = 7;

    mvwprintw(boardWin, starty, startx,
              "  a   b   c   d   e   f   g   h  ");
    wmove(boardWin, ++starty, startx);
    drawBoardEdge(TOP);

    for (int y = 0; y < BOARD_SIZE; y++) {
        mvwprintw(boardWin, ++starty, startx - 2, " %d", y + 1);
        for (int x = 0; x < BOARD_SIZE; x++) {
            waddch(boardWin, ACS_VLINE);
            wprintw(boardWin, " ");

            switch (board->fields[x][y]) {
                case BLACK:
                    waddch(boardWin, ACS_DIAMOND);
                    break;
                case WHITE:
                    waddch(boardWin, ACS_BULLET);
                    break;
                default:
                    wprintw(boardWin, " ");
            }
            wprintw(boardWin, " ");
        }
        waddch(boardWin, ACS_VLINE);
        wprintw(boardWin, " %d", y + 1);
        wmove(boardWin, ++starty, startx);
        (y < BOARD_SIZE - 1 ? drawBoardEdge(MIDDLE) : drawBoardEdge(BOTTOM));
    }
    mvwprintw(boardWin, ++starty, startx, "  a   b   c   d   e   f   g   h  ");
    wrefresh(boardWin);

}

void drawBoardEdge(edgeType type) {
    chtype leftEdge, middleJoint, rightEdge;
    switch (type) {
        case TOP:
            leftEdge = ACS_ULCORNER;
            middleJoint = ACS_TTEE;
            rightEdge = ACS_URCORNER;
            break;
        case MIDDLE:
            leftEdge = ACS_LTEE;
            middleJoint = ACS_PLUS;
            rightEdge = ACS_RTEE;
            break;
        case BOTTOM:
            leftEdge = ACS_LLCORNER;
            middleJoint = ACS_BTEE;
            rightEdge = ACS_LRCORNER;
            break;
        default:
            leftEdge = ACS_PLUS;
            middleJoint = ACS_PLUS;
            rightEdge = ACS_PLUS;
    }
    waddch(boardWin, leftEdge);
    for (int i = 0; i < BOARD_SIZE - 1; i++) {
        for (int j = 0; j < 3; ++j) {
            waddch(boardWin, 113 | A_ALTCHARSET);

        }
        waddch(boardWin, middleJoint);
    }
    waddch(boardWin, 113 | A_ALTCHARSET);
    waddch(boardWin, 113 | A_ALTCHARSET);
    waddch(boardWin, 113 | A_ALTCHARSET);

    waddch(boardWin, rightEdge);
}

/**
 * Ugly hack to get some form of padding within the painted
 * box.
 */
void refreshLog() {
    werase(logWin);
    logWin->_begx--;
    logWin->_begy--;
    logWin->_maxx++;
    logWin->_maxy++;
    box(logWin, 0, 0);
    setTitle("Console Log", logWin);
    wrefresh(logWin);

    logWin->_begx++;
    logWin->_begy++;
    logWin->_maxx--;
    logWin->_maxy--;
    wmove(logWin, 0, 0);
    wrefresh(logWin);
}

void terminateRender() {
    destroy_win(logWin);
    destroy_win(boardWin);
    destroy_win(inputWin);
    endwin();
}
