cmake_minimum_required(VERSION 3.6)
project(Home_Exam__2016)

set(CMAKE_C_STANDARD 11)

#SET(SDL2_ROOT       "/usr/local/")
#SET(SDL_LIB         "-L${SDL2_ROOT}/lib -Wl,-rpath,${SDL2_ROOT}/lib -Wl,--enable-new-dtags -lSDL2")
#SET(SDL_INCLUDE     "-I${SDL2_ROOT}/include/SDL2 -D_REENTRANT")

#SET(LIB "-lSDL2 -I/usr/local/include/SDL2")
SET(LIB "-lncurses -I/usr/include")

SET(CMAKE_C_FLAGS "-g -Wall -Wextra ${LIB}")
#include_directories(include  /usr/local/lib)

set(SOURCE_FILES board.c game.c player.h board.h logic.c logic.h player.c debug.h stringParser.c stringParser.h records.c records.h render.c render.h)

add_executable(Home_Exam__2016 ${SOURCE_FILES})
target_link_libraries(Home_Exam__2016 ncurses)
#target_link_libraries(Home_Exam__2016 SDL2)