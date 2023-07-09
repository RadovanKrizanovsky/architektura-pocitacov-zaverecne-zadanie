cmake_minimum_required(VERSION 3.17)
project(posledne_zadanie C)

set(CMAKE_C_STANDARD 99)

add_executable(posledne_zadanie main.c)

link_libraries(ws2_32 wsock32)

target_link_libraries(posledne_zadanie wsock32 ws2_32)