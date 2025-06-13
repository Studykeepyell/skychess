
cmake_minimum_required(VERSION 3.25)
project(chess_gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML REQUIRED COMPONENTS system window graphics)


add_library(engine
    src/engine/Board.cpp)

target_include_directories(engine PUBLIC include)

add_library(gui
    src/gui/Renderer.cpp)

target_include_directories(gui PUBLIC include)
target_link_libraries(gui PUBLIC engine sfml-graphics sfml-window sfml-system)

add_executable(chess_gui src/main.cpp)
target_link_libraries(chess_gui PRIVATE gui)

# ----------------------------- Catch2 --------------------------------

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.3
)
FetchContent_MakeAvailable(Catch2)

enable_testing()
add_library(test_main OBJECT
test/TestMain.cpp)
target_link_libraries(test_main PUBLIC Catch2::Catch2WithMain)


include(Catch)

#------------unit test executable---------------------#

add_executable(chess_tests
tests/BoardTests.cpp
tests/GeneratorTests.cpp
MoveTests.cpp)

target_link_libraries(chess_tests PRIVATE
engine gui
Catch2::Catch2)

catch_discover_tests(chess_tests)
