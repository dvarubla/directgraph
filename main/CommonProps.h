#pragma once

#include <stdint.h>
#include "util.h"

namespace directgraph {
    struct CommonProps {
        uint_fast8_t lineStyle;
        uint_fast8_t fillStyle;
        uint_fast32_t fillColor;
        uint_fast32_t bgColor;
        uint_fast32_t drawColor;
        uint_fast32_t lineThickness;
        uint_fast16_t userLinePattern;
        char *userFillPattern;
        Coords curPos;
    };

    CommonProps get_default_common_props();
}
