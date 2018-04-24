#include <gtest/gtest.h>
#include <main/ThreadController.h>
#include <directgraph/directgraph_constants.h>
#include <RendererStub.h>
#include <tests/stubs/QueueItemUtils.h>
#include <tests/stubs/MyWindowStub.h>
#include <patterns.h>
#include <QueueItem.h>

using namespace directgraph;
using testing::Return;
using testing::Invoke;
using testing::WithArgs;
using testing::NiceMock;
using testing::_;

class PaletteTest : public ::testing::Test {
public:
    MyWindowStub *win;
    RendererStub *ren;
    CommonProps props;
    PixelContainerCreator *fact;
protected:
    PaletteTest() {
        ren = new NiceMock<RendererStub>();
        win = new NiceMock<MyWindowStub>();
        fact = new PixelContainerCreator(500, 500, ColorFormat::A8R8G8B8);
        ON_CALL(*win, getRenderer()).WillByDefault(Return(ren));
        ON_CALL(*ren, draw(_)).WillByDefault(Invoke(ren, &RendererStub::drawImpl));
        ON_CALL(*ren, prepare(_)).WillByDefault(Invoke(ren, &RendererStub::prepareImpl));
        ON_CALL(*ren, getPixContFactory()).WillByDefault(Return(fact));
        props = get_default_common_props();
    }

    virtual ~PaletteTest() {
        delete ren;
        delete fact;
        delete win;
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(PaletteTest, OneThread16ColorsSimple){
    ThreadController tc(win, props);
    tc.init();
    tc.initpalette(true, 16, true);
    tc.setcolor(15);
    tc.bar(0,0,1,1);
    tc.setcolor(0);
    tc.repaint();
    EXPECT_EQ(ren->getData()[1].data.color, 0xFFFFFF);
    EXPECT_EQ(ren->getData()[3].data.color, 0);
}

TEST_F(PaletteTest, OneThread16ColorsChangeColor){
    ThreadController tc(win, props);
    tc.init();
    tc.initpalette(true, 16, true);
    tc.setpalette(15, 0xABCDEF0);
    tc.setcolor(15);
    tc.repaint();
    EXPECT_EQ(ren->getData()[1].data.color, 0xABCDEF0);
}

TEST_F(PaletteTest, OneThread16ColorsClearPalette){
    ThreadController tc(win, props);
    tc.init();
    tc.initpalette(true, 16, true);
    tc.setpalette(15, 0xABCDEF0);
    tc.clearpalette();
    tc.setcolor(15);
    tc.repaint();
    EXPECT_EQ(ren->getData()[1].data.color, 0xFFFFFF);
}

struct RepParam{
    ThreadController *controller;
};

static DWORD WINAPI ThreadHelper(LPVOID param){
    RepParam *p = static_cast<RepParam *>(param);
    p->controller->setpalette(11, 0xABCDEF0);
    return 0;
}

TEST_F(PaletteTest, TwoThreads256ColorsChangeColor){
    ThreadController tc(win, props);
    tc.init();
    tc.initpalette(true, 256, false);
    RepParam p = {&tc};
    WaitForSingleObject(CreateThread(NULL, 0, ThreadHelper, &p, 0, NULL), INFINITE);
    tc.setbgcolor(11);
    tc.repaint();
    EXPECT_EQ(ren->getData()[1].data.bgColor, 0xABCDEF0);
}