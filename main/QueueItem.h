#ifndef PROJECT_QUEUEITEM_H
#define PROJECT_QUEUEITEM_H

#include <stdint.h>

namespace directgraph{
    struct QueueItem{
        enum {
            CLEAR = 0,
            SETFILLSTYLE,
            BAR,
            SINGLE_PIXEL
        } type;
        union{
            struct{
                uint8_t fillStyle;
                uint32_t color;
            } setfillstyle;
            struct {
                float left, top, right, bottom;
            } bar;
            struct{
                uint32_t x, y;
                uint32_t color;
            } singlePixel;
        } data;
    };
}

#endif //PROJECT_QUEUEITEM_H
