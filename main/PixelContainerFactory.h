#ifndef PROJECT_PIXELCONTAINERFACTORY_H
#define PROJECT_PIXELCONTAINERFACTORY_H

#include <utility>
#include "IPixelContainer.h"
#include "QueueItem.h"

namespace directgraph {
    class PixelContainerFactory {
        uint_fast32_t _maxWidth;
        uint_fast32_t _maxHeight;
        IPixelContainer::Format _format;
    public:
        struct ContainerOpt{
            bool containerCreated;
            IPixelContainer *container;
        };
        PixelContainerFactory(uint_fast32_t maxWidth, uint_fast32_t maxHeight, IPixelContainer::Format format);
        ContainerOpt tryGetContainer(
                uint_fast32_t firstX,
                uint_fast32_t firstY,
                uint_fast32_t firstColor,
                uint_fast32_t secondX,
                uint_fast32_t secondY,
                uint_fast32_t secondColor
        );
    };
}

#endif //PROJECT_PIXELCONTAINERFACTORY_H
