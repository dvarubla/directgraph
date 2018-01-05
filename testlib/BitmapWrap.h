#pragma once

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
