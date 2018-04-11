#include <gtest/gtest.h>
#include <dx9/WindowFactory.h>
#include <directgraph/directgraph_constants.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9FillPolyTest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static int32_t WIDTH;
    static int32_t HEIGHT;

    DX9FillPolyTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    virtual ~DX9FillPolyTest() {
    }
};

int32_t DX9FillPolyTest::WIDTH = 200;
int32_t DX9FillPolyTest::HEIGHT = 300;

IMG_TEST_F(DX9FillPolyTest, NullLineTriangle){
    int32_t points[] = {100, 100, 100, 50, 150, 50};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillPolyTest, NullLineQuad){
    int32_t points[] = {100, 100, 100, 50, 150, 50, 150, 100};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillPolyTest, NullLine7Points){
    int32_t points[] = {50, 100,
                        0, 100,
                        0, 50,
                        50, 0,
                        100, 25,
                        50, 50,
                        100, 50
    };
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillPolyTest, NullLine12Points){
    int32_t points[] = {
            10, 0, 30, 0,
            30, 10, 20,10,
            20,20, 50,20,
            50,10, 40,10,
            40,0,60,0,
            60,30, 10,30
    };
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillPolyTest, EmptyNullLine){
    int32_t points[] = {
            10, 10,
            10,20,
            10,30,
            10,40,
            10,70
    };
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}
IMG_TEST_F(DX9FillPolyTest, SolidLineTriangle){
    int32_t points[] = {100, 100, 100, 50, 150, 50};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0xFF0000),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillPolyTest, SolidLineTriangleInv){
    int32_t points[] = {150, 50, 100, 100, 100, 50};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0xFF0000),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillPolyTest, SolidLineQuad){
    int32_t points[] = {100, 100, 100, 50, 150, 50, 150, 100};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0xFF0000),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 13),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillPolyTest, SolidLine7Points){
    int32_t points[] = {50, 100,
                        0, 100,
                        0, 50,
                        50, 0,
                        100, 25,
                        50, 50,
                        100, 50
    };
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0xFF0000),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 8),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillPolyTest, SolidLine12Points){
    int32_t points[] = {
            10, 0, 30, 0,
            30, 10, 20,10,
            20,20, 50,20,
            50,10, 40,10,
            40,0,60,0,
            60,30, 10,30
    };
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0xFF0000),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 4),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillPolyTest, SolidVerticalLines){
    int32_t points[] = {
            50, 70, 50, 80, 50, 100,
            90, 100, 90, 80, 90, 70
    };
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0xFF0000),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 4),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}
