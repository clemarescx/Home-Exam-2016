//
// Created by clement on 10/12/16.
//

#ifndef HOME_EXAM_2016_DEBUG_H
#define HOME_EXAM_2016_DEBUG_H

#ifdef DEBUGMODE

/* Commented out... does not play well with ncurses.
//#define DEBUG(str, ...) wprintw(logWin, str, __VA_ARGS__)
//#define ARRAYPRINT(array)   for(int i = 0; i < array.count; i++){ DEBUG("{%d,%d}\n", array.list[i].x,array.list[i].y);}
*/
#define DEBUG(str, ...)
#define ARRAYPRINT(array)

#else
#define DEBUG(str, ...)
#define ARRAYPRINT(array)

#endif

#endif //HOME_EXAM_2016_DEBUG_H
