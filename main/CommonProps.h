#pragma once

#include <stdint.h>

namespace directgraph {
    struct CommonProps {
        uint_fast8_t lineStyle;
        uint_fast8_t fillStyle;
        uint_fast32_t fillColor;
        uint_fast32_t bgColor;
        char *userFillPattern;
    };

    CommonProps get_default_common_props();
}
