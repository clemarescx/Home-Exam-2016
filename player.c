#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "player.h"
#include "inputParser.h"

short playerCount = 0;
extern WINDOW *msgWindow, *inputWindow;

void initAI(Player* player);
void promptForName(Player *player);
/**
 * Initialise a player struct variables and name;
 * @param player
 * @return
 */
int initialisePlayer(Player *player, PlayerType playerType) {

    playerCount++;

    player->name = (char *) calloc(PLAYERNAME_SIZE, sizeof(char));

    if(playerType == HUMAN)
        promptForName(player);
    else
        snprintf(player->name, PLAYERNAME_SIZE, "Pl4Y3r-number-%d", playerCount);

    player->score = 2;
    player->type = playerType;

    return 0;
}

void promptForName(Player *player){
    int y = 0;
    //this is -such- ugly code, but I don't think ncurses can do much for autowrap...
    mvwprintw(msgWindow, y++, 0, "Player %d, please enter your", playerCount);
    mvwprintw(msgWindow, y, 0, "name");
    wrefresh(msgWindow);

    parseString(player->name, PLAYERNAME_SIZE);

    if (strlen(player->name) < 3) {
        int cursor_yPos = getmaxy(msgWindow) - 5;

        // the cursor must be reset for every line
        // otherwise the box gets destroyed
        mvwprintw(msgWindow, cursor_yPos++, 0, "'%s' was too short, so we", player->name);
        mvwprintw(msgWindow, cursor_yPos++, 0, "baptized you '");

        wattron(msgWindow, A_BOLD);
        wprintw(msgWindow, "Player%d", playerCount);
        wattroff(msgWindow, A_BOLD);
        wprintw(msgWindow, "'.");

        mvwprintw(msgWindow, cursor_yPos++, 0, "...We hope you're happy");
        mvwprintw(msgWindow, cursor_yPos, 0, "about that.");
        wrefresh(msgWindow);
        //refresh();
        snprintf(player->name, 8, "Player%i", playerCount);
    }

}

void initAI(Player* player){
    snprintf(player->name, PLAYERNAME_SIZE, "Pl4Y3r-number-%d", playerCount + 1);
}