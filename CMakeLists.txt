cmake_minimum_required(VERSION 3.16)
project(2dplatformer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
add_executable(2dplatformer
    src/main.cpp
    src/Game.cpp
    src/Input.cpp
    src/Player.cpp
    src/Level.cpp
)

target_include_directories(2dplatformer PRIVATE src)
target_link_libraries(2dplatformer PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)
target_compile_definitions(2dplatformer PRIVATE SDL_MAIN_HANDLED)
