#include <gtest/gtest.h>
#include <dx9/DX9WindowFactory.h>
#include <graphics_const_internal.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9BarTest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    const float WIDTH = 200;
    const float HEIGHT = 300;

    DX9BarTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    virtual ~DX9BarTest() {
    }
};

IMG_TEST_F(DX9BarTest, SimpleBar){
    QueueItem items[3];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::SETFILLSTYLE;
    items[1].data.setfillstyle = {SOLID_FILL, 0x00BBFF};
    items[2].type = QueueItem::BAR;
    items[2].data.bar = {WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3};
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarTest, InverseCoords){
    QueueItem items[3];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::SETFILLSTYLE;
    items[1].data.setfillstyle = {SOLID_FILL, 0x00BBFF};
    items[2].type = QueueItem::BAR;
    items[2].data.bar = {WIDTH/4*3, HEIGHT/4*3, WIDTH/4, HEIGHT/4};
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarTest, OutsideScreen){
    QueueItem items[3];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::SETFILLSTYLE;
    items[1].data.setfillstyle = {SOLID_FILL, 0x00BBFF};
    items[2].type = QueueItem::BAR;
    items[2].data.bar = {-WIDTH/4, -HEIGHT/4, WIDTH/4*3, HEIGHT/4*3};
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarTest, OutsideScreenFull){
    QueueItem items[3];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::SETFILLSTYLE;
    items[1].data.setfillstyle = {SOLID_FILL, 0x00BBFF};
    items[2].type = QueueItem::BAR;
    items[2].data.bar = {-WIDTH/4, -HEIGHT/4, -WIDTH/4*3, -HEIGHT/4*3};
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarTest, OnePixel){
    QueueItem items[3];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::SETFILLSTYLE;
    items[1].data.setfillstyle = {SOLID_FILL, 0x000000};
    items[2].type = QueueItem::BAR;
    items[2].data.bar = {WIDTH/4, HEIGHT/4, WIDTH/4 + 1, HEIGHT/4 + 1};
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarTest, OnePxBorder){
    QueueItem items[5];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::SETFILLSTYLE;
    items[1].data.setfillstyle = {SOLID_FILL, 0x0000FF};
    items[2].type = QueueItem::BAR;
    items[2].data.bar = {0, 0, WIDTH, HEIGHT};
    items[3].type = QueueItem::SETFILLSTYLE;
    items[3].data.setfillstyle = {SOLID_FILL, 0x00FFFF};
    items[4].type = QueueItem::BAR;
    items[4].data.bar = {1, 1, WIDTH - 1, HEIGHT - 1};
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}