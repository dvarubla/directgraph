#pragma once

#include <stdint.h>
#include "util.h"

namespace directgraph {
    class IPixelContainer {
    public:
        struct SinglePixel{
            uint_fast32_t x, y;
            uint_fast32_t color;
        };
        virtual void *getBuffer() = 0;
        virtual Rectangle getCoords() = 0;
        virtual void addPixel(uint_fast32_t x, uint_fast32_t y, uint_fast32_t color) = 0;
        virtual uint_fast32_t getStartOffset() = 0;
        virtual uint_fast32_t getNextLineOffset() = 0;
        virtual uint_fast32_t getStride() = 0;
        virtual ~IPixelContainer(){}
    };
}
