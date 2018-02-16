#include "PixelContainerCreator.h"
#include "QueueItem.h"
#include "PixelContainer.h"
#include "WException.h"

namespace directgraph{

    PixelContainerCreator::PixelContainerCreator(
            uint_fast32_t maxWidth, uint_fast32_t maxHeight,
            ColorFormat::Format format
    ): _maxWidth(maxWidth), _maxHeight(maxHeight), _format(format), _container(NULL) {
    }

    uint_fast32_t PixelContainerCreator::getMaxWidth() {
        return _maxWidth;
    }

    uint_fast32_t PixelContainerCreator::getMaxHeight() {
        return _maxHeight;
    }

    IPixelContainer *PixelContainerCreator::getContainer() {
        return _container;
    }

    void PixelContainerCreator::addPixel(uint_fast32_t x, uint_fast32_t y, uint_fast32_t color) {
        if(_container == NULL){
            if(_pixels.size() == MAX_SINGLE_PIXELS){
                createContainer();
            } else {
                IPixelContainer::SinglePixel pixel = {x, y, color};
                _pixels.push_back(pixel);
            }
        }
        if(_container != NULL){
            _container->addPixel(x, y, color);
        }
    }

    void PixelContainerCreator::clear() {
        _pixels.clear();
        _container = NULL;
    }

    PixelContainerCreator::PixelVector::iterator PixelContainerCreator::pixelsBegin() {
        return _pixels.begin();
    }

    PixelContainerCreator::PixelVector::iterator PixelContainerCreator::pixelsEnd() {
        return _pixels.end();
    }

    bool PixelContainerCreator::havePixels() {
        return !_pixels.empty();
    }

    bool PixelContainerCreator::haveContainer() {
        return _container != NULL;
    }

    void PixelContainerCreator::createContainer() {
        switch(_format){
            case ColorFormat::A8R8G8B8:
                _container = new PixelContainer<ColorFormat::A8R8G8B8>(_pixels, _maxWidth, _maxHeight);
                break;
            case ColorFormat::AR5G5B5:
                _container = new PixelContainer<ColorFormat::AR5G5B5>(_pixels, _maxWidth, _maxHeight);
                break;
            default:
                THROW_EXC_CODE(WException, UNREACHABLE_CODE, L"Unknown format");
        }
        _pixels.clear();
    }

    PixelContainerCreator::~PixelContainerCreator() {
        delete _container;
    }
}