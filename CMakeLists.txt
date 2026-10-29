cmake_minimum_required(VERSION 3.10)
project(lab3 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic -g)

find_package(Threads REQUIRED)

add_executable(lab3 src/main.c src/quicksort_p.c)

target_link_libraries(lab3 PRIVATE Threads::Threads)