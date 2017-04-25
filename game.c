#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <ctype.h>
#include <ncurses.h>

#include <time.h>

#include "board.h"
#include "player.h"
#include "logic.h"
#include "records.h"
#include "render.h"
#include "inputParser.h"
#include "AI_Logic.h"


#define printInputPosX(x)      (char)('A' + x)
#define printInputPosY(y)      y + 1
#define printInputPosition(in)  printInputPosX(in->x),printInputPosY(in->y)
#define WIDTH 30
#define HEIGHT 10


//variables
extern WINDOW *msgWindow;
extern Board gBoard;

Player player1, player2, players[2];
Player *currentPlayer;

char *consoleMsg;
const size_t CONSOLE_MSG_SIZE = 256; //feedback should be concise, 256 is a large margin
int roundCounter = 0;


static int startx = 0, starty = 0;
char *choices[] = {
        "Player vs Player",
        "Player vs Computer",
        "Debug mode",
        "Exit"
};
int n_choices = sizeof(choices) / sizeof(char *);

Position inputPosition;
Position *validMoves;
PositionList validNeighbours;

void getInput(Position *inputPosition);

void print_menu(WINDOW *menu_win, int highlight);

void logPrint(Player *player, char *info);

void initialiseView();

void initialisePlayers(int choice);

int selectMenuOption();

void executeMenuChoice(int choice);

bool noValidMovesLeft();

