#include <algorithm>
#include "PixelContainer.h"
#include "WException.h"

#undef max
#undef min

namespace directgraph{

    template<uint_fast32_t DispMode>
    PixelContainer<DispMode>::PixelContainer(
            const std::vector<SinglePixel> &pixels,
            uint_fast32_t maxWidth, uint_fast32_t maxHeight
    ): _maxWidth(maxWidth), _maxHeight(maxHeight)
    {
        const Rectangle t = {pixels[0].x, pixels[0].y, pixels[0].x, pixels[0].y};
        _coords = t;
        try{
            _buffer = new ContainerType[maxWidth * maxHeight];
            std::fill(_buffer, _buffer + maxWidth * maxHeight, 0);
        } catch(const std::bad_alloc &) {
            THROW_EXC_CODE(WException, CANT_ALLOC, std::wstring(L"Can't allocate pixel container buffer"));
        }
        for(std::vector<SinglePixel>::const_iterator it = pixels.begin(); it != pixels.end(); ++it){
            addPixel(it->x, it->y, it->color);
        }
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
    void PixelContainer<DispMode>::addPixel(uint_fast32_t x, uint_fast32_t y, uint_fast32_t color) {
        _buffer[y * _maxWidth + x] = FFinder::convert(color, false);
        updateCoords(x, y);
    }

    template<uint_fast32_t DispMode>
    Rectangle PixelContainer<DispMode>::getCoords() {
        Rectangle res = _coords;
        res.bottom++;
        res.right++;
        return res;
    }

    template<uint_fast32_t DispMode>
    void PixelContainer<DispMode>::updateCoords(uint_fast32_t x, uint_fast32_t y) {
        _coords.left = std::min(_coords.left, x);
        _coords.top = std::min(_coords.top, y);
        _coords.right = std::max(_coords.right, x);
        _coords.bottom = std::max(_coords.bottom, y);
    }

    template<uint_fast32_t DispMode>
    uint_fast32_t PixelContainer<DispMode>::getStartOffset() {
        return (_coords.top * _maxWidth + _coords.left) * sizeof(ContainerType);
    }

    template<uint_fast32_t DispMode>
    uint_fast32_t PixelContainer<DispMode>::getNextLineOffset() {
        return _maxWidth * sizeof(ContainerType);
    }

    template<uint_fast32_t DispMode>
    uint_fast32_t PixelContainer<DispMode>::getStride() {
        return (_coords.right - _coords.left + 1) * sizeof(ContainerType);
    }

    template class PixelContainer<ColorFormat::A8R8G8B8>;
    template class PixelContainer<ColorFormat::AR5G5B5>;
}