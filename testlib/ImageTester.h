#ifndef DIRECTGRAPH_TESTLIB_IMAGETESTER_H
#define DIRECTGRAPH_TESTLIB_IMAGETESTER_H

#include <string>
#include <gdiplus.h>
#include "BitmapWrap.h"

namespace directgraph_testlib {
    class ImageTester {
    private:
        std::wstring _imageDir;
        void getEncoderClsid(const wchar_t* format, CLSID* pClsid);
        std::wstring getFullPath(const std::wstring &name);
    public:
        void setImageDir(const std::wstring &imageDir);
        ImageTester();
        void saveAll();
        void test(BitmapWrap *bmpWin, const std::wstring &name);
    };
}

#endif //DIRECTGRAPH_TESTLIB_IMAGETESTER_H
