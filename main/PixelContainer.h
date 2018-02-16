#pragma once

#include <vector>
#include "IPixelContainer.h"
#include "util.h"
#include "FormatFinder.h"

namespace directgraph {

    template <uint_fast32_t DispMode>
    class PixelContainer: public IPixelContainer {
    private:
        typedef FormatFinder<DispMode> FFinder;
        typedef typename FFinder::FormatType ContainerType;
        ContainerType *_buffer;
        Rectangle _coords;
        uint_fast32_t _maxWidth;
        uint_fast32_t _maxHeight;
        void updateCoords(uint_fast32_t x, uint_fast32_t y);
    public:
        PixelContainer(
                const std::vector<SinglePixel> &pixels,
                uint_fast32_t maxWidth, uint_fast32_t maxHeight
        );
        ~PixelContainer();

        void *getBuffer();

        Rectangle getCoords();

        void addPixel(uint_fast32_t x, uint_fast32_t y, uint_fast32_t color);

        uint_fast32_t getStartOffset();
        uint_fast32_t getNextLineOffset();
        uint_fast32_t getStride();
    };
}
