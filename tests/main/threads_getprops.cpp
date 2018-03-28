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

class ThreadsGetpropsTest : public ::testing::Test {
public:
    MyWindowStub *win;
    RendererStub *ren;
    CommonProps props;
    PixelContainerCreator *fact;
protected:
    ThreadsGetpropsTest() {
        ren = new NiceMock<RendererStub>();
        win = new NiceMock<MyWindowStub>();
        fact = new PixelContainerCreator(500, 500, ColorFormat::A8R8G8B8);
        ON_CALL(*win, getRenderer()).WillByDefault(Return(ren));
        ON_CALL(*ren, draw(_)).WillByDefault(Invoke(ren, &RendererStub::drawImpl));
        ON_CALL(*ren, prepare(_)).WillByDefault(Invoke(ren, &RendererStub::prepareImpl));
        ON_CALL(*ren, getPixContFactory()).WillByDefault(Return(fact));
        props = get_default_common_props();
    }

    virtual ~ThreadsGetpropsTest() {
        delete ren;
        delete fact;
        delete win;
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(ThreadsGetpropsTest, OneThreadGetColor){
    ThreadController tc(win, props);
    tc.init();
    tc.setcolor(0x0000FF);
    EXPECT_EQ(tc.getcolor(), 0x0000FF);
}

TEST_F(ThreadsGetpropsTest, OneThreadFillPattern){
    ThreadController tc(win, props);
    tc.init();
    uint8_t pat[] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t pat2[FPATTERN_SIZE];
    tc.setfillpattern(reinterpret_cast<const char *>(pat), 0xFF0000);
    tc.getfillpattern(reinterpret_cast<char *>(pat2));
    std::vector<uint8_t> v1(pat, pat + FPATTERN_SIZE), v2(pat2, pat2 + FPATTERN_SIZE);
    EXPECT_EQ(v1, v2);
    fillsettingstype fsett;
    tc.getfillsettings(&fsett);
    EXPECT_EQ(fsett.pattern, USER_FILL);
    EXPECT_EQ(fsett.color, 0xFF0000);
    tc.repaint();
    delete [] ren->getData()[1].data.setfillpattern.fillPattern;
}

struct RepParam{
    ThreadController *controller;
};

static DWORD WINAPI ThreadHelper(LPVOID param){
    RepParam *p = static_cast<RepParam *>(param);
    p->controller->setbgcolor(0x0000FF);
    return 0;
}

TEST_F(ThreadsGetpropsTest, TwoThreadsGetBgColor){
    ThreadController tc(win, props);
    tc.init();
    tc.setbgcolor(0x00FF00);
    RepParam p = {&tc};
    WaitForSingleObject(CreateThread(NULL, 0, ThreadHelper, &p, 0, NULL), INFINITE);
    EXPECT_EQ(tc.getbgcolor(), 0x0000FF);
}