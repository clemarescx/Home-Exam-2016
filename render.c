//
// Created by clement on 13/12/16.
//

//#include <SDL.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "render.h"

int render2() {
    char mesg[] = "Just a string";        /* message to be appeared on the screen */
    int row, col;                /* to store the number of rows and *
					 * the number of colums of the screen */
    initscr();                /* start the curses mode */
    getmaxyx(stdscr, row, col);        /* get the number of rows and columns */
    mvprintw(row / 2, (col - strlen(mesg)) / 2, "%s", mesg);
    /* print the message at the center of the screen */
    mvprintw(row - 2, 0, "This screen has %d rows and %d columns\n", row, col);
    printw("Try resizing your window(if possible) and then run this program again");
    refresh();
    getch();
    endwin();

    return 0;
}

void render() {
    int ch;

    initscr();            /* Start curses mode 		*/
    raw();                /* Line buffering disabled	*/
    keypad(stdscr, TRUE);        /* We get F1, F2 etc..		*/
    noecho();            /* Don't echo() while we do getch */

    printw("Type any character to see it in bold\n");
    ch = getch();            /* If raw() hadn't been called
					 * we have to press enter before it
					 * gets to the program 		*/
    if (ch == KEY_F(1))        /* Without keypad enabled this will */
        printw("F1 Key pressed");/*  not get to us either	*/
        /* Without noecho() some ugly escape
         * charachters might have been printed
         * on screen			*/
    else {
        printw("The pressed key is ");
        attron(A_BOLD);
        printw("%c", ch);
        attroff(A_BOLD);
    }
    refresh();            /* Print it on to the real screen */
    getch();            /* Wait for user input */
    endwin();            /* End curses mode		  */

    render2();

    exit(EXIT_SUCCESS);
}

/*
int render(){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "SDL_Init error:%s\n",SDL_GetError());
        exit(EXIT_FAILURE);
    }
    printf("SDL WORKS!");
    SDL_Quit();
    return 0;

}
*/