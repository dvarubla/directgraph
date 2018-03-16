#include <gtest/gtest.h>
#include <dx9/WindowFactory.h>
#include <directgraph/directgraph_constants.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9FillEllipseOldTest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    IFeatures::ShaderVersion noShVer;
    static int32_t WIDTH;
    static int32_t HEIGHT;

    DX9FillEllipseOldTest() {
        IFeatures::ShaderVersion t = {0, 0};
        noShVer = t;
        startThread();
        FeaturesStub *fstub = setupFeatures();
        ON_CALL(*fstub, getVertexShaderVer()).WillByDefault(testing::Return(noShVer));
        ON_CALL(*fstub, getPixelShaderVer()).WillByDefault(testing::Return(noShVer));
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    virtual ~DX9FillEllipseOldTest() {
    }
};

int32_t DX9FillEllipseOldTest::WIDTH = 200;
int32_t DX9FillEllipseOldTest::HEIGHT = 200;

IMG_TEST_F(DX9FillEllipseOldTest, SimpleEllipse){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/4, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/2))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseOldTest, SimpleEllipseReverse){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/4, static_cast<uint32_t>(WIDTH/2), static_cast<uint32_t>(HEIGHT/4))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseOldTest, Ellipse4){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/2), static_cast<uint32_t>(HEIGHT/2)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/4, HEIGHT/5, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/5)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0xFF0000),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(3*WIDTH/4, HEIGHT/5, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/5)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0xFFFF00),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, 4*HEIGHT/5, static_cast<uint32_t>(WIDTH/2), static_cast<uint32_t>(HEIGHT/5))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseOldTest, EllipseThenBar2){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/2), static_cast<uint32_t>(HEIGHT/2)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0xFF0000),
            QueueItemCreator::create<QueueItem::BAR>(0, 0, WIDTH/8, HEIGHT/8),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/4, HEIGHT/5, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/5)),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, HEIGHT/2, 3*WIDTH/4, 3*HEIGHT/4)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseOldTest, Ellipse1px){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(1, 1, 1u, 1u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseOldTest, Ellipse2to10px){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(10, 10, 2u, 2u),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(20, 20, 3u, 3u),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(30, 30, 4u, 4u),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(40, 40, 5u, 5u),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(50, 50, 6u, 6u),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(65, 65, 7u, 7u),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(80, 80, 8u, 8u),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(95, 95, 9u, 9u),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(110, 110, 10u, 10u)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9FillEllipseOldTest, EllipseLarge){
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x00BBFF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/2), static_cast<uint32_t>(HEIGHT/2))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}