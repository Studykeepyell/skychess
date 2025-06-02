
cmake_minimum_required(VERSION 3.25)
project(chess_gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)
add_library(engine
    src/engine/Board.cpp)

target_include_directories(engine PUBLIC include)

add_library(gui
    src/gui/Renderer.cpp)

target_include_directories(gui PUBLIC include)
target_link_libraries(gui PUBLIC engine sfml-graphics sfml-window sfml-system)

add_executable(chess_gui src/main.cpp)
target_link_libraries(chess_gui PRIVATE gui)
