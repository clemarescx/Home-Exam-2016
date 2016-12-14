//
// Created by clement on 12/12/16.
//

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#include "stringParser.h"

/**
 * Checks for keywords in user input
 * @param str
 * @param strSize
 */

void parseString(char *str, size_t strSize) {

    char *format = (char *) calloc(15, sizeof(char)); // 15 is a wide margin just to fit the regex %num[^\n]
    sprintf(format, "%%%d[^\n]", (int) strSize - 1);

    scanf(format, str);

    free(format);

    if (strlen(str) == strlen("exit") &&
        strncmp(str, "exit", strlen(str)) == 0 &&
        strlen(str) != 0)
        exit(EXIT_SUCCESS);

}