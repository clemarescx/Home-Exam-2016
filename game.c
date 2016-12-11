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
Player player1, player2;
Board gameBoard;

int parseInput(char *str, Position *pos);

int main(void) {

    /* options:
     * - a start menu
     * - add a load function (parse a save file with moves recorded)
     * - high scores
     *
     * */
#ifdef DEBUGMODE
    DEBUG("%s\n", "DEBUG MODE IS ON");
    srand(time(NULL));
#endif

    player1.token = WHITE;
    player2.token = BLACK;

    if (!playerInit(&player1)) return EXIT_FAILURE;
    if (!playerInit(&player2)) return EXIT_FAILURE;


    initBoard(&gameBoard);

    bool playerSwitch = 0;

    Player players[2] = {player1, player2};

    Player currentPlayer = players[playerSwitch];
    Player opponent = players[!playerSwitch];

    bool running = true;

    int roundCounter = 0;
    printBoard(&gameBoard);

    while (running) {

        roundCounter++;
        fflush(stdout);

        printf("%s (%c), your move: ", currentPlayer.name, currentPlayer.token);
        char *input = calloc(10, sizeof(char));

        Position inputPosition = {-1, -1};
        Neighbours validNeighbours;

        Position *validMoves = (Position *) calloc(64, sizeof(Position));
        int validMovesCount = getValidMoves(currentPlayer, validMoves);

#ifdef DEBUGMODE
        int validInput = false;

        while (!validInput) {

            // Make the computer pull a random valid move to play
            Position randPos = validMoves[rand() % validMovesCount ];
            printf("randpos = {%i,%i}\n", randPos.x, randPos.y);
            char x = (char) ('A' + randPos.x);
            sprintf(input,"%c%i",x,randPos.y+1);
            printf("input = %s\n", input);

            if (!parseInput(input, &inputPosition)) {
                printf("Please enter valid coordinates (e.g. C4): ");
                continue;
            }

            if (getField(inputPosition) != EMPTY) {
                printf("Field %c-%d is already occupied.\n"
                               "Please enter a valid move: ",
                       printInputPosition);
                continue;
            }

            validNeighbours = findValidNeighbours(inputPosition, currentPlayer);


            if (validNeighbours.count == 0) {
                printf("Field %c-%d is not a valid move.\n"
                               "Please try again: ",
                       printInputPosition);
                continue;
            }

            //printf("%c-%d is a valid move!\n", printInputPosition);

            validInput = true;

        }

#else
        int validInput = false;
        while (!validInput) {

            fgets(input, 10, stdin);

            if (!parseInput(input, &inputPosition)) {
                printf("Please enter valid coordinates (e.g. C4): ");
                continue;
            }

            if (getField(inputPosition) != EMPTY) {
                printf("Field %c-%d is already occupied.\n"
                               "Please enter a valid move: ",
                       printInputPosition);
                continue;
            }

            validNeighbours = findValidNeighbours(inputPosition, currentPlayer);


            if (validNeighbours.count == 0) {
                printf("Field %c-%d is not a valid move.\n"
                               "Please try again: ",
                       printInputPosition);
                continue;
            }

            printf("%c-%d is a valid move!\n", printInputPosition);

            validInput = true;

        }
#endif


        currentPlayer.log[currentPlayer.roundsPlayed++] = inputPosition;

        for (int i = 0; i < validNeighbours.count; ++i) {
            Position Direction = validNeighbours.list[i];
            int score = flipDirection(inputPosition, Direction, currentPlayer);
            currentPlayer.score += score;
            opponent.score -= score;
        }

        printBoard(&gameBoard);


        if (!getValidMoves(opponent, validMoves) && validMovesCount < 0) {
            printf("%s has no valid moves available.\n", opponent.name);
            printf("%s, it's your turn again!\n", currentPlayer.name);
            playerSwitch = !playerSwitch; // toggle once to cancel the end-loop toggle;
        }

        if (opponent.score == 0 ||
            (!getValidMoves(opponent, validMoves) && !getValidMoves(currentPlayer, validMoves))) {
            printf("Game over, %s won the game with a score of %i!\n", currentPlayer.name, currentPlayer.score);
            running = !running;
        }


        playerSwitch = !playerSwitch; //switch the player index
        currentPlayer = players[playerSwitch];
        opponent = players[!playerSwitch];

        free(input);
        free(validMoves);

    }

    //TODO: record the executed move to an external file



    free(player1.name);
    free(player2.name);

    //TODO: make a makefile that compiles project correctly
    // only files changed since last compile ( see forelesning)

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

    void *strOrigin = str; // original address, needed for free();
    sscanf(input, "%[^\n]", str);

    if (strcmp(str, "exit") == 0) exit(EXIT_FAILURE);

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

    pos->x = (short) x;
    pos->y = (short) y;

    str = strOrigin;
    free(str);

    return x >= 0 && y >= 0;
}

