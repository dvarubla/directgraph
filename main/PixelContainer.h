#ifndef PROJECT_PIXELCONTAINER_H
#define PROJECT_PIXELCONTAINER_H

#include "IPixelContainer.h"

namespace directgraph {
    template <uint_fast32_t DispMode>
    class FormatFinder{
    };

    template <>
    class FormatFinder<IPixelContainer::R8G8B8>{
    public:
        typedef uint32_t ContainerType;
        static ContainerType convert(uint32_t color){
            return color;
        }
    };

    template <uint_fast32_t DispMode>
    class PixelContainer: public IPixelContainer {
    private:
        enum Direction{
            NO_DIRECTION,
            LEFT_RIGHT_TOP_DOWN,
            LEFT_RIGHT_BOTTOM_UP
        } _direction;
        typedef FormatFinder<DispMode> FFinder;
        typedef typename FFinder::ContainerType ContainerType;
        ContainerType *_buffer;
        uint_fast32_t _firstX;
        uint_fast32_t _firstY;
        uint_fast32_t _lastX;
        uint_fast32_t _lastY;
        uint_fast32_t _maxWidth;
        uint_fast32_t _maxHeight;
        uint_fast32_t _width;
        uint_fast32_t _height;
        uint_fast32_t _lastWidth;
        void setPixel(uint_fast32_t x, uint_fast32_t y, uint_fast32_t color);
    public:
        PixelContainer(
                uint_fast32_t firstX, uint_fast32_t firstY, uint_fast32_t firstColor,
                uint_fast32_t secondX, uint_fast32_t secondY, uint_fast32_t secondColor,
                uint_fast32_t maxWidth, uint_fast32_t maxHeight
        );
        ~PixelContainer();

        void *getBuffer();

        uint_fast32_t getLeft();

        uint_fast32_t getTop();

        uint_fast32_t getRight();

        uint_fast32_t getBottom();

        uint_fast32_t getStride();

        uint_fast32_t getLastStride();

        uint_fast32_t getHeight();

        uint_fast32_t getStartOffset();

        uint_fast32_t getNextLineOffset();

        bool tryAddPixel(uint_fast32_t x, uint_fast32_t y, uint_fast32_t color);
    };
}

#endif //PROJECT_PIXELCONTAINER_H
