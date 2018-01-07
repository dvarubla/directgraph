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
    EXPECT_EQ(m->getCurrentWindowAndLock(false).controller, (IController*)NULL);
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

static const int MSG_CODE = WM_USER + 6000;

struct WinParam{
    WindowManager *m;
    uint_fast32_t maxSleep;
    std::vector<DirectgraphWinIndex> *createIndices;
    std::vector<DirectgraphWinIndex> *deleteIndices;
    DWORD id;
};

static DWORD WINAPI TwoThreadsHelper(LPVOID param){
    WinParam *p = static_cast<WinParam *>(param);
    PostThreadMessage(p->id, MSG_CODE, 0, 0);
    MSG msg;
    DirectgraphWinParams winParams = {100, 100, L"Test", DIRECTGRAPH_MULT_THREAD_CTRL, DIRECTGRAPH_DX9_RENDERER};
    for(uint32_t i = 0; i < 100; i++){
        p->createIndices->push_back(p->m->createWindow(winParams));
        p->m->getCurrentWindowAndLock(false);
        Sleep(rand() % p->maxSleep);
        p->m->releaseCurrentWindowLock();
    }

    PostThreadMessage(p->id, MSG_CODE, 0, 0);
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);

    for(uint32_t i = 0; i < 100; i++){
        GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
        p->m->destroyWindow(static_cast<DirectgraphWinIndex>(msg.wParam));
        p->deleteIndices->push_back(static_cast<DirectgraphWinIndex>(msg.wParam));
        p->m->getCurrentWindowAndLock(false);
        Sleep(rand() % 5);
        p->m->releaseCurrentWindowLock();
    }

    PostThreadMessage(p->id, MSG_CODE, 0, 0);
    return 0;
}

TEST_F(WindowManagerTest, TwoThreads){
    DirectgraphWinParams p = {100, 100, L"Test", DIRECTGRAPH_MULT_THREAD_CTRL, DIRECTGRAPH_DX9_RENDERER};
    std::vector<DirectgraphWinIndex> createIndices1, createIndices2, deleteIndices2, allIndices;
    WinParam winParam = {m, 5, &createIndices2, &deleteIndices2, GetCurrentThreadId()};
    DWORD delThreadId;
    CreateThread(NULL, 0, TwoThreadsHelper, &winParam, 0, &delThreadId);
    MSG msg;
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    for(uint32_t i = 0; i < 100; i++){
        createIndices1.push_back(m->createWindow(p));
        m->getCurrentWindowAndLock(false);
        Sleep(rand() % 5);
        m->releaseCurrentWindowLock();
    }
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);

    std::copy(createIndices1.cbegin(), createIndices1.cend(), std::back_inserter(allIndices));
    std::copy(createIndices2.cbegin(), createIndices2.cend(), std::back_inserter(allIndices));
    std::sort(allIndices.begin(), allIndices.end());
    ASSERT_EQ(allIndices.size(), 200);
    for(uint32_t i = 1; i <= 200; i++){
        EXPECT_EQ(allIndices[i - 1], i);
    }

    PostThreadMessage(delThreadId, MSG_CODE, 0, 0);

    for(uint32_t i = 0; i < 100; i++){
        m->destroyWindow(createIndices1[i]);
        PostThreadMessage(delThreadId, MSG_CODE, createIndices2[i], 0);
        m->getCurrentWindowAndLock(false);
        Sleep(rand() % 5);
        m->releaseCurrentWindowLock();
    }
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    std::sort(createIndices2.begin(), createIndices2.end());
    std::sort(deleteIndices2.begin(), deleteIndices2.end());
    EXPECT_EQ(createIndices2, deleteIndices2);
}