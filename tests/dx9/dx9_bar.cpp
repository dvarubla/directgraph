#include <gtest/gtest.h>
#include <dx9/DX9WindowFactory.h>
#include <directgraph/directgraph_constants.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9BarTest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static float WIDTH;
    static float HEIGHT;

    DX9BarTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    virtual ~DX9BarTest() {
    }
};

float DX9BarTest::WIDTH = 200;
float DX9BarTest::HEIGHT = 300;

IMG_TEST_F(DX9BarTest, SimpleBar){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarTest, InverseCoords){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4*3, HEIGHT/4*3, WIDTH/4, HEIGHT/4)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarTest, OutsideScreen){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::BAR>(-WIDTH/4, -HEIGHT/4, WIDTH/4*3, HEIGHT/4*3)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarTest, OutsideScreenFull){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::BAR>(-WIDTH/4, -HEIGHT/4, -WIDTH/4*3, -HEIGHT/4*3)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarTest, OnePixel){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x000000),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4 + 1, HEIGHT/4 + 1)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarTest, OnePxBorder){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::BAR>(0, 0, WIDTH, HEIGHT),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00FFFF),
            QueueItemCreator::create<QueueItem::BAR>(1, 1, WIDTH - 1, HEIGHT - 1)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}