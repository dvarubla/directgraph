#pragma once

#include <utility>
#include "IPixelContainer.h"
#include "QueueItem.h"
#include "FormatFinder.h"

namespace directgraph {
    class PixelContainerFactory {
        uint_fast32_t _maxWidth;
        uint_fast32_t _maxHeight;
        ColorFormat::Format _format;
    public:
        struct ContainerOpt{
            bool containerCreated;
            IPixelContainer *container;
        };
        PixelContainerFactory(uint_fast32_t maxWidth, uint_fast32_t maxHeight, ColorFormat::Format format);
        ContainerOpt tryGetContainer(
                uint_fast32_t firstX,
                uint_fast32_t firstY,
                uint_fast32_t firstColor,
                uint_fast32_t secondX,
                uint_fast32_t secondY,
                uint_fast32_t secondColor
        );
        uint_fast32_t getMaxWidth();
        uint_fast32_t getMaxHeight();
    };
}
