#include <gtest/gtest.h>
#include <dx9/WindowFactory.h>
#include <directgraph/directgraph_constants.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9TransparencyTest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static int32_t WIDTH;
    static int32_t HEIGHT;

    DX9TransparencyTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    virtual ~DX9TransparencyTest() {
    }
};

int32_t DX9TransparencyTest::WIDTH = 200;
int32_t DX9TransparencyTest::HEIGHT = 300;

IMG_TEST_F(DX9TransparencyTest, TwoBars){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x7FFF0000),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyTest, TwoBarsHatchFill){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x7FFF0000),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL,0x7F00FF00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyTest, TwoEllipses){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x7FFF0000),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(3*WIDTH/4, 3*HEIGHT/4, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyTest, TwoEllipsesHatchFill){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x7FFF0000),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL,0x7F00FF00),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(3*WIDTH/4, 3*HEIGHT/4, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyTest, EllipsesRectanglesLinestyle){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),
            QueueItemCreator::create<QueueItem::COLOR>(0x7FFF0000),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 5),
            QueueItemCreator::create<QueueItem::RECTANGLE>(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(USERBIT_LINE, 0x1111, 5),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00FF00),
            QueueItemCreator::create<QueueItem::RECTANGLE>(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyTest, MultipleItems){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x44AAAAAA),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(BKSLASH_FILL, 0x7F0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/3, static_cast<uint32_t>(WIDTH/3 + 1), static_cast<uint32_t>(HEIGHT/3)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SLASH_FILL, 0x7FFF0000),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/3 + 1, HEIGHT/2, static_cast<uint32_t>(WIDTH/3 + 1), static_cast<uint32_t>(HEIGHT/3)),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0xFFFFFF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0xAA00AA),
            QueueItemCreator::create<QueueItem::BAR>(2*WIDTH/5, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0xFF000000),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),

    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}