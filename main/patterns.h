#pragma once

#include <stdint.h>
#include <directgraph/directgraph_constants.h>

namespace directgraph{
    const static int NUM_STANDARD_FPATTERNS = 10;
    const static int FPATTERN_SIZE = 8;
    const static int FIRST_FPATTERN = LINE_FILL;
    extern const uint8_t fillPatterns[NUM_STANDARD_FPATTERNS][FPATTERN_SIZE];

    const static int NUM_STANDARD_LPATTERNS = 3;
    const static int FIRST_LPATTERN = DOTTED_LINE;
    const static int LPATTERN_SIZE = 16;
    extern const uint16_t linePatterns[NUM_STANDARD_LPATTERNS];
}

