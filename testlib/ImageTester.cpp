#include <windows.h>
#include <gdiplus.h>
#include "ImageTester.h"
#include "BitmapWrap.h"
#include <testlib_funcs.h>
#include <vector>
#include <iostream>
#include <gtest/gtest.h>

namespace directgraph_testlib {
    void ImageTester::saveAll() {
        CLSID encoderClsid;
        getEncoderClsid(L"image/png", &encoderClsid);
        for(
            std::vector<TestInfo>::const_iterator i = getTestInfos().cbegin();
            i != getTestInfos().cend();
            ++i
        ) {
            ImageTest *test = i->test->getObj();
            BitmapWrap *bitmapWrap = test->imageTestRun();
            delete test;
            Gdiplus::Bitmap *bmp = bitmapWrap->getBitmap();
            std::wstring filename = getFullPath(i->str);
            bmp->Save(filename.c_str(), &encoderClsid, NULL);
            delete bitmapWrap;
            delete bmp;
        }
    }

    void ImageTester::setImageDir(const std::wstring &imageDir) {
        _imageDir = imageDir;
    }

    ImageTester::ImageTester() {
    }

    void ImageTester::getEncoderClsid(const wchar_t *format, CLSID *pClsid) {
        uint_fast32_t num = 0;
        uint_fast32_t size = 0;

        Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
        Gdiplus::GetImageEncodersSize(&num, &size);
        pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
        GetImageEncoders(num, size, pImageCodecInfo);
        for(uint_fast32_t j = 0; j < num; j++){
            if( std::wstring(pImageCodecInfo[j].MimeType) == format ){
                *pClsid = pImageCodecInfo[j].Clsid;
                free(pImageCodecInfo);
                return;
            }
        }
        free(pImageCodecInfo);
    }

    void ImageTester::test(BitmapWrap *bitmapWrap, const std::wstring &name) {
        std::wstring filename = getFullPath(name);
        Gdiplus::Bitmap *bmpFile = new Gdiplus::Bitmap(filename.c_str());
        Gdiplus::Bitmap *bmpWin = bitmapWrap->getBitmap();
        delete bitmapWrap;
        ASSERT_EQ(bmpFile->GetWidth(), bmpWin->GetWidth()) << "Widths differ";
        ASSERT_EQ(bmpFile->GetHeight(), bmpWin->GetHeight()) << "Heights differ";
        uint_fast16_t width = bmpWin->GetWidth(), height = bmpWin->GetHeight();
        Gdiplus::Rect lockRect = Gdiplus::Rect(0, 0, bmpWin->GetWidth(), bmpWin->GetHeight());
        Gdiplus::BitmapData bmpFileData, bmpWinData;
        bmpFile->LockBits(
                &lockRect,
                Gdiplus::ImageLockModeRead, PixelFormat32bppARGB,
                &bmpFileData
        );
        bmpWin->LockBits(
                &lockRect,
                Gdiplus::ImageLockModeRead, PixelFormat32bppARGB,
                &bmpWinData
        );
        uint32_t *pixelDataFile = (uint32_t*)bmpFileData.Scan0;
        uint32_t *pixelDataWin = (uint32_t*)bmpWinData.Scan0;
        for(uint_fast16_t i = 0; i < height; i++){
            for(uint_fast16_t j = 0; j < width; j++){
                ASSERT_EQ(
                        pixelDataFile[i * bmpWinData.Stride / 4 + j],
                        pixelDataWin[i * bmpFileData.Stride / 4 + j]
                ) << "Pixels differ at (" << i <<", " << j<< ")";
            }
        }
        bmpFile->UnlockBits(&bmpFileData);
        bmpWin->UnlockBits(&bmpWinData);
        delete bmpFile;
        delete bmpWin;
    }

    std::wstring ImageTester::getFullPath(const std::wstring &name) {
        return _imageDir + L"/" + name + L".png";;
    }
}
