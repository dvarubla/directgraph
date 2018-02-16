#include <gtest/gtest.h>
#include <tests/stubs/QueueReaderStub.h>
#include <directgraph/directgraph_constants.h>
#include <PixelContainer.h>
#include "common.h"

class DX9PixelContainerTest : public ImageTest, public CommonSimple {
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static float WIDTH;
    static float HEIGHT;

    DX9PixelContainerTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }
    
    IPixelContainer *createContainer(uint_fast32_t x1, uint_fast32_t y1, uint_fast32_t c1,
                                     uint_fast32_t x2, uint_fast32_t y2, uint_fast32_t c2){
        std::vector<IPixelContainer::SinglePixel> pixels;
        IPixelContainer::SinglePixel v1 = {x1, y1, c1};
        IPixelContainer::SinglePixel v2 = {x2, y2, c2};
        pixels.push_back(v1);
        pixels.push_back(v2);
        IPixelContainer *pixelContainer = new PixelContainer<ColorFormat::A8R8G8B8>(
                pixels,
                static_cast<uint_fast32_t>(WIDTH),
                static_cast<uint_fast32_t>(HEIGHT)
        );
        return pixelContainer;
    }
    
    ~DX9PixelContainerTest(){

    }
};

float DX9PixelContainerTest::WIDTH = 200;
float DX9PixelContainerTest::HEIGHT = 300;

IMG_TEST_F(DX9PixelContainerTest, TwoPixels){
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    
    items[1].data.pixelContainer = createContainer(0, 1, 0x0000FF,
                                                   1, 1, 0x0000FF);
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, TwoPixelsVertical){
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = createContainer(
            0, 1, 0x0000FF,
            0, 2, 0x0000FF
    );
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, VerticalTopDown){
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = createContainer(
            2, 1, 0x0000FF,
            2, 2, 0x0000BB
    );
    for(uint_fast32_t j = 3; j <= 10; j++) {
        pixelContainer->addPixel(2, j, (j % 2 == 1) ? 0x0000FF : 0x0000BB);
    }
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, VerticalBottomUp){
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = createContainer(
            2, 10, 0x0000BB,
            2, 9, 0x0000FF
    );
    for(uint_fast32_t j = 8; j >= 1; j--) {
        pixelContainer->addPixel(2, j, (j % 2 == 1) ? 0x0000FF : 0x0000BB);
    }
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, RedSquare5x5TopDown){
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = createContainer(
            4, 1, 0x000022,
            5, 1, 0x000022
    );

    for(uint_fast32_t i = 1; i <= 5; i++) {
        for(uint_fast32_t j = (i == 1) ? 6 : 4; j <= 8; j++) {
            pixelContainer->addPixel(j, i, (i % 2 == 1) ? 0x0000FF : 0x0000BB);
        }
    }
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, RedSquareAndBar){
    QueueItem items[4];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = createContainer(
            4, 1, 0x000022,
            5, 1, 0x000022
    );

    for(uint_fast32_t i = 1; i <= 5; i++) {
        for(uint_fast32_t j = (i == 1) ? 6 : 4; j <= 8; j++) {
            pixelContainer->addPixel(j, i, (i % 2 == 1) ? 0x0000FF : 0x0000BB);
        }
    }
    items[1].data.pixelContainer = pixelContainer;
    items[2] = QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00FF00);
    items[3] = QueueItemCreator::create<QueueItem::BAR>(6, 3, 10, 20);
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, RedSquare5x5BottomUp){
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = createContainer(
            4, 5, 0x000022,
            5, 5, 0x000022
    );

    for(uint_fast32_t i = 5; i != 0; i--) {
        for(uint_fast32_t j = (i == 5) ? 6 : 4; j <= 8; j++) {
            pixelContainer->addPixel(j, i, (i % 2 == 1) ? 0x0000FF : 0x0000BB);
        }
    }
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, ExtraLineTopDown){
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = createContainer(
            20, 1, 0xFF0000,
            21, 1, 0xFF0000
    );

    for(uint_fast32_t i = 1; i < 50; i++) {
        for(uint_fast32_t j = (i == 1) ? 22 : 20; j < 100; j++) {
            pixelContainer->addPixel(j, i, (i * 10) % 0xFF);
        }
    }
    for(uint_fast32_t j = 20; j < 40; j++) {
        pixelContainer->addPixel(j, 50, 0xFF0000);
    }
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, ExtraLineBottomUp){
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = createContainer(
            20, 50, 0xFF0000,
            21, 50, 0xFF0000
    );

    for(uint_fast32_t i = 50; i != 1; i--) {
        for(uint_fast32_t j = (i == 50) ? 22 : 20; j < 100; j++) {
            pixelContainer->addPixel(j, i, (i * 10) % 0xFF);
        }
    }
    for(uint_fast32_t j = 20; j < 40; j++) {
        pixelContainer->addPixel(j, 1, 0xFF0000);
    }
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}