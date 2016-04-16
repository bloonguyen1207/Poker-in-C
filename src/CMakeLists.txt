cmake_minimum_required(VERSION 3.3)

set(SOURCE_FILES demo.c)

add_executable(demo demo.c)
target_link_libraries(demo ${CURSES_LIBRARIES})

add_executable(poker poker.c)
target_link_libraries(poker ${CURSES_LIBRARIES})
