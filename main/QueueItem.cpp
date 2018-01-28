#include <cstring>
#include <algorithm>
#include "QueueItem.h"
#include "patterns.h"

namespace directgraph{
    template<>
    QueueItem QueueItemCreator::create<QueueItem::SETFILLSTYLE>(uint8_t fillStyle, uint32_t color) {
        QueueItem item;
        item.type = QueueItem::SETFILLSTYLE;
        item.data.setfillstyle.fillStyle = fillStyle;
        item.data.setfillstyle.color = color;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::BAR>(float left, float top, float right, float bottom) {
        QueueItem item;
        item.type = QueueItem::BAR;
        item.data.bar.left = left;
        item.data.bar.top = top;
        item.data.bar.right = right;
        item.data.bar.bottom = bottom;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::SINGLE_PIXEL>(uint32_t x, uint32_t y, uint32_t color) {
        QueueItem item;
        item.type = QueueItem::SINGLE_PIXEL;
        item.data.singlePixel.x = x;
        item.data.singlePixel.y = y;
        item.data.singlePixel.color = color;
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
    QueueItem QueueItemCreator::create<QueueItem::BGCOLOR>(uint32_t color) {
        QueueItem item;
        item.type = QueueItem::BGCOLOR;
        item.data.bgColor = color;
        return item;
    }

    template<>
    QueueItem QueueItemCreator::create<QueueItem::SETFILLPATTERN>(const char *fillPattern, uint32_t color) {
        QueueItem item;
        item.type = QueueItem::SETFILLPATTERN;
        item.data.setfillpattern.fillPattern = new char[FPATTERN_SIZE];
        std::copy(fillPattern, fillPattern + FPATTERN_SIZE, item.data.setfillpattern.fillPattern);
        item.data.setfillpattern.color = color;
        return item;
    }
}