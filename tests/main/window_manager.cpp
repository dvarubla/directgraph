#include <gtest/gtest.h>
#include <WindowManager.h>
#include <tests/stubs/WindowFactoryStub.h>

using namespace directgraph;

namespace {
    class WindowManagerTest : public ::testing::Test {
    protected:
        WindowManager *m;
        WindowManagerTest() {
            std::vector<IWindowFactory *> facts = {new WindowFactoryStub()};
            m = new WindowManager(facts);
        }

        virtual ~WindowManagerTest() {
            delete m;
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