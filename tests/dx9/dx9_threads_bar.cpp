#include <gtest/gtest.h>
#include <testlib/testlib_funcs.h>
#include <dx9/DX9WindowFactory.h>
#include <include/directgraph_mainloop.h>
#include <main/ThreadController.h>
#include <include/graphics_const.h>

using namespace directgraph;
using namespace directgraph_testlib;

namespace {
    class ThreadsBarTest : public ::testing::Test {
    public:
    protected:
        ThreadsBarTest() {
        }

        virtual ~ThreadsBarTest() {
        }

        virtual void SetUp() {
        }

        virtual void TearDown() {
        }
    };
}

static DX9WindowFactory *_dx9Wf = NULL;

void init_factory(){
    if(_dx9Wf == NULL) {
        _dx9Wf = new DX9WindowFactory();
    }
}

static float WIDTH = 200;
static float HEIGHT = 300;
static const int MSG_CODE = WM_USER + 6000;

IMG_TEST_F(ThreadsBarTest, BarOneThread){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    ThreadController tc(win);
    tc.init();
    uint_fast32_t cnt = 0;
    bool continueLoop = true;
    for(uint_fast32_t i = 0; i < HEIGHT && continueLoop; i++){
        for(uint_fast32_t j = 0; j < WIDTH; j++, cnt++){
            tc.setfillstyle(SOLID_FILL, cnt % 0xFF);
            tc.bar((float)j, (float)i, (float)j + 1, (float)i + 1);
        }
        if(i == 20) {
            continueLoop = false;
        }
    }
    tc.repaint();
    return new BitmapWrap(win->getHWND());
}

static DWORD WINAPI BarTwoThreadsLongInterval_helper(LPVOID param){
    ThreadController *tc = static_cast<ThreadController *>(param);
    while(true){
        MSG msg;
        if(PeekMessage(&msg, NULL, MSG_CODE, MSG_CODE, PM_REMOVE)){
            PostThreadMessage(msg.wParam, MSG_CODE, 0, 0);
            break;
        }
        tc->repaint();
        Sleep(rand() % 5000);
    }
    return 0;
}

IMG_TEST_F(ThreadsBarTest, BarTwoThreadsLongInterval){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    ThreadController tc(win);
    tc.init();
    DWORD threadId;
    CreateThread(NULL, 0, BarTwoThreadsLongInterval_helper, &tc, 0, &threadId);
    uint_fast32_t cnt = 0;
    bool continueLoop = true;
    for(uint_fast32_t i = 0; i < HEIGHT && continueLoop; i++){
        for(uint_fast32_t j = 0; j < WIDTH; j++, cnt++){
            tc.setfillstyle(SOLID_FILL, cnt % 0xFF);
            tc.bar((float)j, (float)i, (float)j + 1, (float)i + 1);
            Sleep(rand() % 5);
        }
        if(i == 20) {
            continueLoop = false;
        }
    }
    MSG msg;
    PostThreadMessage(threadId, MSG_CODE, GetCurrentThreadId(), 0);
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    tc.repaint();
    return new BitmapWrap(win->getHWND());
}

static DWORD WINAPI BarTwoThreadsShortInterval_helper(LPVOID param){
    ThreadController *tc = static_cast<ThreadController *>(param);
    while(true){
        MSG msg;
        if(PeekMessage(&msg, NULL, MSG_CODE, MSG_CODE, PM_REMOVE)){
            PostThreadMessage(msg.wParam, MSG_CODE, 0, 0);
            break;
        }
        tc->repaint();
        Sleep(rand() % 50);
    }
    return 0;
}

IMG_TEST_F(ThreadsBarTest, BarTwoThreadsShortInterval){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    ThreadController tc(win);
    tc.init();
    DWORD threadId;
    CreateThread(NULL, 0, BarTwoThreadsShortInterval_helper, &tc, 0, &threadId);
    uint_fast32_t cnt = 0;
    bool continueLoop = true;
    for(uint_fast32_t i = 0; i < HEIGHT && continueLoop; i++){
        for(uint_fast32_t j = 0; j < WIDTH; j++, cnt++){
            tc.setfillstyle(SOLID_FILL, cnt % 0xFF);
            tc.bar((float)j, (float)i, (float)j + 1, (float)i + 1);
            Sleep(rand() % 5);
        }
        if(i == 20) {
            continueLoop = false;
        }
    }
    MSG msg;
    PostThreadMessage(threadId, MSG_CODE, GetCurrentThreadId(), 0);
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    tc.repaint();
    return new BitmapWrap(win->getHWND());
}