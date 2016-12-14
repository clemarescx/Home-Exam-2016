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
#include "records.h"
#include "render.h"
#include "stringParser.h"


#define printInputPosX      (char)('A' + inputPosition.x)
#define printInputPosY      inputPosition.y + 1
#define printInputPosition  printInputPosX,printInputPosY
#define getField(pos)       getField(gameBoard, pos.x, pos.y)


//variables
Player player1, player2, players[2];
Board gameBoard;
const size_t CONSOLE_MSG_SIZE = 256; //feedback should be concise, 256 is a large margin

int parseInput(Position *pos);

int main(void) {

#ifdef DEBUGMODE
    DEBUG("%s\n", "DEBUG MODE IS ON");
    srand((unsigned int) time(NULL));
#endif

    //render();

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

    // buffer for feedback messages
    char *consoleMsg = (char *) calloc(CONSOLE_MSG_SIZE, sizeof(char));

    //#######################
    // Start of game loop
    //#######################
    while (running) {

        roundCounter++;
        fflush(stdout);

        Position inputPosition = {-1, -1}; //initialised with invalid values
        Neighbours validNeighbours;

        Position *validMoves = (Position *) calloc(64, sizeof(Position));
        int validMovesCount = getValidMoves(*currentPlayer, validMoves);

        snprintf(consoleMsg,
                 CONSOLE_MSG_SIZE,
                 "%s (%c), your move (enter 'exit' to quit the game): \n",
                 currentPlayer->name,
                 currentPlayer->token);

        int validInput = false;

        while (!validInput) {


            printf("%s", consoleMsg);
#ifdef DEBUGMODE
            DEBUG("validMovesCount = %d\n", validMovesCount);

            // Make the computer pull a random valid move to play
            inputPosition = validMoves[rand() % validMovesCount];
            DEBUG("randpos = {%i,%i}\n", inputPosition.x, inputPosition.y);
#else

            if (!parseInput(&inputPosition)) {
                snprintf(consoleMsg, CONSOLE_MSG_SIZE, "%s", "Please enter valid coordinates (e.g. C4): ");
                continue;
            }
#endif

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

        addMoveToLog(*currentPlayer, inputPosition); //record the current move

        int flippedCount = 0;

        for (int i = 0; i < validNeighbours.count; ++i) {
            Position Direction = validNeighbours.list[i];

            // flipDirection returns the number of fields traversed on the board,
            // including starting point and point after traversing (i.e trigger for traversing to stop).
            // the -2 is an adjustment to get the correct score.
            flippedCount += flipDirection(inputPosition, Direction, *currentPlayer) - 2;
        }

        currentPlayer->score += flippedCount + 1; // include the current move
        opponent->score -= flippedCount;

        printBoard(&gameBoard);

        DEBUG("currentPlayer: %s - score: %d\n", currentPlayer->name, currentPlayer->score);
        DEBUG("opponent: %s - score: %d\n\n\n", opponent->name, opponent->score);


        if (!getValidMoves(*opponent, validMoves) && validMovesCount > 1) {
            snprintf(consoleMsg,
                     CONSOLE_MSG_SIZE,
                     "%s has no valid moves available.\n%s, it's your turn again!\n",
                     opponent->name,
                     currentPlayer->name);
            playerSwitch = !playerSwitch; // toggle once to cancel the end-loop toggle;
        }

        if (opponent->score == 0 ||
            (!getValidMoves(*opponent, validMoves) && !getValidMoves(*currentPlayer, validMoves))) {
            snprintf(consoleMsg,
                     CONSOLE_MSG_SIZE,
                     "\nGame over!\n\n");
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

    printf("%s won the game with a score of %i,\n", winner->name, winner->score);
    printf("versus %s with a score of %i.\n", loser->name, loser->score);

    printLogToFile();


    free(player1.name);
    free(player2.name);
    free(consoleMsg);

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

    char *input = calloc(6, sizeof(char));

    parseString(input, 6);
    DEBUG("Input = '%s'\n", input);

    int c = 0;
    while ((c = fgetc(stdin)) != EOF && c != '\n') {} //flush extra characters from stdin


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

