#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "board.h"
#include "player.h"
#include "logic.h"


#define printInputPosX     (char)('A' + inputPosition.x)
#define printInputPosY      inputPosition.y + 1
#define printInputPosition  printInputPosX,printInputPosY

//variables
Player player1, player2;
Board gameBoard;


//functions
//int playerInit(Player *player);
int parseInput(char *str, Position *pos);


int main(void) {

    /* options:
     * - a start menu
     * - add a load function (parse a save file with moves recorded)
     * - high scores
     *
     * */
    DEBUG("%s\n", "DEBUG MODE IS ON");

    player1.token = WHITE;
    player2.token = BLACK;

    if (!playerInit(&player1)) return EXIT_FAILURE;
    if (!playerInit(&player2)) return EXIT_FAILURE;


    initBoard(&gameBoard);

    bool playerSwitch = 0;

    Player players[2] = {player1, player2};
    Player currentPlayer = players[playerSwitch];

    bool running = true;

    int loopCounter = 3;

    while (loopCounter--) {

        printBoard(&gameBoard);



        // OPT: player can enter exit to exit the game
        //TODO: if move is illegal, player can input again
        //one check 8 times, NOT 8 individual checks for directions

        printf("%s (%c), your move: ", currentPlayer.name, currentPlayer.token);
        char *input = calloc(10, sizeof(char));
        int success = false;
        Position inputPosition;


#ifdef DEBUGON
        //for parsing tests + semantic tests
        //char *wonkyEntries = {"3,F","\n", " ", "a3", " a,3", " a3", "A3", " a 3", " A , # 4", "h8"};

        //for validity tests
        char *inputs[] = {"c3", "c4", "c5", "c6 ", "d3 ", "d4", "d5", "d6 ", "e3 ", "e4", "e5", "e6 ", "f3 ", "f4 ",
                          "f5 ", "f6", "a1", "h1", "a8", "h8"};
        for (int i = 0; i < 20; i++) {
            DEBUG("%s\n", "========================================================");
            if (parseInput(inputs[i], &inputPosition)) {
                DEBUG("Parse success: %s returns position {%i,%i}\n", inputs[i], inputPosition.x, inputPosition.y);
                if (getField(gameBoard, inputPosition.x, inputPosition.y) == EMPTY) {
                    DEBUG("Emptyfield success: This should show 0 (index of EMPTY):%c\n",
                          getField(gameBoard, inputPosition.x, inputPosition.y));
                    if (isValidMove(currentPlayer, inputPosition)) {
                        DEBUG("ValidMove success: {%i,%i} is playable\n", inputPosition.x, inputPosition.y);
                    }
                }
            }

        }
#else
        while (!success) {

            fgets(input, 10, stdin);


            if (!parseInput(input, &inputPosition)) {
                printf("Please enter valid coordinates (e.g. C4): ");
                continue;
            }

            //if (!isEmptyField(gameBoard, inputPosition)) {
            if (getField(gameBoard,inputPosition.x,inputPosition.y) != EMPTY) {
                printf("Field %c-%d is already occupied.\n"
                               "Please enter a valid move: ",
                       printInputPosition);
                //(char) ('A' + inputPosition.x),
                //inputPosition.y + 1);
                continue;
            }

            if (!isValidMove(currentPlayer, inputPosition)) {
                printf("Field %c-%d is not a valid move.\n"
                               "Please try again: ",
                       printInputPosition);
                //(char) ('A' + inputPosition.x),
                //inputPosition.y + 1
                continue;
            }


            printf("%c-%d is a valid move!\n", printInputPosition);

            success = true;

        }
#endif


        //TODO: update the board according to the new move
        //  - update score of both players
        //  - flip affected fields
        //  - record the executed move to an external file

        //TODO: check if next player has any valid moves
        // if not, notify, and current player plays again (no playerSwitch)
        //TODO: check if game is finished (score == 0 for one player)

        playerSwitch = !playerSwitch; //switch the player index
        currentPlayer = players[playerSwitch];

        if (running) running = !running;

#ifndef DEBUGON

        free(input);
#endif
    }
    //TODO: if game finished, announce winner + exit program

    //TODO: make a makefile that compiles project correctly
    // only files changed since last compile ( see forelesning)

    free(player1.name);
    free(player2.name);

    return 0;
}

/**
 * validates the input
 * @param str
 * @param pos
 * @return
 */
int parseInput(char *input, Position *pos) {
    DEBUG("Input = '%s'\n", input);

    char *str = calloc(10, sizeof(char));
    //__intptr_t strOrigin = (__intptr_t ) str;
    void *strOrigin = str;
    sscanf(input, "%[^\n]", str);

    int x = -1, y = -1;

    while (*str != '\0' && (x < 0 || y < 0)) {
        int c = toupper(*str);
        if (c >= 'A' && c <= 'H' && x < 0) {
            x = c - 'A';
            str++;
            continue;
        }

        if (*str >= '1' && *str <= '8' && y < 0) {
            y = *str - '0' - 1;
            str++;
            continue;
        }
        str++;
    }
    DEBUG("x = %d\n", x);
    DEBUG("y = %d\n", y);

    pos->x = (short) x;
    pos->y = (short) y;

    str = strOrigin;
    free(str);

    return x >= 0 && y >= 0;
}

