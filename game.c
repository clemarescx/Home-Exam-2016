#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <memory.h>

#ifdef DEBUGMODE
#include <time.h>
#endif

#include "board.h"
#include "player.h"
#include "logic.h"


#define printInputPosX     (char)('A' + inputPosition.x)
#define printInputPosY      inputPosition.y + 1
#define printInputPosition  printInputPosX,printInputPosY
#define getField(pos)           getField(gameBoard, pos.x, pos.y)


//variables
Player player1, player2, players[2];
Board gameBoard;
const size_t CONSOLE_MSG_SIZE = 256;

int parseInput(Position *pos);

int main(void) {

    /* options:
     * - a start menu
     * - add a load function (parse a save file with moves recorded)
     * - high scores
     *
     * */
#ifdef DEBUGMODE
    DEBUG("%s\n", "DEBUG MODE IS ON");
    srand((unsigned int) time(NULL));
#endif

    player1.token = WHITE;
    player2.token = BLACK;

    playerInit(&player1);
    playerInit(&player2);

    players[0] = player1;
    players[1] = player2;

    Player *currentPlayer = &players[0];
    Player *opponent = &players[1];

    initBoard(&gameBoard);
    printBoard(&gameBoard);

    bool playerSwitch = 0;
    bool running = true;
    int roundCounter = 0;

    // holder for console messages
    char *consoleMsg = (char *) calloc(CONSOLE_MSG_SIZE, sizeof(char));

    //#######################
    // Start of game loop
    //#######################
    while (running) {

        roundCounter++;
        fflush(stdout);


        Position inputPosition = {-1, -1};
        Neighbours validNeighbours;

        Position *validMoves = (Position *) calloc(64, sizeof(Position));
        int validMovesCount = getValidMoves(*currentPlayer, validMoves);

        snprintf(consoleMsg, CONSOLE_MSG_SIZE, "%s (%c), your move: \n", currentPlayer->name, currentPlayer->token);

#ifdef DEBUGMODE
        int validInput = false;

        while (!validInput) {

            printf("%s", consoleMsg);
            DEBUG("validMovesCount = %d\n", validMovesCount);
            // Make the computer pull a random valid move to play
            inputPosition = validMoves[rand() % validMovesCount];
            DEBUG("randpos = {%i,%i}\n", inputPosition.x, inputPosition.y);

/*
            if (!parseInput(&inputPosition)) {
                snprintf(consoleMsg, CONSOLE_MSG_SIZE, "%s", "Please enter valid coordinates (e.g. C4): ");
                continue;
            }
*/
            if (getField(inputPosition) != EMPTY) {
                snprintf(consoleMsg, CONSOLE_MSG_SIZE, "Field %c-%d is already occupied.\nPlease enter a valid move: ",
                         printInputPosition);
                continue;
            }

            validNeighbours = findValidNeighbours(inputPosition, *currentPlayer);

            if (validNeighbours.count == 0) {
                snprintf(consoleMsg, CONSOLE_MSG_SIZE, "Field %c-%d is not a valid move.\nPlease try again: ",
                         printInputPosition);
                continue;
            }

            validInput = true;

        }

#else

        int validInput = false;
        while (!validInput) {

            printf("%s", consoleMsg);

            if (!parseInput(&inputPosition)) {
                snprintf(consoleMsg, CONSOLE_MSG_SIZE, "%s", "Please enter valid coordinates (e.g. C4): ");
                continue;
            }

            if (getField(inputPosition) != EMPTY) {
                snprintf(consoleMsg, CONSOLE_MSG_SIZE, "Field %c-%d is already occupied.\nPlease enter a valid move: ",
                         printInputPosition);
                continue;
            }

            validNeighbours = findValidNeighbours(inputPosition, *currentPlayer);

            if (validNeighbours.count == 0) {
                snprintf(consoleMsg, CONSOLE_MSG_SIZE, "Field %c-%d is not a valid move.\nPlease try again: ",
                         printInputPosition);
                continue;
            }

            validInput = true;
        }
#endif

        currentPlayer->log[currentPlayer->roundsPlayed++] = inputPosition;

        int flippedCount = 0;

        for (int i = 0; i < validNeighbours.count; ++i) {
            Position Direction = validNeighbours.list[i];
            // the value return is the distance which always starts with 1 and returns when 1 too many
            // the -2 is an adjustment for this
            flippedCount += flipDirection(inputPosition, Direction, *currentPlayer) - 2;
        }

        currentPlayer->score += flippedCount + 1; // include the current move
        opponent->score -= flippedCount;
        printBoard(&gameBoard);

        DEBUG("currentPlayer: %s - score: %d\n", currentPlayer->name, currentPlayer->score);
        DEBUG("opponent: %s - score: %d\n\n\n", opponent->name, opponent->score);


        if (!getValidMoves(*opponent, validMoves) && validMovesCount > 1) {
            printf("%s has no valid moves available.\n", opponent->name);
            printf("%s, it's your turn again!\n", currentPlayer->name);
            playerSwitch = !playerSwitch; // toggle once to cancel the end-loop toggle;
        }

        if (opponent->score == 0 ||
            (!getValidMoves(*opponent, validMoves) && !getValidMoves(*currentPlayer, validMoves))) {
            printf("Game over!\n");
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

    //TODO: record the executed move to an external file
    Player *winner = (currentPlayer->score > opponent->score ? currentPlayer : opponent);
    Player *loser = (currentPlayer->score < opponent->score ? currentPlayer : opponent);;

    printf("%s won the game with a score of %i,\n", winner->name, winner->score);
    printf("versus %s with a score of %i.\n", loser->name, loser->score);

    free(player1.name);
    free(player2.name);
    free(consoleMsg);

    //TODO: make a makefile that compiles project correctly
    // only files changed since last compile ( see forelesning)

    return 0;
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

    char *input = calloc(5, sizeof(char));
    scanf("%5[^\n]", input);

    printf("in = %s\n", input);
    int c = 0;
    while ((c = fgetc(stdin)) != EOF && c != '\n') {} //flush extra characters from stdin

    DEBUG("Input = '%s'\n", input);
    printf("input = %s\n", input);

    void *strOrigin = input; // original address, needed for free();

    if (strcmp(input, "exit") == 0) exit(EXIT_FAILURE);

    int x = -1, y = -1;

    while (*input != '\0' && (x < 0 || y < 0)) {
        int c = toupper(*input);
        if (c >= 'A' && c <= 'H' && x < 0) {
            x = c - 'A';
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

