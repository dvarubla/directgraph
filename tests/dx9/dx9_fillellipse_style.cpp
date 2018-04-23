#include <gtest/gtest.h>
#include <dx9/WindowFactory.h>
#include <directgraph/directgraph_constants.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9FillEllipseStyleTest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static int32_t WIDTH;
    static int32_t HEIGHT;

    DX9FillEllipseStyleTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    virtual ~DX9FillEllipseStyleTest() {
    }
};

int32_t DX9FillEllipseStyleTest::WIDTH = 200;
int32_t DX9FillEllipseStyleTest::HEIGHT = 200;

IMG_TEST_F(DX9FillEllipseStyleTest, SimpleEllipseHatch){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/4, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/2))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseStyleTest, TwoEllipsesHatch){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/4, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/2)),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(3*WIDTH/4, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/2))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseStyleTest, EllipsesDifferentFill){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/4, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/2)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0xAA2200),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x00FF00),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SLASH_FILL, 0xFF00FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(3*WIDTH/4, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/2)),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/4, HEIGHT/4, static_cast<uint32_t>(WIDTH/8), static_cast<uint32_t>(HEIGHT/5))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseStyleTest, EllipsesDifferentFillAndSolid){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/2), static_cast<uint32_t>(HEIGHT/2)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/4, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/2)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0xAA2200),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x00FF00),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SLASH_FILL, 0xFF00FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(3*WIDTH/4, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/2)),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/4, HEIGHT/4, static_cast<uint32_t>(WIDTH/8), static_cast<uint32_t>(HEIGHT/5)),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(3*WIDTH/4, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/2)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0xFFAAFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/2), static_cast<uint32_t>(HEIGHT/5))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseStyleTest, SimpleEllipseHatchSolid){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00FF00),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 11),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, 50u, 15u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseStyleTest, SimpleEllipseHatchSolid1px){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00FF00),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 1),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, 50u, 15u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseStyleTest, SimpleEllipseHatchSolidThenNull){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00FF00),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 11),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, 50u, 15u),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(XHATCH_FILL, 0x3300DD),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/4, HEIGHT/3, 20u, 14u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseStyleTest, SimpleEllipseHatchSolidTooThick){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00FF00),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 22),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, 50u, 10u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}