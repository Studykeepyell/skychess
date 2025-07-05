
cmake_minimum_required(VERSION 3.25)
project(chess_gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML 3 REQUIRED COMPONENTS Window Graphics)

add_library(engine
    src/engine/Board.cpp
    src/engine/Move.cpp
    src/engine/MoveGenerator.cpp)

target_include_directories(engine PUBLIC include)

add_library(gui
    src/gui/Renderer.cpp
src/engine/Controller.cpp)


target_include_directories(gui PUBLIC include)
target_link_libraries(gui PUBLIC
  engine
  SFML::Window
  SFML::Graphics
)

add_executable(chess_gui src/main.cpp)
target_link_libraries(chess_gui PUBLIC gui)
target_include_directories(chess_gui
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

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
test/BoardTests.cpp
test/GeneratorTests.cpp
test/MoveTests.cpp)

target_link_libraries(chess_tests PRIVATE
engine gui
Catch2::Catch2WithMain)

catch_discover_tests(chess_tests)
