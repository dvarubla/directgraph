#ifndef PROJECT_UTIL_H
#define PROJECT_UTIL_H

#include <stdint.h>

namespace directgraph{
    uint_fast32_t swap_color(uint_fast32_t color);

    struct Rectangle{
        uint_fast32_t left;
        uint_fast32_t top;
        uint_fast32_t right;
        uint_fast32_t bottom;
    };
}

#endif //PROJECT_UTIL_H
