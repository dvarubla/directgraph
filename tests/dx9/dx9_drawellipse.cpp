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

IMG_TEST_F(DX9FillEllipseTest, SimpleEllipse1px){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 1),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00BBFF),
            QueueItemCreator::create<QueueItem::ELLIPSE>(2, 2, 1u, 1u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, SimpleEllipse1pxThick){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 1),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00BBFF),
            QueueItemCreator::create<QueueItem::ELLIPSE>(11, 11, 10u, 7u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, SimpleEllipse1pxThickReverse){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 1),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00BBFF),
            QueueItemCreator::create<QueueItem::ELLIPSE>(11, 11, 7u, 10u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, Ellipse5pxThick){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 5),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00BBFF),
            QueueItemCreator::create<QueueItem::ELLIPSE>(15, 15, 10u, 7u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, Ellipse5pxThickLong){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 5),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00BBFF),
            QueueItemCreator::create<QueueItem::ELLIPSE>(75, 15, 70u, 7u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, Ellipse1pxLong){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 1),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00BBFF),
            QueueItemCreator::create<QueueItem::ELLIPSE>(52, 2, 50u, 1u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, Ellipse10pxThick){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 10),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00BBFF),
            QueueItemCreator::create<QueueItem::ELLIPSE>(16, 16, 10u, 6u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, Ellipse10pxThickReverse){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 10),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00BBFF),
            QueueItemCreator::create<QueueItem::ELLIPSE>(16, 16, 6u, 10u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseTest, EllipseTooThick){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 30),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00BBFF),
            QueueItemCreator::create<QueueItem::ELLIPSE>(50, 50, 20u, 1u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}