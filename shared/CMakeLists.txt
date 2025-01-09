add_library(shared
    board.c
    snake.c
    timer.c
)

target_include_directories(shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})