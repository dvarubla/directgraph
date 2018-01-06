#pragma once

#include <stdint.h>
#include "IPixelContainer.h"

namespace directgraph{
    struct QueueItem{
        enum QueueItemType{
            CLEAR = 0,
            SETFILLSTYLE,
            BAR,
            SINGLE_PIXEL,
            PIXEL_CONTAINER
        } type;
        union{
            struct FillStyleItem{
                uint8_t fillStyle;
                uint32_t color;
            } setfillstyle;
            struct BarItem{
                float left, top, right, bottom;
            } bar;
            struct SinglePixelItem{
                uint32_t x, y;
                uint32_t color;
            } singlePixel;
            IPixelContainer *pixelContainer;
        } data;
    };

    class QueueItemCreator{
    public:
        template <QueueItem::QueueItemType T>
        static QueueItem create();
        template <QueueItem::QueueItemType T>
        static QueueItem create(uint8_t fillStyle, uint32_t color);
        template <QueueItem::QueueItemType T>
        static QueueItem create(float left, float top, float right, float bottom);
        template <QueueItem::QueueItemType T>
        static QueueItem create(uint32_t x, uint32_t y, uint32_t color);
        template <QueueItem::QueueItemType T>
        static QueueItem create(IPixelContainer *pixelContainer);
    };
}
