#pragma once

#include <utility>
#include <vector>
#include "IPixelContainer.h"
#include "QueueItem.h"
#include "FormatFinder.h"

namespace directgraph {
    class PixelContainerCreator {
    public:
        typedef std::vector<IPixelContainer::SinglePixel> PixelVector;
    private:
        const static uint_fast8_t MAX_SINGLE_PIXELS = 10;
        uint_fast32_t _maxWidth;
        uint_fast32_t _maxHeight;
        ColorFormat::Format _format;
        PixelVector _pixels;
        IPixelContainer *_container;
        void createContainer();
    public:
        PixelContainerCreator(uint_fast32_t maxWidth, uint_fast32_t maxHeight, ColorFormat::Format format);
        ~PixelContainerCreator();
        IPixelContainer * getContainer();
        void addPixel(uint_fast32_t x, uint_fast32_t y, uint_fast32_t color);
        uint_fast32_t getMaxWidth();
        uint_fast32_t getMaxHeight();
        PixelVector::iterator pixelsBegin();
        PixelVector::iterator pixelsEnd();
        void clear();
        bool havePixels();
        bool haveContainer();
    };
}
