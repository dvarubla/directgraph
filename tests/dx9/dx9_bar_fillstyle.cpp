#include <gtest/gtest.h>
#include <tests/stubs/QueueReaderStub.h>
#include <directgraph/directgraph_constants.h>
#include "common.h"

class DX9BarFillstyleTest : public ImageTest, public CommonSimple {
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static float WIDTH;
    static float HEIGHT;

    DX9BarFillstyleTest() {
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    ~DX9BarFillstyleTest(){

    }
};

float DX9BarFillstyleTest::WIDTH = 200;
float DX9BarFillstyleTest::HEIGHT = 300;

IMG_TEST_F(DX9BarFillstyleTest, LineFill8x8){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0x00FF00),
            QueueItemCreator::create<QueueItem::BAR>(0, 0, 8, 8)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarFillstyleTest, LineFill){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0x00FF00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarFillstyleTest, HatchFill){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x00FF00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarFillstyleTest, LineFill4Bars){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0x00FF00),
            QueueItemCreator::create<QueueItem::BAR>(0, 0, WIDTH/2, HEIGHT/2),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0xFF0000),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, 0, WIDTH, HEIGHT/2),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x00FFFF),
            QueueItemCreator::create<QueueItem::BAR>(0, HEIGHT/2, WIDTH/2, HEIGHT),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0x00FF00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarFillstyleTest, DifferentFill4Bars){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x00FF00),
            QueueItemCreator::create<QueueItem::BAR>(0, 0, WIDTH/2, HEIGHT/2),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0x00FF00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, 0, WIDTH, HEIGHT/2),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x00FFFF),
            QueueItemCreator::create<QueueItem::BAR>(0, HEIGHT/2, WIDTH/2, HEIGHT),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(CLOSE_DOT_FILL, 0x00FF00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarFillstyleTest, DifferentFill5Bars){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x00FF00),
            QueueItemCreator::create<QueueItem::BAR>(0, 0, WIDTH/2, HEIGHT/2),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0x00FF00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, 0, WIDTH, HEIGHT/2),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x00FFFF),
            QueueItemCreator::create<QueueItem::BAR>(0, HEIGHT/2, WIDTH/2, HEIGHT),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(CLOSE_DOT_FILL, 0x00FF00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00CC00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarFillstyleTest, UserPattern){
    uint8_t pattern[] = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0xFFFFFF),
            QueueItemCreator::create<QueueItem::SETFILLPATTERN>(reinterpret_cast<char*>(pattern), 0x0000FF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarFillstyleTest, UserPattern2Times){
    uint8_t pattern[] = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88};
    uint8_t pattern2[] = {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0xFFFFFF),
            QueueItemCreator::create<QueueItem::SETFILLPATTERN>(reinterpret_cast<char*>(pattern), 0x0000FF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/2, HEIGHT/2),
            QueueItemCreator::create<QueueItem::SETFILLPATTERN>(reinterpret_cast<char*>(pattern2), 0x0000FF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, HEIGHT/2, 3*WIDTH/4, 3*HEIGHT/4)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarFillstyleTest, UserFill){
    uint8_t pattern[] = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0xFFFFFF),
            QueueItemCreator::create<QueueItem::SETFILLPATTERN>(reinterpret_cast<char*>(pattern), 0x0000FF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(USER_FILL, 0xFF00FF),
            QueueItemCreator::create<QueueItem::BAR>(3*WIDTH/8, 3*HEIGHT/8, 5*WIDTH/8, 5*HEIGHT/8)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarFillstyleTest, UserPatternThenSolidFill){
    uint8_t pattern[] = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88};
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0xFFFFFF),
            QueueItemCreator::create<QueueItem::SETFILLPATTERN>(reinterpret_cast<char*>(pattern), 0x0000FF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00CC00),
            QueueItemCreator::create<QueueItem::BAR>(0, 0, WIDTH/4, HEIGHT/4)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarFillstyleTest, EmptyFill){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(EMPTY_FILL, 0x00CC00),
            QueueItemCreator::create<QueueItem::BAR>(0, 0, WIDTH/4, HEIGHT/4)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarFillstyleTest, EmptyFillThenSolid){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(EMPTY_FILL, 0x00CC00),
            QueueItemCreator::create<QueueItem::BAR>(0, 0, WIDTH/4, HEIGHT/4),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00CC00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/2, HEIGHT/2)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}