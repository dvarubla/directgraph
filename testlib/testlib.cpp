#include <windows.h>
#include <gdiplus.h>
#include <gtest/gtest.h>
#include <testlib_funcs.h>

int wmain(int argc, wchar_t **argv) {
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    if(argc > 1){
        directgraph_testlib::ImageTester *tester = directgraph_testlib::getTester();
        if(argc > 2) {
            if (std::wstring(argv[1]) == L"-gen") {
                std::wstring path(argv[2]);
                tester->setImageDir(path);
                tester->saveAll();
                return 0;
            }
        }
        std::wstring path(argv[1]);
        tester->setImageDir(path);
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
    return 1;
}