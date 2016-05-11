cmake_minimum_required(VERSION 3.3)

set(SOURCE_FILES poker.c)

add_executable(poker poker.c)
target_link_libraries(poker ${CURSES_LIBRARIES})

add_executable(poker_ui poker_ui.c)
target_link_libraries(poker_ui ${CURSES_LIBRARIES})