cmake_minimum_required(VERSION 3.25)
project(chess_gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ——— Enable AddressSanitizer globally ————————————————————————
# this will add -fsanitize=address to both compile and link lines
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-g -fsanitize=address)
  add_link_options   (-fsanitize=address)
endif()

# ——— Find SFML 3 ——————————————————————————————————————————————
find_package(SFML 3 REQUIRED COMPONENTS Window Graphics)

# ——— Engine library ————————————————————————————————————————
add_library(engine
    src/engine/Board.cpp
    src/engine/Move.cpp
    src/engine/MoveGenerator.cpp
)
target_include_directories(engine PUBLIC include)

# ——— GUI library (renders and handles input) ————————————————————
add_library(gui
    src/gui/Renderer.cpp
    src/engine/Controller.cpp
)
target_include_directories(gui PUBLIC include)
target_link_libraries(gui
  PUBLIC
    engine
    SFML::Window
    SFML::Graphics
)

# ——— Main executable ————————————————————————————————————————
add_executable(chess_gui
    src/main.cpp
)
# by linking PUBLIC to gui, chess_gui inherits gui’s include dirs
target_link_libraries(chess_gui
  PUBLIC
    gui
)

# ----------------------------- Catch2 & Tests -----------------------------

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.3
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

add_library(test_main OBJECT
  test/TestMain.cpp
)
target_link_libraries(test_main PUBLIC Catch2::Catch2WithMain)

include(Catch)

add_executable(chess_tests
  test/BoardTests.cpp
  test/GeneratorTests.cpp
  test/MoveTests.cpp
)
target_link_libraries(chess_tests
  PRIVATE
    engine
    gui
    Catch2::Catch2WithMain
)
catch_discover_tests(chess_tests)

