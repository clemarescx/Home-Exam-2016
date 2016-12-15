//
// Created by clement on 12/12/16.
//

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <ncurses.h>

#include "inputParser.h"
#include "render.h"

extern WINDOW *inputWin;

/**
 * Checks for keywords in user input
 * @param str
 * @param strSize
 */
void parseString(char *str, size_t strSize) {

    //char *format = (char *) calloc(15, sizeof(char)); // 15 is a wide margin just to fit the regex %num[^\n]
    //sprintf(format, "%%%d[^\n]", (int) strSize - 1);

    wmove(inputWin, 1, 3);
    //echo();
    wrefresh(inputWin);
    int c = 0;
    unsigned int charcount = 0;

    while (charcount < strSize && (c = wgetch(inputWin)) != '\0' && c != '\n') {
        str[charcount++] = (char) c; // not ideal for all OS's... but for Linux, that will do
    }

    //wscanw(inputWin, format, str);
    str[strSize] = '\0';

    initInputWin();
    //wrefresh(inputWin);
    //free(format);

    if (strlen(str) == strlen("exit") &&
        strncmp(str, "exit", strlen(str)) == 0 &&
        strlen(str) != 0) {
        endwin();
        exit(EXIT_SUCCESS);
    }

}