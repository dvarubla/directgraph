#include <gtest/gtest.h>
#include <dx9/WindowFactory.h>
#include <directgraph/directgraph_constants.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9FillEllipseTest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static int32_t WIDTH;
    static int32_t HEIGHT;

    DX9FillEllipseTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    virtual ~DX9FillEllipseTest() {
    }
};

int32_t DX9FillEllipseTest::WIDTH = 200;
int32_t DX9FillEllipseTest::HEIGHT = 200;

IMG_TEST_F(DX9FillEllipseTest, SimpleEllipse){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/4, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/2))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, SimpleEllipseSolid1px){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x00FF00),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 1),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/4, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/2))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, Ellipse4){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/2), static_cast<uint32_t>(HEIGHT/2)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/4, HEIGHT/5, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/5)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0xFF0000),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(3*WIDTH/4, HEIGHT/5, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/5)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0xFFFF00),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, 4*HEIGHT/5, static_cast<uint32_t>(WIDTH/2), static_cast<uint32_t>(HEIGHT/5))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, CircleOutside){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00CCDD),
            QueueItemCreator::create<QueueItem::BAR>(-WIDTH/4, -HEIGHT/4, WIDTH - WIDTH/4, HEIGHT - HEIGHT/4),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0xFF0000),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH / 8, static_cast<int_fast32_t>(0.117634f * HEIGHT) + 1, WIDTH, HEIGHT),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(-WIDTH * 2, -HEIGHT * 2, static_cast<uint32_t>(WIDTH * 3), static_cast<uint32_t>(HEIGHT * 3))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, EllipseThenBar2){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/2), static_cast<uint32_t>(HEIGHT/2)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0xFF0000),
            QueueItemCreator::create<QueueItem::BAR>(0, 0, WIDTH/8, HEIGHT/8),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/4, HEIGHT/5, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/5)),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, HEIGHT/2, 3*WIDTH/4, 3*HEIGHT/4)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, Ellipse1px){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(1), static_cast<uint32_t>(1))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, Ellipse2px){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(2), static_cast<uint32_t>(2))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, SimpleEllipseSolid5px){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x00FF00),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 5),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(30, 50, 10u, 20u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, SimpleEllipseSolid20px){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00FF00),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 20),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x7F00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(30, 50, 10u, 20u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, SimpleEllipseSolid25px){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00FF00),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 25),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x7F00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(30, 50, 10u, 20u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, SimpleEllipseSolid25pxThenNull){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00FF00),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 11),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x7F00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(30, 50, 10u, 20u),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 25),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(20, 20, 10u, 20u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}