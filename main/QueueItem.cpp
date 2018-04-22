#include <cstring>
#include <algorithm>
#include "QueueItem.h"
#include "patterns.h"

namespace directgraph{
    template<>
    QueueItem QueueItemCreator::create<QueueItem::SETFILLSTYLE>(uint8_t fillStyle, uint_fast32_t color) {
        QueueItem item;
        item.type = QueueItem::SETFILLSTYLE;
        item.data.setfillstyle.fillStyle = fillStyle;
        item.data.setfillstyle.color = color;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::BAR>(int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom) {
        QueueItem item;
        item.type = QueueItem::BAR;
        item.data.bar.left = left;
        item.data.bar.top = top;
        item.data.bar.right = right;
        item.data.bar.bottom = bottom;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::RECTANGLE>(int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom) {
        QueueItem item;
        item.type = QueueItem::RECTANGLE;
        item.data.rectangle.left = left;
        item.data.rectangle.top = top;
        item.data.rectangle.right = right;
        item.data.rectangle.bottom = bottom;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::LINE>(int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom) {
        QueueItem item;
        item.type = QueueItem::LINE;
        item.data.line.left = left;
        item.data.line.top = top;
        item.data.line.right = right;
        item.data.line.bottom = bottom;
        return item;
    }
    
    template<>
    QueueItem QueueItemCreator::create<QueueItem::FILLELLIPSE>(
            int_fast32_t x, int_fast32_t y,
            uint_fast32_t xradius, uint_fast32_t yradius,
            uint_fast32_t startAngle, uint_fast32_t endAngle
    ) {
        QueueItem item;
        item.type = QueueItem::FILLELLIPSE;
        item.data.ellipse.x = x;
        item.data.ellipse.y = y;
        item.data.ellipse.xradius = xradius;
        item.data.ellipse.yradius = yradius;
        item.data.ellipse.startAngle = static_cast<uint_fast8_t>(startAngle);
        item.data.ellipse.endAngle = static_cast<uint_fast8_t>(endAngle);
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::ELLIPSE>(
            int_fast32_t x, int_fast32_t y,
            uint_fast32_t xradius, uint_fast32_t yradius,
            uint_fast32_t startAngle, uint_fast32_t endAngle
    ) {
        QueueItem item;
        item.type = QueueItem::ELLIPSE;
        item.data.ellipse.x = x;
        item.data.ellipse.y = y;
        item.data.ellipse.xradius = xradius;
        item.data.ellipse.yradius = yradius;
        item.data.ellipse.startAngle = static_cast<uint_fast8_t>(startAngle);
        item.data.ellipse.endAngle = static_cast<uint_fast8_t>(endAngle);
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::FILLELLIPSE>(
            int_fast32_t x, int_fast32_t y,
            uint_fast32_t xradius, uint_fast32_t yradius
    ) {
        QueueItem item;
        item.type = QueueItem::FILLELLIPSE;
        item.data.ellipse.x = x;
        item.data.ellipse.y = y;
        item.data.ellipse.xradius = xradius;
        item.data.ellipse.yradius = yradius;
        item.data.ellipse.startAngle = 0;
        item.data.ellipse.endAngle = 360;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::ELLIPSE>(
            int_fast32_t x, int_fast32_t y,
            uint_fast32_t xradius, uint_fast32_t yradius
    ) {
        QueueItem item;
        item.type = QueueItem::ELLIPSE;
        item.data.ellipse.x = x;
        item.data.ellipse.y = y;
        item.data.ellipse.xradius = xradius;
        item.data.ellipse.yradius = yradius;
        item.data.ellipse.startAngle = 0;
        item.data.ellipse.endAngle = 360;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::SINGLE_PIXEL>(uint_fast32_t x, uint_fast32_t y, uint_fast32_t color) {
        QueueItem item;
        item.type = QueueItem::SINGLE_PIXEL;
        item.data.singlePixel.x = x;
        item.data.singlePixel.y = y;
        item.data.singlePixel.color = color;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::SETLINESTYLE>(uint_fast32_t linestyle, uint_fast32_t pattern, uint_fast32_t thickness) {
        QueueItem item;
        item.type = QueueItem::SETLINESTYLE;
        item.data.setlinestyle.linestyle = static_cast<uint8_t>(linestyle);
        item.data.setlinestyle.pattern = pattern;
        item.data.setlinestyle.thickness = thickness;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::PIXEL_CONTAINER>(IPixelContainer *pixelContainer) {
        QueueItem item;
        item.type = QueueItem::PIXEL_CONTAINER;
        item.data.pixelContainer = pixelContainer;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::CLEAR>() {
        QueueItem item;
        item.type = QueueItem::CLEAR;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::BGCOLOR>(uint_fast32_t bgColor) {
        QueueItem item;
        item.type = QueueItem::BGCOLOR;
        item.data.bgColor = bgColor;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::COLOR>(uint_fast32_t color) {
        QueueItem item;
        item.type = QueueItem::COLOR;
        item.data.color = color;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::SETFILLPATTERN>(const char *fillPattern, uint_fast32_t color) {
        QueueItem item;
        item.type = QueueItem::SETFILLPATTERN;
        item.data.setfillpattern.fillPattern = new char[FPATTERN_SIZE];
        std::copy(fillPattern, fillPattern + FPATTERN_SIZE, item.data.setfillpattern.fillPattern);
        item.data.setfillpattern.color = color;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::DRAWPOLY>(uint_fast32_t numPoints, const int32_t *points) {
        QueueItem item;
        item.type = QueueItem::DRAWPOLY;
        item.data.drawPoly.numPoints = numPoints;
        item.data.drawPoly.points = new int32_t[numPoints * 2];
        std::copy(points, points + numPoints * 2, item.data.drawPoly.points);
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::BAR3D>(
            int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom, uint_fast32_t depth, bool haveTop
    ) {
        QueueItem item;
        item.type = QueueItem::BAR3D;
        item.data.bar3D.top = top;
        item.data.bar3D.bottom = bottom;
        item.data.bar3D.left = left;
        item.data.bar3D.right = right;
        item.data.bar3D.depth = depth;
        item.data.bar3D.haveTop = haveTop;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::FILLPOLY>(uint_fast32_t numPoints, const int32_t *points) {
        QueueItem item;
        item.type = QueueItem::FILLPOLY;
        item.data.drawPoly.numPoints = numPoints;
        item.data.drawPoly.points = new int32_t[numPoints * 2];
        std::copy(points, points + numPoints * 2, item.data.drawPoly.points);
        return item;
    }
}