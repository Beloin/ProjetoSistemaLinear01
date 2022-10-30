cmake_minimum_required(VERSION 3.23)
project(project_01_cn_ep1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

target_link_libraries(pow m)

add_executable(project_01_cn_ep1
        ep1.c)
