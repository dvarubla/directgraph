#pragma once

#include <stdint.h>
#include "IPixelContainer.h"

namespace directgraph{
    struct QueueItem{
        enum QueueItemType{
            CLEAR = 0,
            BGCOLOR,
            SETFILLSTYLE,
            BAR,
            SINGLE_PIXEL,
            PIXEL_CONTAINER,
            SETFILLPATTERN,
            SETLINESTYLE,
            FILLELLIPSE
        } type;
        union{
            uint32_t bgColor;
            struct FillStyleItem{
                uint8_t fillStyle;
                uint32_t color;
            } setfillstyle;
            struct FillPatternItem{
                char *fillPattern;
                uint32_t color;
            } setfillpattern;
            struct LineStyleItem{
                uint8_t linestyle;
                uint32_t pattern;
                uint32_t thickness;
            } setlinestyle;
            struct BarItem{
                float left, top, right, bottom;
            } bar;
            struct FillEllipseItem{
                float x, y, xradius, yradius;
            } fillellipse;
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
        static QueueItem create(const char *fillPattern, uint32_t color);
        template <QueueItem::QueueItemType T>
        static QueueItem create(uint32_t color);
        template <QueueItem::QueueItemType T>
        static QueueItem create(float, float, float, float);
        template <QueueItem::QueueItemType T>
        static QueueItem create(uint32_t, uint32_t , uint32_t );
        template <QueueItem::QueueItemType T>
        static QueueItem create(IPixelContainer *pixelContainer);
    };
}
