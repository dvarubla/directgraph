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
                int32_t left, top, right, bottom;
            } bar;
            struct FillEllipseItem{
                int32_t x, y;
                uint32_t xradius, yradius;
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
        static QueueItem create(uint8_t fillStyle, uint_fast32_t color);
        template <QueueItem::QueueItemType T>
        static QueueItem create(const char *fillPattern, uint_fast32_t color);
        template <QueueItem::QueueItemType T>
        static QueueItem create(uint_fast32_t color);
        template <QueueItem::QueueItemType T>
        static QueueItem create(int_fast32_t, int_fast32_t, int_fast32_t, int_fast32_t);
        template <QueueItem::QueueItemType T>
        static QueueItem create(int_fast32_t, int_fast32_t, uint_fast32_t, uint_fast32_t);
        template <QueueItem::QueueItemType T>
        static QueueItem create(uint_fast32_t, uint_fast32_t , uint_fast32_t );
        template <QueueItem::QueueItemType T>
        static QueueItem create(IPixelContainer *pixelContainer);
    };
}
