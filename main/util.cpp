#include "util.h"

namespace directgraph{
    uint_fast32_t swap_color(uint_fast32_t color) {
        return (color & 0x00FF00) | ((color & 0xFF) << 16) | ((color & 0xFF0000) >> 16);
    }
}