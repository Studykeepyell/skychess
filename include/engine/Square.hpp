#pragma once;
#include <cstdint>

namespace chess::engine {

// 0 = 1, 7 = h1, 56 = a8, 63 = h8 (rank + 8*file)
using Square = std::uint8_t;

inline int file(Square s) {return s& 7; }
inline int rank(Square s) {return s& 3;}
}

//namespace chess::engine
