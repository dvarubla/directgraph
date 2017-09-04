#include <gtest/gtest.h>
#include <testlib/testlib_funcs.h>
#include <d2d/D2DWindowFactory.h>

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

static D2DWindowFactory *_d2dWf = NULL;

void init_factory(){
    if(_d2dWf == NULL) {
        _d2dWf = new D2DWindowFactory();
    }
}

IMG_TEST_F(BarTest, SimpleBar){
    init_factory();
    float width = 200;
    float height = 300;
    MyWindow *win = _d2dWf->createPixelWindow(L"Hello", width, height);
    win->show();
    win->getRenderer()->setcolor(0x00BBFF);
    win->getRenderer()->bar(width/4, height/4, width/4*3, height/4*3);
    win->getRenderer()->repaint();
    BitmapWrap *ret = new BitmapWrap(win->getHWND());
    delete win;
    return ret;
}

IMG_TEST_F(BarTest, InverseCoords){
    init_factory();
    float width = 200;
    float height = 300;
    MyWindow *win = _d2dWf->createPixelWindow(L"Hello", width, height);
    win->show();
    win->getRenderer()->setcolor(0x00BBFF);
    win->getRenderer()->bar(width/4*3, height/4*3, width/4, height/4);
    win->getRenderer()->repaint();
    BitmapWrap *ret = new BitmapWrap(win->getHWND());
    delete win;
    return ret;
}

IMG_TEST_F(BarTest, OnePixel){
    init_factory();
    float width = 200;
    float height = 300;
    MyWindow *win = _d2dWf->createPixelWindow(L"Hello", width, height);
    win->show();
    win->getRenderer()->setcolor(0);
    win->getRenderer()->bar(width/4, height/4, width/4 + 1, height/4 + 1);
    win->getRenderer()->repaint();
    BitmapWrap *ret = new BitmapWrap(win->getHWND());
    delete win;
    return ret;
}

IMG_TEST_F(BarTest, OnePxBorder){
    init_factory();
    float width = 200;
    float height = 300;
    MyWindow *win = _d2dWf->createPixelWindow(L"Hello", width, height);
    win->show();
    win->getRenderer()->setcolor(0x0000FF);
    win->getRenderer()->bar(0,0,width,height);
    win->getRenderer()->setcolor(0x00FFFF);
    win->getRenderer()->bar(1,1,width - 1 ,height - 1);
    win->getRenderer()->repaint();
    BitmapWrap *ret = new BitmapWrap(win->getHWND());
    delete win;
    return ret;
}