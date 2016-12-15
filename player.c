#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "player.h"
#include "inputParser.h"

short playerCount = 0;
extern WINDOW *logWin, *inputWin;

/**
 * Initialise a player struct variables and name;
 * @param player
 * @return
 */
int playerInit(Player *player) {

    player->score = 2;
    player->name = (char *) calloc(PLAYERNAME_SIZE, sizeof(char));

#ifdef DEBUGMODE
    snprintf(player->name, PLAYERNAME_SIZE, "Pl4Y3r-number-%d", playerCount + 1);
#else

    int y = 0;
    //this is -such- ugly code, but I don't think ncurses can do much for autowrap...
    mvwprintw(logWin, y++, 0, "Player %d, please enter your", playerCount + 1);
    mvwprintw(logWin, y, 0, "name");
    wrefresh(logWin);

    parseString(player->name, PLAYERNAME_SIZE);

    if (strlen(player->name) < 3) {
        int max_y = getmaxy(logWin) - 5;

        // the cursor must be reset for every line
        // otherwise the box gets destroyed
        mvwprintw(logWin, max_y++, 0, "'%s' was too short, so we", player->name);
        mvwprintw(logWin, max_y++, 0, "baptized you '");

        wattron(logWin, A_BOLD);
        wprintw(logWin, "Player%d", playerCount + 1);
        wattroff(logWin, A_BOLD);
        wprintw(logWin, "'.");

        mvwprintw(logWin, max_y++, 0, "...We hope you're happy");
        mvwprintw(logWin, max_y, 0, "about that.");
        wrefresh(logWin);
        //refresh();
        snprintf(player->name, 8, "Player%i", playerCount + 1);
    }
#endif

    playerCount++;
    return 0;
}