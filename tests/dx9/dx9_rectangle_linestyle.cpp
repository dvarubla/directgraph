#include <gtest/gtest.h>
#include <dx9/WindowFactory.h>
#include <directgraph/directgraph_constants.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9RectangleLinestyleTest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static int32_t WIDTH;
    static int32_t HEIGHT;

    DX9RectangleLinestyleTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    virtual ~DX9RectangleLinestyleTest() {
    }
};

int32_t DX9RectangleLinestyleTest::WIDTH = 200;
int32_t DX9RectangleLinestyleTest::HEIGHT = 300;

IMG_TEST_F(DX9RectangleLinestyleTest, Simple2pxRectangle){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 2),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::RECTANGLE>(2, 2, 40, 60)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9RectangleLinestyleTest, Simple10pxRectangle){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 10),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::RECTANGLE>(6, 6, 40, 60)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9RectangleLinestyleTest, 4x6pxRectangle){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 7),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::RECTANGLE>(4, 4, 8, 10)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9RectangleLinestyleTest, 3Rectangles){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DASHED_LINE, 0, 4),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::RECTANGLE>(2, 2, 40, 60),
            QueueItemCreator::create<QueueItem::COLOR>(0x00FF00),
            QueueItemCreator::create<QueueItem::RECTANGLE>(60, 70, 80, 90),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DASHED_LINE, 0, 2),
            QueueItemCreator::create<QueueItem::COLOR>(0xFF0000),
            QueueItemCreator::create<QueueItem::RECTANGLE>(2, 40, 80, 60)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9RectangleLinestyleTest, 3RectanglesDifferentStyle){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 4),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::RECTANGLE>(2, 2, 40, 60),
            QueueItemCreator::create<QueueItem::COLOR>(0x00FF00),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DASHED_LINE, 0, 3),
            QueueItemCreator::create<QueueItem::RECTANGLE>(60, 70, 80, 90),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(CENTER_LINE, 0, 2),
            QueueItemCreator::create<QueueItem::COLOR>(0xFF0000),
            QueueItemCreator::create<QueueItem::RECTANGLE>(2, 40, 80, 60)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9RectangleLinestyleTest, 3RectanglesDifferentStyleThenSolid){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 4),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::RECTANGLE>(2, 2, 40, 60),
            QueueItemCreator::create<QueueItem::COLOR>(0x00FF00),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DASHED_LINE, 0, 3),
            QueueItemCreator::create<QueueItem::RECTANGLE>(60, 70, 80, 90),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(CENTER_LINE, 0, 4),
            QueueItemCreator::create<QueueItem::COLOR>(0xFF0000),
            QueueItemCreator::create<QueueItem::RECTANGLE>(2, 40, 80, 60),
            QueueItemCreator::create<QueueItem::COLOR>(0x00AABB),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(SOLID_LINE, 0, 2),
            QueueItemCreator::create<QueueItem::RECTANGLE>(2, 40, 80, 60)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}