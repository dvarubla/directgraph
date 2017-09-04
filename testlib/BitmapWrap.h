#ifndef DIRECTGRAPH_TESTLIB_BITMAPWRAP_H
#define DIRECTGRAPH_TESTLIB_BITMAPWRAP_H


#include <windows.h>
#include <gdiplus.h>

namespace directgraph_testlib {
    class BitmapWrap {
    private:
        Gdiplus::Bitmap *_bitmap;
    public:
        BitmapWrap(HWND win);
        Gdiplus::Bitmap * getBitmap();
    };
}

#endif //DIRECTGRAPH_TESTLIB_BITMAPWRAP_H
