
cmake_minimum_required(VERSION 3.20)
project(Grapher C)

set(CMAKE_C_STANDARD 17)

# Find SDL3 package (installed via package manager or from source)
find_package(SDL3 REQUIRED CONFIG)

add_executable(grapher
    src/main.c
)

# Link your executable against SDL3::SDL3
target_link_libraries(grapher PRIVATE SDL3::SDL3)
