#include "PixelContainer.h"
#include "WException.h"

namespace directgraph{

    template<uint_fast32_t DispMode>
    PixelContainer<DispMode>::PixelContainer(
            uint_fast32_t firstX, uint_fast32_t firstY, uint_fast32_t firstColor,
            uint_fast32_t secondX, uint_fast32_t secondY, uint_fast32_t secondColor,
            uint_fast32_t maxWidth, uint_fast32_t maxHeight
    ): _direction(NO_DIRECTION),
       _firstX(firstX), _firstY(firstY), _lastX(secondX), _lastY(secondY),
       _maxWidth(maxWidth), _maxHeight(maxHeight)
    {
        try{
            _buffer = new ContainerType[maxWidth * maxHeight];
        } catch(const std::bad_alloc &) {
            THROW_EXC_CODE(WException, CANT_ALLOC, std::wstring(L"Can't allocate pixel container buffer"));
        }
        if(firstX == secondX){
            if(secondY > firstY){
                _direction = LEFT_RIGHT_TOP_DOWN;
            } else {
                _direction = LEFT_RIGHT_BOTTOM_UP;
            }
            _height = 2;
            _width = 1;
            _lastWidth = 1;
        } else {
            _height = 1;
            _width = 2;
            _lastWidth = 2;
        }
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
    Rectangle PixelContainer<DispMode>::getFirstCoords() {
        Rectangle result;
        result.left = _firstX;
        result.right = _firstX + _width - 1;
        if(_direction == LEFT_RIGHT_TOP_DOWN){
            result.top = _firstY;
            result.bottom = (_lastWidth == _width) ? _lastY : _lastY - 1;
        } else {
            result.top = (_lastWidth == _width) ? _lastY : _lastY + 1;
            result.bottom = _firstY;
        }
        return result;
    }

    template<uint_fast32_t DispMode>
    Rectangle PixelContainer<DispMode>::getLastCoords() {
        Rectangle result;
        result.left = _firstX;
        result.right = _lastX;
        result.top = result.bottom = _lastY;
        return result;
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
        uint_fast32_t top = (_direction == LEFT_RIGHT_TOP_DOWN) ? _firstY : _lastY;
        return (top * _maxWidth + _firstX) * sizeof(ContainerType);
    }

    template<uint_fast32_t DispMode>
    uint_fast32_t PixelContainer<DispMode>::getNextLineOffset() {
        return _maxWidth * sizeof(ContainerType);
    }

    template<uint_fast32_t DispMode>
    bool PixelContainer<DispMode>::tryAddPixel(uint_fast32_t x, uint_fast32_t y, uint_fast32_t color) {
        bool status = true;
        if(_lastY == y && (_lastX + 1) == x && (_height == 1 || _lastWidth < _width)){
            _lastX++;
            if(_height == 1) {
                _width++;
            }
            _lastWidth++;
        } else if(_firstX == x && _lastWidth == _width){
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
                    _lastWidth = 1;
                    _lastX = _firstX;
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
    template class PixelContainer<IPixelContainer::R5G6B5>;
}