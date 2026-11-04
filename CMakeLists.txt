cmake_minimum_required(VERSION 3.26)
project(main C CXX)

find_package(SDL3 REQUIRED)

add_executable(main)

target_sources(main
PRIVATE
    src/main.c
    gfx/graphics.c
    cam/camera.c
    coms/commons.c
)

target_include_directories(main PRIVATE
    .
    src
    gfx
    cam
    coms
)

target_link_libraries(main PRIVATE SDL3::SDL3)