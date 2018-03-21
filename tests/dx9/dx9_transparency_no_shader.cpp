#include <gtest/gtest.h>
#include <dx9/WindowFactory.h>
#include <directgraph/directgraph_constants.h>
#include <tests/stubs/QueueReaderStub.h>

#include <testlib/testlib_funcs.h>
#include "common.h"

using testing::NiceMock;

class DX9TransparencyNoShaderTest: public ImageTest, public CommonSimple{
public:
    NiceMock<QueueReaderStub> _readerStub;
    IMyWindow *win;
    static int32_t WIDTH;
    static int32_t HEIGHT;
    IFeatures::ShaderVersion noShVer;

    DX9TransparencyNoShaderTest() {
    }

    void setupTest(ColorFormat::Format mainFormat, bool haveConstant, ColorFormat::Format transpFormat){
        IFeatures::ShaderVersion t = {0, 0};
        noShVer = t;
        startThread();
        FeaturesStub *fstub = setupFeatures();
        ON_CALL(*fstub, getPatternTexFormat()).WillByDefault(testing::Return(mainFormat));
        ON_CALL(*fstub, supportsTexConst()).WillByDefault(testing::Return(haveConstant));
        ON_CALL(*fstub, getTranspPatternTexFormat()).WillByDefault(testing::Return(transpFormat));
        ON_CALL(*fstub, getVertexShaderVer()).WillByDefault(testing::Return(noShVer));
        ON_CALL(*fstub, getPixelShaderVer()).WillByDefault(testing::Return(noShVer));
        win = createWindow(WIDTH, HEIGHT);
        addOnCall(_readerStub);
    }

    virtual ~DX9TransparencyNoShaderTest() {
    }
};

int32_t DX9TransparencyNoShaderTest::WIDTH = 200;
int32_t DX9TransparencyNoShaderTest::HEIGHT = 300;

IMG_TEST_F(DX9TransparencyNoShaderTest, TwoBars){
    setupTest(ColorFormat::A8R8G8B8, true, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x7FFF0000),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyNoShaderTest, EllipsesRectanglesLinestyle){
    setupTest(ColorFormat::AR5G5B5, false, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),
            QueueItemCreator::create<QueueItem::COLOR>(0x7FFF0000),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(DOTTED_LINE, 0, 5),
            QueueItemCreator::create<QueueItem::RECTANGLE>(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(USERBIT_LINE, 0x1111, 5),
            QueueItemCreator::create<QueueItem::COLOR>(0x7F00FF00),
            QueueItemCreator::create<QueueItem::RECTANGLE>(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyNoShaderTest, TwoEllipses){
    setupTest(ColorFormat::A8R8G8B8, true, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x7FFF0000),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(3*WIDTH/4, 3*HEIGHT/4, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyNoShaderTest, TwoBarsHatchFill32bitColConst){
    setupTest(ColorFormat::A8R8G8B8, true, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x7FFF0000),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL,0x7F00FF00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyNoShaderTest, TwoEllipsesHatchFill32bitColConst){
    setupTest(ColorFormat::A8R8G8B8, true, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x7FFF0000),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL,0x7F00FF00),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(3*WIDTH/4, 3*HEIGHT/4, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyNoShaderTest, MultipleItems32bitColConst){
    setupTest(ColorFormat::A8R8G8B8, true, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x44AAAAAA),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(BKSLASH_FILL, 0x7F0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/3, static_cast<uint32_t>(WIDTH/3 + 1), static_cast<uint32_t>(HEIGHT/3)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SLASH_FILL, 0x7FFF0000),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/3 + 1, HEIGHT/2, static_cast<uint32_t>(WIDTH/3 + 1), static_cast<uint32_t>(HEIGHT/3)),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0xFFFFFF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0xAA00AA),
            QueueItemCreator::create<QueueItem::BAR>(2*WIDTH/5, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0xFF000000),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),

    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}


