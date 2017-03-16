#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <ncurses.h>

#ifdef DEBUGMODE
#include <time.h>
#endif

#include "board.h"
#include "player.h"
#include "logic.h"
#include "records.h"
#include "render.h"
#include "inputParser.h"


#define printInputPosX      (char)('A' + inputPosition.x)
#define printInputPosY      inputPosition.y + 1
#define printInputPosition  printInputPosX,printInputPosY


//variables
extern WINDOW *logWin;
extern Board gameBoard;
Player player1, player2, players[2];
const size_t CONSOLE_MSG_SIZE = 256; //feedback should be concise, 256 is a large margin
int roundCounter = 0;
int parseInput(Position *pos);

void logPrint(Player *player, char *info);


void Init();

int main(void) {

#ifdef DEBUGMODE
    DEBUG("%s\n", "DEBUG MODE IS ON");
    srand((unsigned int) time(NULL));
#endif

    Init();

    Player *currentPlayer = &players[0];
    Player *opponent = &players[1];


    bool playerSwitch = 0;
    bool running = true;

    // buffer for feedback messages
    char *consoleMsg = (char *) calloc(CONSOLE_MSG_SIZE, sizeof(char));

    //#######################
    // Start of game loop
    //#######################
    while (running) {

        roundCounter += !playerSwitch;

        Position inputPosition = {-1, -1}; //initialised with invalid values
        Neighbours validNeighbours;

        Position *validMoves = (Position *) calloc(64, sizeof(Position));
        int validMovesCount = getValidMoves(currentPlayer, validMoves);

        wmove(logWin, 0, 0);

        int validInput = false;

        while (!validInput) {

            logPrint(currentPlayer, consoleMsg);

#ifdef DEBUGMODE
            DEBUG("validMovesCount = %d\n", validMovesCount);

            // Make the computer pull a random valid move to play
            inputPosition = validMoves[rand() % validMovesCount];
            DEBUG("randpos = {%i,%i}\n", inputPosition.x, inputPosition.y);
#else

            if (!parseInput(&inputPosition)) {
                snprintf(consoleMsg, CONSOLE_MSG_SIZE, "%s", "Please enter a valid field.");
                continue;
            }
#endif

            if (getField(&inputPosition) != EMPTY) {
                snprintf(consoleMsg, CONSOLE_MSG_SIZE, "Field %c-%d is already taken.",
                         printInputPosition);
                continue;
            }

            validNeighbours = findValidNeighbours(&inputPosition, currentPlayer);

            if (validNeighbours.count == 0) {
                snprintf(consoleMsg, CONSOLE_MSG_SIZE, "Field %c-%d is illegal.",
                         printInputPosition);
                continue;
            }

            validInput = true;
        }

        addMoveToLog(*currentPlayer, inputPosition); //record the current move

        int flippedCount = 0;

        for (int i = 0; i < validNeighbours.count; ++i) {
            Position Direction = validNeighbours.list[i];

            // flipDirection returns the number of fields traversed on the board,
            // including starting point and point after traversing (i.e trigger for traversing to stop).
            // the -2 is an adjustment to get the correct score.
            flippedCount += flipDirection(&inputPosition, &Direction, currentPlayer) - 2;
        }

        currentPlayer->score += flippedCount + 1; // include the current move
        opponent->score -= flippedCount;

        _printBoard(&gameBoard);

        DEBUG("currentPlayer: %s - score: %d\n", currentPlayer->name, currentPlayer->score);
        DEBUG("opponent: %s - score: %d\n\n\n", opponent->name, opponent->score);


        if (!getValidMoves(opponent, validMoves) && validMovesCount > 1) {
            snprintf(consoleMsg,
                     CONSOLE_MSG_SIZE,
                     "%s has no valid moves.\n%s, it's your turn again!\n",
                     opponent->name,
                     currentPlayer->name);
            playerSwitch = !playerSwitch; // toggle once to cancel the end-loop toggle;
        }

        if (opponent->score == 0 ||
            (!getValidMoves(opponent, validMoves) && !getValidMoves(currentPlayer, validMoves))) {

            //game over
            running = !running;
        }


        playerSwitch = !playerSwitch; //switch the player index
        currentPlayer = &players[playerSwitch];
        opponent = &players[!playerSwitch];

        free(validMoves);

    }
    //#######################
    // end of game loop
    //#######################

#ifdef DEBUGMODE
    printGameLog();
#endif

    Player *winner = (currentPlayer->score > opponent->score ? currentPlayer : opponent);
    Player *loser = (currentPlayer->score < opponent->score ? currentPlayer : opponent);

    refreshLog();
    mvwprintw(logWin, 0, 0, "%s won the game ", winner->name);
    mvwprintw(logWin, 1, 0, "with a score of %i,", winner->score);
    mvwprintw(logWin, 2, 0, "==================");
    mvwprintw(logWin, 3, 0, "versus %s", loser->name);
    mvwprintw(logWin, 4, 0, "with a score of %i.", loser->score);
    printLogToFile();

    mvwprintw(logWin, 6, 0, "Press any key to exit", loser->score);
    wrefresh(logWin);
    getch();


    free(player1.name);
    free(player2.name);
    free(consoleMsg);

    terminateRender();

    return 0;
}


void Init() {
    initscr();
    cbreak();
    refresh();
    initRender();

    initBoard();
    _printBoard(&gameBoard);

    player1.token = WHITE;
    player2.token = BLACK;
    playerInit(&player1);
    playerInit(&player2);
    refresh();
    players[0] = player1;
    players[1] = player2;
}



/**
 * Parse and validates the input
 * Will accept any input containing a valid char for columns
 * and int for rows
 * e.g: e3 will be accepted as well as 3,E or E 3
 * @param pos   a pointer to the position the input is being parsed for
 * @return  1 if the input parsed to a valid a1-h8 coordinate
 */
int parseInput(Position *pos) {

    char *input = calloc(6, sizeof(char));

    parseString(input, 6);
    DEBUG("Input = '%s'\n", input);

    // saving original address, so we can increment input and still be able to free() at the end of parsing
    void *strOrigin = input;

    int x = -1, y = -1;

    while (*input != '\0' && (x < 0 || y < 0)) {
        int _c = toupper(*input);
        if (_c >= 'A' && _c <= 'H' && x < 0) {
            x = _c - 'A';
            input++;
            continue;
        }

        if (*input >= '1' && *input <= '8' && y < 0) {
            y = *input - '0' - 1;
            input++;
            continue;
        }
        input++;
    }

    pos->x = (short) x;
    pos->y = (short) y;

    input = strOrigin;
    free(input);

    return x >= 0 && y >= 0;
}


// returns the ncurses version of a player token
chtype ncursesToken(Player *player) {
    switch (player->token) {
        case BLACK:
            return ACS_DIAMOND;
        case WHITE:
            return ACS_BULLET;
        default:
            return (chtype) " ";
    }
}

// Formats log information
void logPrint(Player *player, char *info) {
    refreshLog();
    mvwprintw(logWin, 0, 0, "Round %d", roundCounter);
    mvwprintw(logWin, 1, 0, "%s (", player->name);
    waddch(logWin, ncursesToken(player));
    wprintw(logWin, ")");
    mvwprintw(logWin, 2, 0, "Score: %d", player->score);

    mvwprintw(logWin, logWin->_maxy - 3, 0, "%s", info);
    mvwprintw(logWin, logWin->_maxy - 1, 0, "Enter 'exit' to quit");
    wrefresh(logWin);
}
