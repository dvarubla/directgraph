#include "PixelContainerFactory.h"
#include "QueueItem.h"
#include "PixelContainer.h"

namespace directgraph{

    PixelContainerFactory::PixelContainerFactory(
            uint_fast32_t maxWidth, uint_fast32_t maxHeight,
            IPixelContainer::Format format
    ): _maxWidth(maxWidth), _maxHeight(maxHeight), _format(format) {
    }

    PixelContainerFactory::ContainerOpt
    PixelContainerFactory::tryGetContainer(
            uint_fast32_t firstX, uint_fast32_t firstY,
            uint_fast32_t firstColor,
            uint_fast32_t secondX, uint_fast32_t secondY,
            uint_fast32_t secondColor
    ) {
        ContainerOpt result;
        if(firstY == secondY && firstX + 1 == secondX){
            result.containerCreated = true;
            switch(_format){
                case IPixelContainer::R8G8B8:
                    result.container = new PixelContainer<IPixelContainer::R8G8B8>(
                            firstX, firstY, firstColor,
                            secondX, secondY, secondColor,
                            _maxWidth, _maxHeight
                    );
                default: break;
            }
        } else {
            result.containerCreated = false;
        }
        return result;
    }
}