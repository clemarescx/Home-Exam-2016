cmake_minimum_required(VERSION 3.6)
project(Home_Exam__2016)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_C_FLAGS "-D DEBUGMODE -g -Wall -Wextra")

set(SOURCE_FILES
    board.c
        game.c
        player.h
        board.h logic.c logic.h player.c debug.h stringParser.c stringParser.h records.c records.h)

add_executable(Home_Exam__2016 ${SOURCE_FILES})