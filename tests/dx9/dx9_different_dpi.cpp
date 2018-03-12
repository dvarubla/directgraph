#include <gtest/gtest.h>
#include <dx9/WindowFactory.h>
#include <directgraph/directgraph_constants.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9DifferentDPITest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static float WIDTH;
    static float HEIGHT;

    DX9DifferentDPITest() {
    }

    void createWin(float dpiX, float dpiY){
        startThread();
        win = createDPIWindow(WIDTH, HEIGHT, dpiX, dpiY);
        addOnCall(_readerStub);
    }

    virtual ~DX9DifferentDPITest() {
    }
};

float DX9DifferentDPITest::WIDTH = 200;
float DX9DifferentDPITest::HEIGHT = 300;

IMG_TEST_F(DX9DifferentDPITest, BarEllipseX1_5){
    createWin(96.0f * 1.5f, 96.0f * 1.5f);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0xFF00FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, WIDTH/5, HEIGHT/2)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DifferentDPITest, BarEllipseX0_75){
    createWin(96.0f * 0.75f, 96.0f * 0.75f);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0xFF00FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, WIDTH/5, HEIGHT/2)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DifferentDPITest, BarEllipseX1_5_0_75){
    createWin(96.0f * 1.5f, 96.0f * 0.75f);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0xFF00FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, WIDTH/5, HEIGHT/2)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}


IMG_TEST_F(DX9DifferentDPITest, BarEllipseHatchFillX1_5_0_75){
    createWin(96.0f * 1.5f, 96.0f * 0.75f);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0xFF00FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, WIDTH/5, HEIGHT/2)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}