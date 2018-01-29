#pragma once

#include <stdint.h>
#include "util.h"

namespace directgraph {
    class IPixelContainer {
    public:
        virtual void* getBuffer() = 0;
        virtual uint_fast32_t getStartOffset() = 0;
        virtual uint_fast32_t getNextLineOffset() = 0;
        virtual Rectangle getFirstCoords() = 0;
        virtual Rectangle getLastCoords() = 0;
        virtual uint_fast32_t getStride() = 0;
        virtual uint_fast32_t getLastStride() = 0;
        virtual uint_fast32_t getHeight() = 0;
        virtual bool tryAddPixel(uint_fast32_t x, uint_fast32_t y, uint_fast32_t color) = 0;
        virtual ~IPixelContainer(){}
    };
}
