#include <gtest/gtest.h>
#include <tests/stubs/QueueReaderStub.h>
#include "common.h"

class DX9SinglePixelTest : public ImageTest, public CommonSimple {
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    const float WIDTH = 200;
    const float HEIGHT = 300;

    DX9SinglePixelTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    ~DX9SinglePixelTest(){

    }
};

IMG_TEST_F(DX9SinglePixelTest, OnePixel){
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::SINGLE_PIXEL;
    items[1].data.singlePixel = {static_cast<uint32_t>(WIDTH / 4), static_cast<uint32_t>(HEIGHT / 4), 0x000000};
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9SinglePixelTest, PixelLine){
    QueueItem item;
    item.type = QueueItem::CLEAR;
    _readerStub.addItems(&item, 1);
    for(uint32_t i = 0; i < WIDTH; i += 2){
        item.type = QueueItem::SINGLE_PIXEL;
        item.data.singlePixel = {i, 0, 0x000000};
        _readerStub.addItems(&item, 1);
    }
    for(uint32_t i = 1; i < WIDTH; i += 2){
        item.type = QueueItem::SINGLE_PIXEL;
        item.data.singlePixel = {i, 1, 0xFF00FF};
        _readerStub.addItems(&item, 1);
    }
    return afterTestSimple(win, &_readerStub);
}