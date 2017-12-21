#include "PixelContainer.h"

namespace directgraph{

    template<uint_fast32_t DispMode>
    PixelContainer<DispMode>::PixelContainer(
            uint_fast32_t firstX, uint_fast32_t firstY, uint_fast32_t firstColor,
            uint_fast32_t secondX, uint_fast32_t secondY, uint_fast32_t secondColor,
            uint_fast32_t maxWidth, uint_fast32_t maxHeight
    ): _direction(NO_DIRECTION),
       _buffer(new ContainerType[maxWidth * maxHeight]),
       _firstX(firstX), _firstY(firstY), _lastX(secondX), _lastY(secondY),
       _maxWidth(maxWidth), _maxHeight(maxHeight),
       _width(2), _height(1), _lastWidth(2)
    {
        setPixel(firstX, firstY, firstColor);
        setPixel(secondX, secondY, secondColor);
    }

    template<uint_fast32_t DispMode>
    PixelContainer<DispMode>::~PixelContainer() {
        delete [] _buffer;
    }

    template<uint_fast32_t DispMode>
    void *PixelContainer<DispMode>::getBuffer() {
        return _buffer;
    }

    template<uint_fast32_t DispMode>
    uint_fast32_t PixelContainer<DispMode>::getLeft() {
        return _firstX;
    }

    template<uint_fast32_t DispMode>
    uint_fast32_t PixelContainer<DispMode>::getTop() {
        if(_direction == LEFT_RIGHT_TOP_DOWN){
            return _firstY;
        } else {
            return _lastY;
        }
    }

    template<uint_fast32_t DispMode>
    uint_fast32_t PixelContainer<DispMode>::getRight() {
        return _lastX;
    }

    template<uint_fast32_t DispMode>
    uint_fast32_t PixelContainer<DispMode>::getBottom() {
        if(_direction == LEFT_RIGHT_TOP_DOWN){
            return _lastY;
        } else {
            return _firstY;
        }
    }

    template<uint_fast32_t DispMode>
    uint_fast32_t PixelContainer<DispMode>::getStride() {
        return _width * sizeof(ContainerType);
    }

    template<uint_fast32_t DispMode>
    uint_fast32_t PixelContainer<DispMode>::getLastStride() {
        return _lastWidth * sizeof(ContainerType);
    }

    template<uint_fast32_t DispMode>
    uint_fast32_t PixelContainer<DispMode>::getHeight() {
        return _height;
    }

    template<uint_fast32_t DispMode>
    uint_fast32_t PixelContainer<DispMode>::getStartOffset() {
        return (getTop() * _width + getLeft()) * sizeof(ContainerType);
    }

    template<uint_fast32_t DispMode>
    uint_fast32_t PixelContainer<DispMode>::getNextLineOffset() {
        return _maxWidth * sizeof(ContainerType);
    }

    template<uint_fast32_t DispMode>
    bool PixelContainer<DispMode>::tryAddPixel(uint_fast32_t x, uint_fast32_t y, uint_fast32_t color) {
        if(x > _maxWidth || y > _maxHeight){
            return true;
        }
        bool status = true;
        if(_lastY == y && (_lastX + 1) == x && (_height == 1 || _lastWidth < _width)){
            _lastX++;
            if(_height == 1) {
                _width++;
            }
            _lastWidth++;
        } else if(_firstX == x && _lastWidth == _width){
            _lastWidth = 1;
            _lastX = _firstX;
            Direction curDirection;
            if(_lastY + 1== y){
                curDirection = LEFT_RIGHT_TOP_DOWN;
            } else if(_lastY == y + 1){
                curDirection = LEFT_RIGHT_BOTTOM_UP;
            } else {
                status = false;
            }
            if(status) {
                if (_direction == NO_DIRECTION) {
                    _direction = curDirection;
                } else if (_direction != curDirection) {
                    status = false;
                }
                if(status){
                    if(_direction == LEFT_RIGHT_TOP_DOWN){
                        _lastY++;
                    } else {
                        _lastY--;
                    }
                    _height++;
                }
            }
        } else {
            status = false;
        }
        if(status){
            setPixel(x, y, color);
        }
        return status;
    }

    template<uint_fast32_t DispMode>
    void PixelContainer<DispMode>::setPixel(uint_fast32_t x, uint_fast32_t y, uint_fast32_t color) {
        _buffer[y * _maxWidth + x] = FFinder::convert(color);
    }

    template class PixelContainer<IPixelContainer::R8G8B8>;
}