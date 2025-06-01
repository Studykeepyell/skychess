# skychess



chess-cpp-gui/                  # ← project root (repo top)
├─ CMakeLists.txt ★            # master CMake script
├─ cmake/                      # helper modules/macros
│  └─ FindSFML.cmake           # (or FetchContent scripts, toolchains…)
│
├─ src/                        # production source files
│  ├─ main.cpp ★
│  ├─ engine/                  # pure-logic chess engine
│  │  ├─ Board.cpp
│  │  ├─ Move.cpp
│  │  └─ …
│  ├─ gui/                     # SFML-specific rendering & input
│  │  ├─ Renderer.cpp
│  │  ├─ InputController.cpp
│  │  └─ …
│  └─ util/                    # helpers (Timer, Logger, JSON wrapper…)
│
├─ include/                    # public headers (installed / exported)
│  ├─ engine/
│  │  ├─ Board.hpp
│  │  ├─ Move.hpp
│  │  └─ …
│  ├─ gui/
│  │  ├─ Renderer.hpp
│  │  └─ …
│  └─ util/
│
├─ assets/                     # runtime data shipped with the game
│  ├─ pieces/                  # PNG spritesheets or SVGs
│  ├─ sounds/
│  ├─ fonts/
│  └─ themes/                  # optional JSON / palette files
│
├─ tests/                      # unit & integration tests
│  ├─ CMakeLists.txt
│  ├─ engine/
│  │  └─ BoardTests.cpp
│  └─ gui/
│
├─ docs/                       # extra reference (design notes, UML, README images)
│  └─ architecture.md
│
├─ examples/                   # tiny sample programs if you expose a lib
│  └─ perft.cpp
│
└─ .github/                    # CI/CD (GitHub Actions)
   └─ workflows/
      └─ ci.yml
