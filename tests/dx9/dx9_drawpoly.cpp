#include <gtest/gtest.h>
#include <dx9/WindowFactory.h>
#include <directgraph/directgraph_constants.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9DrawPolyTest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static int32_t WIDTH;
    static int32_t HEIGHT;

    DX9DrawPolyTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    virtual ~DX9DrawPolyTest() {
    }
};

int32_t DX9DrawPolyTest::WIDTH = 200;
int32_t DX9DrawPolyTest::HEIGHT = 300;

IMG_TEST_F(DX9DrawPolyTest, TwoPerpLines1){
    int32_t points[] = {100, 100, 100, 50, 150, 50};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyTest, TwoPerpLines2){
    int32_t points[] = {100, 100, 100, 50, 50, 50};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyTest, ThreePerpLines){
    int32_t points[] = {100, 100, 100, 50, 150, 50, 150, 100};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyTest, 4Lines){
    int32_t points[] = {100, 100,
                        70, 30,
                        140, 20,
                        30, 200};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyTest, 4PerpLinesClosed){
    int32_t points[] = {100, 100, 100, 50, 150, 50, 150, 100, 100, 100};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyTest, Triangle){
    int32_t points[] = {50, 150, 150, 150, 100, 200, 50, 150};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyTest, NoConnTwoLines){
    int32_t points[] = {100, 150, 110, 150, 110, 220};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 20),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyTest, NoConnThreeLines){
    int32_t points[] = {100, 150, 110, 150, 110, 220, 100, 220};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 20),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyTest, NoConnTwoLinesAndConn){
    int32_t points[] = {100, 150, 110, 150, 110, 220, 50, 220};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 20),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyTest, NoConnTwoLinesAndConnClosed){
    int32_t points[] = {100, 150, 110, 150, 110, 220, 50, 220, 100, 150};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 20),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyTest, NoConn4LinesClosed){
    int32_t points[] = {100, 150, 110, 150, 110, 220, 100, 220, 100, 150};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 20),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyTest, VerticalLine){
    int32_t points[] = {100, 150, 100, 100, 100, 50};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyTest, VerticalLine2){
    int32_t points[] = {100, 150, 100, 100, 100, 150};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyTest, TwoPerpLinesOnePxWidth){
    int32_t points[] = {1, 10, 1, 1, 10, 1};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 1),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}