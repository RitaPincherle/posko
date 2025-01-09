cmake_minimum_required(VERSION 3.10)

project(server)

set(CMAKE_C_STANDARD 11)

add_executable(server_exec server.c)

target_link_libraries(server_exec PRIVATE shared)

configure_file(boba.txt ${CMAKE_BINARY_DIR}/server/boba.txt COPYONLY)