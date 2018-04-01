#include <gtest/gtest.h>
#include <dx9/WindowFactory.h>
#include <directgraph/directgraph_constants.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9LineStyleTest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static int32_t WIDTH;
    static int32_t HEIGHT;

    DX9LineStyleTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    virtual ~DX9LineStyleTest() {
    }
};

int32_t DX9LineStyleTest::WIDTH = 200;
int32_t DX9LineStyleTest::HEIGHT = 300;

IMG_TEST_F(DX9LineStyleTest, SimpleHorizontalLine){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 1),
            QueueItemCreator::create<QueueItem::COLOR>(0u),
            QueueItemCreator::create<QueueItem::LINE>(1, 1, 100, 1)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9LineStyleTest, SimpleDiagonalLine){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 1),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::LINE>(1, 1, 101, 101)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9LineStyleTest, Line100_50){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 1),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::LINE>(1, 1, 101, 51)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9LineStyleTest, Line50_101){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 1),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::LINE>(51, 101, 1, 1)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9LineStyleTest, LineThick50_101){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DASHED_LINE, 0, 4),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::LINE>(1, 1, 51, 101)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9LineStyleTest, LineThickAndRectangle){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DASHED_LINE, 0, 4),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F0000FF),
            QueueItemCreator::create<QueueItem::LINE>(4, 4, 196, 4),
            QueueItemCreator::create<QueueItem::COLOR>(0x7FFF0000),
            QueueItemCreator::create<QueueItem::RECTANGLE>(4, 4, 196, 200)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9LineStyleTest, LineThickAndRectangle2){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DASHED_LINE, 0, 5),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F0000FF),
            QueueItemCreator::create<QueueItem::LINE>(4, 4, 196, 4),
            QueueItemCreator::create<QueueItem::COLOR>(0x7FFF0000),
            QueueItemCreator::create<QueueItem::RECTANGLE>(4, 4, 196, 200)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9LineStyleTest, MultipleLines){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(USERBIT_LINE, 0x1, 4),
            QueueItemCreator::create<QueueItem::COLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::LINE>(40, 40, 80, 65),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(USERBIT_LINE, 0x2, 4),
            QueueItemCreator::create<QueueItem::COLOR>(0x00FFFF),
            QueueItemCreator::create<QueueItem::LINE>(40, 40, 80, 65),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DASHED_LINE, 0, 7),
            QueueItemCreator::create<QueueItem::COLOR>(0xFF0000),
            QueueItemCreator::create<QueueItem::LINE>(80, 65, 100, 20),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(CENTER_LINE, 0, 2),
            QueueItemCreator::create<QueueItem::COLOR>(0x00FF00),
            QueueItemCreator::create<QueueItem::LINE>(100, 20, 40, 40)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}