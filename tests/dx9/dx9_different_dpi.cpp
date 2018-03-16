#include <gtest/gtest.h>
#include <dx9/WindowFactory.h>
#include <directgraph/directgraph_constants.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9DifferentDPITest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    int32_t width;
    int32_t height;

    DX9DifferentDPITest() {
    }

    void createWin(float dpiX, float dpiY){
        startThread();
        width = static_cast<int32_t>(200 * dpiX / 96);
        height = static_cast<int32_t>(300 * dpiY / 96);
        win = createWindow(width, height);
        addOnCall(_readerStub);
    }

    virtual ~DX9DifferentDPITest() {
    }
};

IMG_TEST_F(DX9DifferentDPITest, BarEllipseX1_5){
    createWin(96.0f * 1.5f, 96.0f * 1.5f);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::BAR>(width/4, height/4, width/4*3, height/4*3),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0xFF00FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(width/2, height/2, static_cast<uint32_t>(width/5), static_cast<uint32_t>(height/2))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DifferentDPITest, BarEllipseX0_75){
    createWin(96.0f * 0.75f, 96.0f * 0.75f);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::BAR>(width/4, height/4, width/4*3, height/4*3),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0xFF00FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(width/2, height/2, static_cast<uint32_t>(width/5), static_cast<uint32_t>(height/2))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9DifferentDPITest, BarEllipseX1_5_0_75){
    createWin(96.0f * 1.5f, 96.0f * 0.75f);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::BAR>(width/4, height/4, width/4*3, height/4*3),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0xFF00FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(width/2, height/2, static_cast<uint32_t>(width/5), static_cast<uint32_t>(height/2))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}


IMG_TEST_F(DX9DifferentDPITest, BarEllipseHatchFillX1_5_0_75){
    createWin(96.0f * 1.5f, 96.0f * 0.75f);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::BAR>(width/4, height/4, width/4*3, height/4*3),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0xFF00FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(width/2, height/2, static_cast<uint32_t>(width/5), static_cast<uint32_t>(height/2))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}