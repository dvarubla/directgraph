#pragma once

#include <stdint.h>
#include <directgraph/directgraph_constants.h>

namespace directgraph{
    const static int NUM_STANDARD_FPATTERNS = 10;
    const static int FPATTERN_SIZE = 8;
    const static int FIRST_FPATTERN = LINE_FILL;
    extern const uint8_t fillPatterns[NUM_STANDARD_FPATTERNS][FPATTERN_SIZE];
}

