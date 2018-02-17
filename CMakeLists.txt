cmake_minimum_required(VERSION 3.6)
project(Home_Exam__2016)

set(CMAKE_C_STANDARD 11)

SET(LIB "-lncurses -I/usr/include")

SET(CMAKE_C_FLAGS "-g -Wall -Wextra ${LIB}")

set(SOURCE_FILES
        board.c
        game.c
        player.h
        board.h
        logic.c
        logic.h
        player.c
        debug.h
        inputParser.c
        inputParser.h
        records.c
        records.h
        render.c
        render.h
        AI_Logic.c
        AI_Logic.h)

add_executable(Home_Exam__2016 ${SOURCE_FILES})
target_link_libraries(Home_Exam__2016 ncurses)
