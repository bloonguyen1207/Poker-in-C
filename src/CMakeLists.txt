cmake_minimum_required(VERSION 3.3)

set(SOURCE_FILES poker_ui.c demo.c)

set(HEADER_FILES poker.h poker_ui.h)

set(SORTING_SOURCE_FILES poker.c)

add_executable(poker poker.c ${SORTING_SOURCE_FILES})
target_link_libraries(poker ${CURSES_LIBRARIES})

add_executable(demo demo.c)
target_link_libraries(demo ${CURSES_LIBRARIES})

add_executable(poker_ui poker_ui.c ${SORTING_SOURCE_FILES})
target_link_libraries(poker_ui ${CURSES_LIBRARIES})