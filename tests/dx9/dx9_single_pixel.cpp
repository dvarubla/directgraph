#include <gtest/gtest.h>
#include <testlib/testlib_funcs.h>
#include <dx9/DX9WindowFactory.h>
#include <main/ThreadController.h>

using namespace directgraph;
using namespace directgraph_testlib;

namespace {
    class SinglePixelTest : public ::testing::Test {
    public:

    protected:

        SinglePixelTest() {
        }

        virtual ~SinglePixelTest() {
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

IMG_TEST_F(SinglePixelTest, OnePixel){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    ThreadController tc(win);
    tc.init();
    tc.putpixel(static_cast<int_fast32_t>(WIDTH / 4), static_cast<int_fast32_t>(HEIGHT / 4), 0x000000);
    tc.repaint();
    return new BitmapWrap(win->getHWND());
}

IMG_TEST_F(SinglePixelTest, PixelLine){
    init_factory();
    MyWindow *win = _dx9Wf->createPixelWindow(L"Hello", WIDTH, HEIGHT);
    win->show();
    ThreadController tc(win);
    tc.init();
    for(int_fast32_t i = 0; i < WIDTH; i += 2){
        tc.putpixel(i, 0, 0x000000);
    }
    for(int_fast32_t i = 1; i < WIDTH; i += 2){
        tc.putpixel(i, 1, 0xFF00FF);
    }
    tc.repaint();
    return new BitmapWrap(win->getHWND());
}