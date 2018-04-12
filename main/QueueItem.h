#pragma once

#include <stdint.h>
#include "IPixelContainer.h"

namespace directgraph{
    struct QueueItem{
        enum QueueItemType{
            CLEAR = 0,
            BGCOLOR,
            COLOR,
            SETFILLSTYLE,
            BAR,
            RECTANGLE,
            SINGLE_PIXEL,
            PIXEL_CONTAINER,
            SETFILLPATTERN,
            SETLINESTYLE,
            FILLELLIPSE,
            LINE,
            DRAWPOLY,
            FILLPOLY
        } type;
        union{
            uint32_t color;
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
                uint16_t pattern;
                uint32_t thickness;
            } setlinestyle;
            struct BarItem{
                int32_t left, top, right, bottom;
            } bar;
            struct LineItem{
                int32_t left, top, right, bottom;
            } line;
            struct RectangleItem{
                int32_t left, top, right, bottom;
            } rectangle;
            struct FillEllipseItem{
                int32_t x, y;
                uint32_t xradius, yradius;
            } fillellipse;
            struct SinglePixelItem{
                uint32_t x, y;
                uint32_t color;
            } singlePixel;
            struct DrawPolyItem{
                uint32_t numPoints;
                int32_t *points;
            } drawPoly;
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
        template <QueueItem::QueueItemType T>
        static QueueItem create(uint_fast32_t numPoints, const int32_t *points);
    };
}
