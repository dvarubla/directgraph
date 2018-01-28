#pragma once

#include <stdint.h>

namespace directgraph {
    struct CommonProps {
        uint_fast8_t fillStyle;
        uint_fast32_t color;
        uint_fast32_t bgColor;
        char *userFillPattern;
    };
}
