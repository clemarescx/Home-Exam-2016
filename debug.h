//
// Created by clement on 10/12/16.
//

#ifndef HOME_EXAM_2016_DEBUG_H
#define HOME_EXAM_2016_DEBUG_H

#ifdef DEBUGMODE

#define DEBUG(str, ...) fprintf(stderr, str, __VA_ARGS__)
#define ARRAYPRINT(array)   for(int i = 0; i < array.count; i++){ DEBUG("{%d,%d}\n", array.list[i].x,array.list[i].y);}

#else
#define DEBUG(str, ...)
#define ARRAYPRINT(array)

#endif

#endif //HOME_EXAM_2016_DEBUG_H
