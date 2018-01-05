#include <gtest/gtest.h>
#include <WindowManager.h>
#include <tests/stubs/WindowFactoryStub.h>
#include <tests/stubs/ControllerFactoryStub.h>
#include <tests/stubs/MyWindowStub.h>
#include <tests/stubs/ControllerStub.h>

using namespace directgraph;

using testing::_;
using testing::Return;
using testing::NiceMock;

namespace {
    class WindowManagerTest : public ::testing::Test {
    protected:
        WindowManager *m;
        WindowFactoryStub *wf;
        ControllerFactoryStub *cf;
        ControllerStub *cstub;
        MyWindowStub *wstub;
        WindowManagerTest() {
            wf = new NiceMock<WindowFactoryStub>();
            wstub = new NiceMock<MyWindowStub>();
            ON_CALL(*wf, createPixelWindow(_, _, _)).WillByDefault(Return(wstub));
            cf = new NiceMock<ControllerFactoryStub>();
            cstub = new NiceMock<ControllerStub>();
            ON_CALL(*cf, createMultThreadController(_)).WillByDefault(Return(cstub));

            std::vector<IWindowFactory *> facts;
            facts.push_back(wf);
            m = new WindowManager(facts, cf);

        }

        virtual ~WindowManagerTest() {
            delete m;
            delete wstub;
            delete cstub;
        }

        virtual void SetUp() {
        }

        virtual void TearDown() {
        }
    };
}

TEST_F(WindowManagerTest, Simple){
    DirectgraphWinParams p = {100, 100, L"Test", DIRECTGRAPH_MULT_THREAD_CTRL, DIRECTGRAPH_DX9_RENDERER};
    DirectgraphWinIndex ind = m->createWindow(p);
    EXPECT_EQ(m->getCurrentWindowAndLock().index, ind);
    m->releaseCurrentWindowLock();
    m->destroyWindow(ind);
}

TEST_F(WindowManagerTest, MultWindows){
    DirectgraphWinParams p = {100, 100, L"Test", DIRECTGRAPH_MULT_THREAD_CTRL, DIRECTGRAPH_DX9_RENDERER};
    DirectgraphWinIndex ind1 = m->createWindow(p);
    DirectgraphWinIndex ind2 = m->createWindow(p);
    EXPECT_EQ(m->getCurrentWindowAndLock().index, ind1);
    m->releaseCurrentWindowLock();
    m->destroyWindow(ind1);
    m->destroyWindow(ind2);
}

TEST_F(WindowManagerTest, MultWindowsSetCur){
    DirectgraphWinParams p = {100, 100, L"Test", DIRECTGRAPH_MULT_THREAD_CTRL, DIRECTGRAPH_DX9_RENDERER};
    DirectgraphWinIndex ind1 = m->createWindow(p);
    DirectgraphWinIndex ind2 = m->createWindow(p);
    m->setCurrentWindow(ind2);
    EXPECT_EQ(m->getCurrentWindowAndLock().index, ind2);
    m->releaseCurrentWindowLock();
    m->destroyWindow(ind1);
    m->destroyWindow(ind2);
}

TEST_F(WindowManagerTest, MultWindowsCurNull){
    DirectgraphWinParams p = {100, 100, L"Test", DIRECTGRAPH_MULT_THREAD_CTRL, DIRECTGRAPH_DX9_RENDERER};
    DirectgraphWinIndex ind1 = m->createWindow(p);
    DirectgraphWinIndex ind2 = m->createWindow(p);
    m->setCurrentWindow(ind2);
    m->destroyWindow(ind1);
    m->destroyWindow(ind2);
    EXPECT_EQ(m->getCurrentWindowAndLock().controller, (IController*)NULL);
    m->releaseCurrentWindowLock();
}

TEST_F(WindowManagerTest, WindowGetCtrl){
    DirectgraphWinParams p = {100, 100, L"Test", DIRECTGRAPH_MULT_THREAD_CTRL, DIRECTGRAPH_DX9_RENDERER};
    DirectgraphWinIndex ind1 = m->createWindow(p);
    IController *ctrl = m->getCurrentWindowAndLock().controller;
    m->releaseCurrentWindowLock();
    EXPECT_EQ(ctrl, m->getWindowByIndexAndLock(ind1));
    m->releaseWindowLock();
    m->destroyWindow(ind1);
}