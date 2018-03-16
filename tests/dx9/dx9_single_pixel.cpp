#include <gtest/gtest.h>
#include <tests/stubs/QueueReaderStub.h>
#include "common.h"

class DX9SinglePixelTest : public ImageTest, public CommonSimple {
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static uint32_t WIDTH;
    static uint32_t HEIGHT;

    DX9SinglePixelTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    ~DX9SinglePixelTest(){

    }
};

uint32_t DX9SinglePixelTest::WIDTH = 200;
uint32_t DX9SinglePixelTest::HEIGHT = 300;

IMG_TEST_F(DX9SinglePixelTest, OnePixel){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SINGLE_PIXEL>(static_cast<uint32_t>(WIDTH / 4), static_cast<uint32_t>(HEIGHT / 4), 0x000000)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9SinglePixelTest, PixelLine){
    QueueItem item = QueueItemCreator::create<QueueItem::CLEAR>();
    _readerStub.addItems(&item, 1);
    for(uint32_t i = 0; i < WIDTH; i += 2){
        item = QueueItemCreator::create<QueueItem::SINGLE_PIXEL>(i, 0, 0x000000);
        _readerStub.addItems(&item, 1);
    }
    for(uint32_t i = 1; i < WIDTH; i += 2){
        item = QueueItemCreator::create<QueueItem::SINGLE_PIXEL>(i, 1, 0xFF00FF);
        _readerStub.addItems(&item, 1);
    }
    return afterTestSimple(win, &_readerStub);
}