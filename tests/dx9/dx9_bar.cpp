#include <gtest/gtest.h>
#include <dx9/DX9WindowFactory.h>
#include <graphics_const_internal.h>
#include <tests/stubs/QueueReaderStub.h>

namespace {
    class BarTest : public ::testing::Test {
    public:

    protected:

        BarTest() {
        }

        virtual ~BarTest() {
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

IMG_TEST_F(BarTest, SimpleBar){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[3];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::SETFILLSTYLE;
    items[1].data.setfillstyle = {SOLID_FILL, 0x00BBFF};
    items[2].type = QueueItem::BAR;
    items[2].data.bar = {WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3};
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(BarTest, InverseCoords){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[3];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::SETFILLSTYLE;
    items[1].data.setfillstyle = {SOLID_FILL, 0x00BBFF};
    items[2].type = QueueItem::BAR;
    items[2].data.bar = {WIDTH/4*3, HEIGHT/4*3, WIDTH/4, HEIGHT/4};
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(BarTest, OutsideScreen){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[3];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::SETFILLSTYLE;
    items[1].data.setfillstyle = {SOLID_FILL, 0x00BBFF};
    items[2].type = QueueItem::BAR;
    items[2].data.bar = {-WIDTH/4, -HEIGHT/4, WIDTH/4*3, HEIGHT/4*3};
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(BarTest, OutsideScreenFull){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[3];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::SETFILLSTYLE;
    items[1].data.setfillstyle = {SOLID_FILL, 0x00BBFF};
    items[2].type = QueueItem::BAR;
    items[2].data.bar = {-WIDTH/4, -HEIGHT/4, -WIDTH/4*3, -HEIGHT/4*3};
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(BarTest, OnePixel){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[3];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::SETFILLSTYLE;
    items[1].data.setfillstyle = {SOLID_FILL, 0x000000};
    items[2].type = QueueItem::BAR;
    items[2].data.bar = {WIDTH/4, HEIGHT/4, WIDTH/4 + 1, HEIGHT/4 + 1};
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(BarTest, OnePxBorder){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    QueueReaderStub _readerStub;
    QueueItem items[5];
    items[0].type = QueueItem::CLEAR;
    items[1].type = QueueItem::SETFILLSTYLE;
    items[1].data.setfillstyle = {SOLID_FILL, 0x0000FF};
    items[2].type = QueueItem::BAR;
    items[2].data.bar = {0, 0, WIDTH, HEIGHT};
    items[3].type = QueueItem::SETFILLSTYLE;
    items[3].data.setfillstyle = {SOLID_FILL, 0x00FFFF};
    items[4].type = QueueItem::BAR;
    items[4].data.bar = {1, 1, WIDTH - 1, HEIGHT - 1};
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}