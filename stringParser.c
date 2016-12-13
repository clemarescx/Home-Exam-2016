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

    char *format = (char *) calloc(strSize, sizeof(char));
    sprintf(format, "%%%d[^\n]", (int) strSize);

    scanf(format, str);

    free(format);

    if (strncmp(str, "exit", strlen(str)) == 0 && strlen(str) != 0) exit(EXIT_SUCCESS);

}