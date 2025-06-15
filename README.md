# skychess

chess-cpp-gui/ # ← project root (repo top)
├─ CMakeLists.txt ★ # master CMake script
├─ cmake/ # helper modules/macros
│ └─ FindSFML.cmake # (or FetchContent scripts, toolchains…)
│
├─ src/ # production source files
│ ├─ main.cpp ★
│ ├─ engine/ # pure-logic chess engine
│ │ ├─ Board.cpp
│ │ ├─ Move.cpp
│ │ └─ …
│ ├─ gui/ # SFML-specific rendering & input
│ │ ├─ Renderer.cpp
│ │ ├─ InputController.cpp
│ │ └─ …
│ └─ util/ # helpers (Timer, Logger, JSON wrapper…)
│
├─ include/ # public headers (installed / exported)
│ ├─ engine/
│ │ ├─ Board.hpp
│ │ ├─ Move.hpp
│ │ └─ …
│ ├─ gui/
│ │ ├─ Renderer.hpp
│ │ └─ …
│ └─ util/
│
├─ assets/ # runtime data shipped with the game
│ ├─ pieces/ # PNG spritesheets or SVGs
│ ├─ sounds/
│ ├─ fonts/
│ └─ themes/ # optional JSON / palette files
│
├─ tests/ # unit & integration tests
│ ├─ CMakeLists.txt
│ ├─ engine/
│ │ └─ BoardTests.cpp
│ └─ gui/
│
├─ docs/ # extra reference (design notes, UML, README images)
│ └─ architecture.md
│
├─ examples/ # tiny sample programs if you expose a lib
│ └─ perft.cpp
│
└─ .github/ # CI/CD (GitHub Actions)
└─ workflows/
└─ ci.yml

| File                    | Responsibility                                                                                            | Typical functions                                                       |
| ----------------------- | --------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------- |
| **MoveGenerator.cpp**   | Generates **pseudo-legal** moves for each piece type, then calls `isKingInCheck()` to prune illegal ones. | `std::vector<Move> generate(const GameState&)`                          |
| **GameState.cpp**       | Owns the `Board`, side-to-move, castling flags, half-move clock, Zobrist key history.                     | `bool make(const Move&)`, `void undo()`                                 |
| **FEN.cpp**             | Serialise/deserialise positions so you can save/restore games and write unit tests.                       | `GameState fromFen(std::string)`, `std::string toFen(const GameState&)` |
| **PGN.cpp**             | Write readable move list, optionally load annotated games.                                                | `void append(const Move&)`, `bool load(std::istream&)`                  |
| **Zobrist.cpp**         | Fast 64-bit hashes for repetition detection & transposition tables.                                       | `uint64_t hash(const GameState&)`                                       |
| **InputController.cpp** | Converts mouse clicks/drag into `Move` objects and sends them to the engine.                              | `void handleEvent(const sf::Event&, GameState&)`                        |
| **Animation.cpp**       | Smooth piece sliding, fade-out capture, square highlights.                                                | `void update(float dt)`, `void draw(sf::RenderTarget&)`                 |
| **Hud.cpp**             | Side panels: clocks, “New Game” button, move list.                                                        | `void draw(sf::RenderTarget&, const GameState&)`                        |
| **Theme.cpp**           | Load board/piece colours & sprite filenames from JSON.                                                    | `void apply(const std::string& themeName)`                              |
| **Sound.cpp**           | Simple wrapper around `sf::SoundBuffer` / `sf::Sound`                                                     | `void playMove()`, `void playCapture()`                                 |
| **TextureCache.cpp**    | Ensure each PNG loads once; share across Renderer & HUD.                                                  | `const sf::Texture& get(const std::string& id)`                         |
| **Logger.cpp**          | `LOG_INFO`, `LOG_WARN`, toggled by build-type.                                                            |                                                                         |
| **Timer.cpp**           | `double seconds()`, `void start() / stop()`—handy for perft speed.                                        |                                                                         |
| **Config.cpp**          | Load/save JSON to `~/.config/skychess/config.json`.                                                       |                                                                         |
| **FileSystem.cpp**      | Finds `assets/` no matter where the EXE is launched from.                                                 |                                                                         |