IMG_TEST_F(DX9TransparencyNoShaderTest, TwoBarsHatchFill16bitColConst){
    setupTest(ColorFormat::AR5G5B5, true, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x7FFF0000),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL,0x7F00FF00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyNoShaderTest, TwoEllipsesHatchFill16bitColConst){
    setupTest(ColorFormat::AR5G5B5, true, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x7FFF0000),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL,0x7F00FF00),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(3*WIDTH/4, 3*HEIGHT/4, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyNoShaderTest, MultipleItems16bitColConst){
    setupTest(ColorFormat::AR5G5B5, true, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x44AAAAAA),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(BKSLASH_FILL, 0x7F0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/3, static_cast<uint32_t>(WIDTH/3 + 1), static_cast<uint32_t>(HEIGHT/3)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SLASH_FILL, 0x7FFF0000),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/3 + 1, HEIGHT/2, static_cast<uint32_t>(WIDTH/3 + 1), static_cast<uint32_t>(HEIGHT/3)),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0xFFFFFF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0xAA00AA),
            QueueItemCreator::create<QueueItem::BAR>(2*WIDTH/5, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0xFF000000),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),

    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyNoShaderTest, TwoBarsHatchFill32bitColNoConst){
    setupTest(ColorFormat::A8R8G8B8, false, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x7FFF0000),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL,0x7F00FF00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyNoShaderTest, TwoEllipsesHatchFill32bitColNoConst){
    setupTest(ColorFormat::A8R8G8B8, false, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x7FFF0000),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL,0x7F00FF00),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(3*WIDTH/4, 3*HEIGHT/4, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyNoShaderTest, MultipleItems32bitColNoConst){
    setupTest(ColorFormat::A8R8G8B8, false, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x44AAAAAA),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(BKSLASH_FILL, 0x7F0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/3, static_cast<uint32_t>(WIDTH/3 + 1), static_cast<uint32_t>(HEIGHT/3)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SLASH_FILL, 0x7FFF0000),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/3 + 1, HEIGHT/2, static_cast<uint32_t>(WIDTH/3 + 1), static_cast<uint32_t>(HEIGHT/3)),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0xFFFFFF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0xAA00AA),
            QueueItemCreator::create<QueueItem::BAR>(2*WIDTH/5, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0xFF000000),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),

    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyNoShaderTest, TwoBarsHatchFill16bitColNoConst){
    setupTest(ColorFormat::AR5G5B5, false, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x7FFF0000),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL,0x7F00FF00),
            QueueItemCreator::create<QueueItem::BAR>(WIDTH/2, HEIGHT/2, WIDTH, HEIGHT)
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyNoShaderTest, TwoEllipsesHatchFill16bitColNoConst){
    setupTest(ColorFormat::AR5G5B5, false, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SOLID_FILL, 0x0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x7FFF0000),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL,0x7F00FF00),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(3*WIDTH/4, 3*HEIGHT/4, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4))
    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}

IMG_TEST_F(DX9TransparencyNoShaderTest, MultipleItems16bitColNoConst){
    setupTest(ColorFormat::AR5G5B5, false, ColorFormat::A4R4G4B4);
    QueueItem items[] = {
            QueueItemCreator::create<QueueItem::CLEAR>(),
            QueueItemCreator::create<QueueItem::SETLINESTYLE>(NULL_LINE, 0, 0),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x44AAAAAA),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(BKSLASH_FILL, 0x7F0000FF),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/3, static_cast<uint32_t>(WIDTH/3 + 1), static_cast<uint32_t>(HEIGHT/3)),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(SLASH_FILL, 0x7FFF0000),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/3 + 1, HEIGHT/2, static_cast<uint32_t>(WIDTH/3 + 1), static_cast<uint32_t>(HEIGHT/3)),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0xFFFFFF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(HATCH_FILL, 0xAA00AA),
            QueueItemCreator::create<QueueItem::BAR>(2*WIDTH/5, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3),
            QueueItemCreator::create<QueueItem::BGCOLOR>(0x0000FF),
            QueueItemCreator::create<QueueItem::SETFILLSTYLE>(LINE_FILL, 0xFF000000),
            QueueItemCreator::create<QueueItem::FILLELLIPSE>(WIDTH/2, HEIGHT/2, static_cast<uint32_t>(WIDTH/4), static_cast<uint32_t>(HEIGHT/4)),

    };
    _readerStub.addItems(items, sizeof(items) / sizeof(QueueItem));
    return afterTestSimple(win, &_readerStub);
}