int main(void) {

#ifdef DEBUGMODE
    DEBUG("%s\n", "DEBUG MODE IS ON");
#endif
    srand((unsigned int) time(NULL));

    // #########################################################

    int choice = selectMenuOption();


    // #########################################################

    player1.token = WHITE;
    player2.token = BLACK;


#ifdef DEBUGMODE
    choice = 3;
#endif

    initialiseView();
    initialisePlayers(choice);


    // buffer for feedback messages
    consoleMsg = (char *) calloc(CONSOLE_MSG_SIZE, sizeof(char));

    currentPlayer = &players[0];
    Player *opponent = &players[1];


    bool playerSwitch = 0;
    bool running = true;


    //#######################
    // Start of game loop
    //#######################
    while (running) {

        roundCounter++;

        // initialised with invalid values
        inputPosition.x = -1;
        inputPosition.y = -1;


        validMoves = (Position *) calloc(32, sizeof(Position));
        int validMovesCount = getValidMoves(currentPlayer, validMoves, &gBoard);

        wmove(msgWindow, 0, 0);

        if (currentPlayer->type == HUMAN) {
            getInput(&inputPosition);
        } else {
            inputPosition = findBestMove(*currentPlayer, *opponent, validMoves, validMovesCount);
            validNeighbours = getFlippableTokens(&inputPosition, currentPlayer, &gBoard);
        }

        addMoveToLog(*currentPlayer, inputPosition); //record the current move

        //        int flippedCount = 0;
        for (int i = 0; i < validNeighbours.count; ++i) {
            Position direction = validNeighbours.list[i];
            //            flippedCount += flipDirection(&inputPosition, &direction, currentPlayer, &gBoard) - 2;
            flipDirection(&inputPosition, &direction, currentPlayer, &gBoard);
        }
        //        currentPlayer->score += flippedCount +1; // include the current move
        //        opponent->score -= flippedCount;
        updateScores(currentPlayer, opponent, &gBoard);

        _printBoard(&gBoard);

        DEBUG("currentPlayer: %s - score: %d\n", currentPlayer->name, currentPlayer->score);
        DEBUG("opponent: %s - score: %d\n\n\n", opponent->name, opponent->score);


        if (!getValidMoves(opponent, validMoves, &gBoard) && validMovesCount > 1) {
            snprintf(consoleMsg,
                     CONSOLE_MSG_SIZE,
                     "%s has no valid moves.\n%s, it's your turn again!\n",
                     opponent->name,
                     currentPlayer->name);
            playerSwitch = !playerSwitch; // toggle once to cancel the end-loop toggle;
        }

        if (!getValidMoves(opponent, validMoves, &gBoard) && !getValidMoves(currentPlayer, validMoves, &gBoard)) {

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
    mvwprintw(msgWindow, 0, 0, "%s won the game ", winner->name);
    mvwprintw(msgWindow, 1, 0, "with a score of %i,", winner->score);
    mvwprintw(msgWindow, 2, 0, "==================");
    mvwprintw(msgWindow, 3, 0, "versus %s", loser->name);
    mvwprintw(msgWindow, 4, 0, "with a score of %i.", loser->score);
    printLogToFile();

    mvwprintw(msgWindow, 6, 0, "Press any key to exit", loser->score);
    wrefresh(msgWindow);
    getch();


    free(player1.name);
    free(player2.name);
    free(consoleMsg);

    terminateRender();

    return 0;
}


void initialiseView() {
    initRender();

    initBoard();
    _printBoard(&gBoard);

}

void initialisePlayers(int choice) {
    initialisePlayer(&player1, HUMAN);

    switch (choice) {
        case 1:
            initialisePlayer(&player2, HUMAN);
            break;
        case 2:
            initialisePlayer(&player2, COMPUTER);
            break;
        case 3:
            initialisePlayer(&player1, COMPUTER);
            initialisePlayer(&player2, COMPUTER);
            break;
        default:
            system(EXIT_SUCCESS);
            break;
    }

    refresh();

    players[0] = player1;
    players[1] = player2;

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
    mvwprintw(msgWindow, 0, 0, "Round %d", roundCounter);
    mvwprintw(msgWindow, 1, 0, "%s (", player->name);
    waddch(msgWindow, ncursesToken(player));
    wprintw(msgWindow, ")");
    mvwprintw(msgWindow, 2, 0, "Score: %d", player->score);

    mvwprintw(msgWindow, msgWindow->_maxy - 3, 0, "%s", info);
    mvwprintw(msgWindow, msgWindow->_maxy - 1, 0, "Enter 'exit' to quit");
    wrefresh(msgWindow);
}


void print_menu(WINDOW *menu_win, int highlight) {
    int x, y, i;

    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    for (i = 0; i < n_choices; ++i) {
        if (highlight == i + 1) /* High light the present choice */
        {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        ++y;
    }
    wrefresh(menu_win);
}

int selectMenuOption() {
    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c;

    initscr();
    clear();
    noecho();
    cbreak();    /* Line buffering disabled. pass on everything */
    startx = (80 - WIDTH) / 2;
    starty = (24 - HEIGHT) / 2;

    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(menu_win, TRUE);
    mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
    refresh();

    print_menu(menu_win, highlight);
    while (1) {
        c = wgetch(menu_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_choices)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                //                mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
                refresh();
                break;
        }
        print_menu(menu_win, highlight);
        if (choice != 0)    /* User did a choice come out of the infinite loop */
            break;
    }
    //    clrtoeol();
    refresh();
    echo();
    //    endwin();
    return choice;
}

void getInput(Position *inputPosition) {
    int validInput = false;

    while (!validInput) {

        logPrint(currentPlayer, consoleMsg);

#ifdef DEBUGMODE
        DEBUG("validMovesCount = %d\n", validMovesCount);

            // Make the computer pull a random valid move to play
            *inputPosition = validMoves[rand() % validMovesCount];
            DEBUG("randpos = {%i,%i}\n", inputPosition.x, inputPosition.y);
#else

        if (!parseInput(inputPosition)) {
            snprintf(consoleMsg, CONSOLE_MSG_SIZE, "%s", "Please enter a valid field.");
            continue;
        }

        if (getField(&gBoard, inputPosition) != EMPTY) {
            snprintf(consoleMsg, CONSOLE_MSG_SIZE, "Field %c-%d is already taken.",
                     printInputPosition(inputPosition));
            continue;
        }
#endif
        validNeighbours = getFlippableTokens(inputPosition, currentPlayer, &gBoard);

        if (validNeighbours.count == 0) {
            snprintf(consoleMsg, CONSOLE_MSG_SIZE, "Field %c-%d is illegal.",
                     printInputPosition(inputPosition));
            continue;
        }

        validInput = true;
    }
}
