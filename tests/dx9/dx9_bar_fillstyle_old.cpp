#include <gtest/gtest.h>
#include <tests/stubs/QueueReaderStub.h>
#include <directgraph/directgraph_constants.h>
#include "common.h"

class DX9BarFillstyleOldTest : public ImageTest, public CommonSimple {
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static float WIDTH;
    static float HEIGHT;

    DX9BarFillstyleOldTest() {
        startThread();
        FeaturesStub *fstub = setupFeatures();
        ON_CALL(*fstub, getPatternTexFormat()).WillByDefault(testing::Return(ColorFormat::AR5G5B5));
        ON_CALL(*fstub, supportsTexConst()).WillByDefault(testing::Return(false));
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    ~DX9BarFillstyleOldTest(){

    }
};

float DX9BarFillstyleOldTest::WIDTH = 200;
float DX9BarFillstyleOldTest::HEIGHT = 300;

IMG_TEST_F(DX9BarFillstyleOldTest, LineFill8x8){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0x00FF00),
            QueueItemCreator::create<QueueItem::BAR>(0, 0, 8, 8)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarFillstyleOldTest, LineFill){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0x00FF00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarFillstyleOldTest, HatchFill){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x00FF00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9BarFillstyleOldTest, LineFill4Bars){
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

IMG_TEST_F(DX9BarFillstyleOldTest, DifferentFill4Bars){
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

IMG_TEST_F(DX9BarFillstyleOldTest, DifferentFill5Bars){
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

IMG_TEST_F(DX9BarFillstyleOldTest, UserPattern){
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

IMG_TEST_F(DX9BarFillstyleOldTest, UserFill){
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

IMG_TEST_F(DX9BarFillstyleOldTest, UserPatternThenSolidFill){
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