#include <gtest/gtest.h>
#include <dx9/WindowFactory.h>
#include <directgraph/directgraph_constants.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9FillPolyStyleTest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static int32_t WIDTH;
    static int32_t HEIGHT;

    DX9FillPolyStyleTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    virtual ~DX9FillPolyStyleTest() {
    }
};

int32_t DX9FillPolyStyleTest::WIDTH = 200;
int32_t DX9FillPolyStyleTest::HEIGHT = 300;

IMG_TEST_F(DX9FillPolyStyleTest, NullLineTriangleHatch){
    int32_t points[] = {100, 100, 100, 50, 150, 50};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillPolyStyleTest, NullLine7PointsSlash){
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
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SLASH_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillPolyStyleTest, DottedLine5PointsLineFill){
    int32_t points[] = {10,10,
                        50,100,
                        100,50,
                        50,50,
                        50,10,
                        10,10
    };
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x00FF00),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 10),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0xFF0000),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points) / sizeof(int32_t) / 2, points)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillPolyStyleTest, DashedLine12PointsAndTriangle){
    int32_t points1[] = {
            10, 0, 30, 0,
            30, 10, 20,10,
            20,20, 50,20,
            50,10, 40,10,
            40,0,60,0,
            60,30, 10,30
    };

    int32_t points2[] = {
            160, 120, 180, 220, 30, 150
    };

    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x00FF00),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DASHED_LINE, 0, 2),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0xFF0000),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points1) / sizeof(int32_t) / 2, points1),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0xFFFF00),
            QueueItemCreator::create<QueueItem::FILLPOLY>(sizeof(points2) / sizeof(int32_t) / 2, points2)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}