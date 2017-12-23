#include <gtest/gtest.h>
#include <tests/stubs/QueueReaderStub.h>
#include <PixelContainer.h>
#include <QueueItem.h>
#include <graphics_const_internal.h>

namespace {
    class DX9PixelContainerTest : public ::testing::Test {
    public:

    protected:

        DX9PixelContainerTest() {
        }

        virtual ~DX9PixelContainerTest() {
        }

        virtual void SetUp() {
        }

        virtual void TearDown() {
        }
    };
}

#include "common.h"

static float WIDTH = 200;
static float HEIGHT = 300;

IMG_TEST_F(DX9PixelContainerTest, TwoPixels){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = win->getRenderer()->getPixContFactory()->tryGetContainer(
            0, 1, 0x0000FF,
            1, 1, 0x0000FF
    ).container;
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, TwoPixelsVertical){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = win->getRenderer()->getPixContFactory()->tryGetContainer(
            0, 1, 0x0000FF,
            0, 2, 0x0000FF
    ).container;
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, VerticalTopDown){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = win->getRenderer()->getPixContFactory()->tryGetContainer(
            2, 1, 0x0000FF,
            2, 2, 0x0000BB
    ).container;
    for(uint_fast32_t j = 3; j <= 10; j++) {
        pixelContainer->tryAddPixel(2, j, (j % 2 == 1) ? 0x0000FF : 0x0000BB);
    }
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, VerticalBottomUp){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = win->getRenderer()->getPixContFactory()->tryGetContainer(
            2, 10, 0x0000BB,
            2, 9, 0x0000FF
    ).container;
    for(uint_fast32_t j = 8; j >= 1; j--) {
        pixelContainer->tryAddPixel(2, j, (j % 2 == 1) ? 0x0000FF : 0x0000BB);
    }
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, RedSquare5x5TopDown){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = win->getRenderer()->getPixContFactory()->tryGetContainer(
            4, 1, 0x000022,
            5, 1, 0x000022
    ).container;

    for(uint_fast32_t i = 1; i <= 5; i++) {
        for(uint_fast32_t j = (i == 1) ? 6 : 4; j <= 8; j++) {
            pixelContainer->tryAddPixel(j, i, (i % 2 == 1) ? 0x0000FF : 0x0000BB);
        }
    }
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, RedSquareAndBar){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[4];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = win->getRenderer()->getPixContFactory()->tryGetContainer(
            4, 1, 0x000022,
            5, 1, 0x000022
    ).container;

    for(uint_fast32_t i = 1; i <= 5; i++) {
        for(uint_fast32_t j = (i == 1) ? 6 : 4; j <= 8; j++) {
            pixelContainer->tryAddPixel(j, i, (i % 2 == 1) ? 0x0000FF : 0x0000BB);
        }
    }
    items[1].data.pixelContainer = pixelContainer;
    items[2].type = QueueItem::SETFILLSTYLE;
    items[2].data.setfillstyle = {SOLID_FILL, 0x00FF00};
    items[3].type = QueueItem::BAR;
    items[3].data.bar = {6, 3, 10, 20};
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, RedSquare5x5BottomUp){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = win->getRenderer()->getPixContFactory()->tryGetContainer(
            4, 5, 0x000022,
            5, 5, 0x000022
    ).container;

    for(uint_fast32_t i = 5; i != 0; i--) {
        for(uint_fast32_t j = (i == 5) ? 6 : 4; j <= 8; j++) {
            pixelContainer->tryAddPixel(j, i, (i % 2 == 1) ? 0x0000FF : 0x0000BB);
        }
    }
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, ExtraLineTopDown){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = win->getRenderer()->getPixContFactory()->tryGetContainer(
            20, 1, 0xFF0000,
            21, 1, 0xFF0000
    ).container;

    for(uint_fast32_t i = 1; i < 50; i++) {
        for(uint_fast32_t j = (i == 1) ? 22 : 20; j < 100; j++) {
            pixelContainer->tryAddPixel(j, i, (i * 10) % 0xFF);
        }
    }
    for(uint_fast32_t j = 20; j < 40; j++) {
        pixelContainer->tryAddPixel(j, 50, 0xFF0000);
    }
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9PixelContainerTest, ExtraLineBottomUp){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[2];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::PIXEL_CONTAINER;
    IPixelContainer *pixelContainer = win->getRenderer()->getPixContFactory()->tryGetContainer(
            20, 50, 0xFF0000,
            21, 50, 0xFF0000
    ).container;

    for(uint_fast32_t i = 50; i != 1; i--) {
        for(uint_fast32_t j = (i == 50) ? 22 : 20; j < 100; j++) {
            pixelContainer->tryAddPixel(j, i, (i * 10) % 0xFF);
        }
    }
    for(uint_fast32_t j = 20; j < 40; j++) {
        pixelContainer->tryAddPixel(j, 1, 0xFF0000);
    }
    items[1].data.pixelContainer = pixelContainer;
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}