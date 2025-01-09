cmake_minimum_required(VERSION 3.10)

project(client)

set(CMAKE_C_STANDARD 11)

add_executable(client_exec client.c)

target_link_libraries(client_exec PRIVATE shared)

configure_file(boba.txt ${CMAKE_BINARY_DIR}/client/boba.txt COPYONLY)