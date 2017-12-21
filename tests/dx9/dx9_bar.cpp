#include <gtest/gtest.h>
#include <testlib/testlib_funcs.h>
#include <dx9/DX9WindowFactory.h>
#include <include/directgraph_mainloop.h>
#include <main/ThreadController.h>

using namespace directgraph;
using namespace directgraph_testlib;

namespace {
    class BarTest : public ::testing::Test {
    public:

    protected:

        BarTest() {
        }

        virtual ~BarTest() {
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

IMG_TEST_F(BarTest, SimpleBar){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    ThreadController tc(win);
    tc.init();
    tc.setcolor(0x00BBFF);
    tc.bar(WIDTH/4, HEIGHT/4, WIDTH/4*3, HEIGHT/4*3);
    tc.repaint();
    return new BitmapWrap(win->getHWND());
}

IMG_TEST_F(BarTest, InverseCoords){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    ThreadController tc(win);
    tc.init();
    tc.setcolor(0x00BBFF);
    tc.bar(WIDTH/4*3, HEIGHT/4*3, WIDTH/4, HEIGHT/4);
    tc.repaint();
    return new BitmapWrap(win->getHWND());
}

IMG_TEST_F(BarTest, OutsideScreen){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    ThreadController tc(win);
    tc.init();
    tc.setcolor(0x00BBFF);
    tc.bar(-WIDTH/4, -HEIGHT/4, WIDTH/4*3, HEIGHT/4*3);
    tc.repaint();
    return new BitmapWrap(win->getHWND());
}

IMG_TEST_F(BarTest, OutsideScreenFull){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    ThreadController tc(win);
    tc.init();
    tc.setcolor(0x00BBFF);
    tc.bar(-WIDTH/4, -HEIGHT/4, -WIDTH/4*3, -HEIGHT/4*3);
    tc.repaint();
    return new BitmapWrap(win->getHWND());
}

IMG_TEST_F(BarTest, OnePixel){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    ThreadController tc(win);
    tc.init();
    tc.setcolor(0);
    tc.bar(WIDTH/4, HEIGHT/4, WIDTH/4 + 1, HEIGHT/4 + 1);
    tc.repaint();
    return new BitmapWrap(win->getHWND());
}

IMG_TEST_F(BarTest, OnePxBorder){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    ThreadController tc(win);
    tc.init();
    tc.setcolor(0x0000FF);
    tc.bar(0,0,WIDTH,HEIGHT);
    tc.setcolor(0x00FFFF);
    tc.bar(1,1,WIDTH - 1 ,HEIGHT - 1);
    tc.repaint();
    return new BitmapWrap(win->getHWND());
}