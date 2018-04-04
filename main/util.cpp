#include "util.h"

namespace directgraph{
    uint_fast32_t swap_color(uint_fast32_t color) {
        return (color & 0x00FF00) | ((color & 0xFF) << 16) | ((color & 0xFF0000) >> 16);
    }

    uint_fast32_t swap_color_transp(uint_fast32_t color) {
        return (0xFF000000u - (color & 0xFF000000u)) | swap_color(color);
    }

    uint_fast32_t color_remove_alpha(uint_fast32_t color) {
        return color & 0xFFFFFF;
    }

    bool color_has_alpha(uint_fast32_t color) {
        return (color & 0xFF000000) != 0;
    }
}