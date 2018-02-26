cmake_minimum_required(VERSION 3.9)
project(sdl)

find_package(SDL2 REQUIRED)

SET(CMAKE_CXX_STANDARD 17)

# include_directories(include)
include_directories(${SDL2_INCLUDE_DIRECTORIES})
add_executable(sdl src/main.cpp)
target_link_libraries(sdl ${SDL2_LIBRARIES})

# add_custom_command(TARGET sdl POST_BUILD COMMAND "./sdl")
