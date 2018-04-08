#include <gtest/gtest.h>
#include <dx9/WindowFactory.h>
#include <directgraph/directgraph_constants.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9DrawPolyStyleTest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static int32_t WIDTH;
    static int32_t HEIGHT;

    DX9DrawPolyStyleTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    virtual ~DX9DrawPolyStyleTest() {
    }
};

int32_t DX9DrawPolyStyleTest::WIDTH = 200;
int32_t DX9DrawPolyStyleTest::HEIGHT = 300;

IMG_TEST_F(DX9DrawPolyStyleTest, TwoPerpLines1){
    int32_t points[] = {100, 150, 100, 50, 170, 50};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 25),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyStyleTest, TwoPerpLines2){
    int32_t points[] = {100, 100, 100, 50, 50, 50};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(USERBIT_LINE, 0xAAAA, 13),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyStyleTest, ThreePerpLines){
    int32_t points[] = {100, 100, 100, 50, 150, 50, 150, 100};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyStyleTest, 5Lines){
    int32_t points[] = {100, 100,
                        70, 30,
                        140, 20,
                        30, 200,
                        30, 120
    };
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyStyleTest, LinesOnePxWidth){
    int32_t points[] = {1, 10, 1, 1, 10, 1, 100, 150, 70, 20};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(USERBIT_LINE, 0xAAAA, 1),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyStyleTest, 4PerpLinesClosed){
    int32_t points[] = {100, 100, 100, 50, 150, 50, 150, 100, 100, 100};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyStyleTest, Triangle){
    int32_t points[] = {50, 150, 150, 150, 100, 200, 50, 150};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyStyleTest, NoConnTwoLines){
    int32_t points[] = {100, 150, 110, 150, 110, 220};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 20),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyStyleTest, NoConnThreeLines){
    int32_t points[] = {100, 150, 110, 150, 110, 219, 100, 219};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DASHED_LINE, 0, 20),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyStyleTest, NoConnTwoLinesAndConn){
    int32_t points[] = {100, 150, 110, 150, 110, 220, 50, 220};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DASHED_LINE, 0, 20),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyStyleTest, NoConnTwoLinesAndConnClosed){
    int32_t points[] = {100, 150, 110, 150, 110, 220, 50, 220, 100, 150};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DASHED_LINE, 0, 20),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyStyleTest, NoConn4LinesClosed){
    int32_t points[] = {100, 150, 110, 150, 110, 220, 100, 220, 100, 150};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DASHED_LINE, 0, 20),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DrawPolyStyleTest, VerticalLine){
    int32_t points[] = {100, 150, 100, 100, 100, 50};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DASHED_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::DRAWPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}
