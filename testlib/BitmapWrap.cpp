#include <stdint.h>
#include "BitmapWrap.h"

directgraph_testlib::BitmapWrap::BitmapWrap(HWND win) {
    RECT rect;
    GetClientRect(win, &rect);
    //uint_fast32_t width = GetSystemMetrics(SM_CXSCREEN);//(uint_fast32_t)rect.right;
    //uint_fast32_t height = GetSystemMetrics(SM_CYSCREEN);//(uint_fast32_t)rect.bottom;
    //HDC hdc = GetDC(GetDesktopWindow());
    uint_fast32_t width = (uint_fast32_t)rect.right;
    uint_fast32_t height = (uint_fast32_t)rect.bottom;
    HDC hdc = GetDC(win);

    HDC compHdc = CreateCompatibleDC(hdc);

    HBITMAP hbmp = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(compHdc, hbmp);
    BitBlt(compHdc, 0, 0, width, height, hdc, 0, 0, SRCCOPY | CAPTUREBLT);
    Gdiplus::Bitmap *bmp = new Gdiplus::Bitmap(hbmp, NULL);
    DeleteObject(hbmp);
    DeleteDC(compHdc);
    ReleaseDC(win, hdc);
    _bitmap = bmp;
}

Gdiplus::Bitmap *directgraph_testlib::BitmapWrap::getBitmap() {
    return _bitmap;
}
