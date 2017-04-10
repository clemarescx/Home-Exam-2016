//
// Created by clement on 12/12/16.
//

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <ncurses.h>
#include <ctype.h>

#include "inputParser.h"
#include "render.h"
#include "debug.h"

extern WINDOW *inputWindow;

/**
 * Checks for keywords in user input
 * @param str
 * @param strSize
 */
void parseString(char *str, size_t strSize) {

    //char *format = (char *) calloc(15, sizeof(char)); // 15 is a wide margin just to fit the regex %num[^\n]
    //sprintf(format, "%%%d[^\n]", (int) strSize - 1);

    wmove(inputWindow, 1, 3);
    //echo();
    wrefresh(inputWindow);
    int c = 0;
    unsigned int charcount = 0;

    while (charcount < strSize && (c = wgetch(inputWindow)) != '\0' && c != '\n') {
        str[charcount++] = (char) c; // not ideal for all OS's... but for Linux, that will do
    }

    //wscanw(inputWindow, format, str);
    str[strSize] = '\0';

    initInputWin();
    //wrefresh(inputWindow);
    //free(format);

    if (strlen(str) == strlen("exit") &&
        strncmp(str, "exit", strlen(str)) == 0 &&
        strlen(str) != 0) {
        endwin();
        exit(EXIT_SUCCESS);
    }

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
