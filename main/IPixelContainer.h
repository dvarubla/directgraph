#ifndef PROJECT_IPIXELCONTAINER_H
#define PROJECT_IPIXELCONTAINER_H

#include <stdint.h>

namespace directgraph {
    class IPixelContainer {
    public:
        enum Format{
            R5G6B5,
            R8G8B8
        };
        virtual void* getBuffer() = 0;
        virtual uint_fast32_t getStartOffset() = 0;
        virtual uint_fast32_t getNextLineOffset() = 0;
        virtual uint_fast32_t getLeft() = 0;
        virtual uint_fast32_t getTop() = 0;
        virtual uint_fast32_t getRight() = 0;
        virtual uint_fast32_t getBottom() = 0;
        virtual uint_fast32_t getStride() = 0;
        virtual uint_fast32_t getLastStride() = 0;
        virtual uint_fast32_t getHeight() = 0;
        virtual bool tryAddPixel(uint_fast32_t x, uint_fast32_t y, uint_fast32_t color) = 0;
        virtual ~IPixelContainer(){}
    };
}

#endif //PROJECT_IPIXELCONTAINER_H
