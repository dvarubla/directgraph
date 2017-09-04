#ifndef DIRECTGRAPH_TESTLIB_FUNCS_H
#define DIRECTGRAPH_TESTLIB_FUNCS_H

#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include "ImageTester.h"
#include "BitmapWrap.h"

#define IMAGE_TEST_NAME(test_fixture, test_name)\
    test_fixture##_##test_name##_image_test

#ifdef __JETBRAINS_IDE__

#define IMG_TEST_F(test_fixture, test_name)\
    BitmapWrap* IMAGE_TEST_NAME(test_fixture, test_name)();\
    TEST_F(test_fixture, test_name){\
        IMAGE_TEST_NAME(test_fixture, test_name)();\
    }\
    BitmapWrap* IMAGE_TEST_NAME(test_fixture, test_name)()
#else

#define IMAGE_TEST_NAME(test_fixture, test_name)\
    test_fixture##_##test_name##_image_test

#define IMAGE_TEST_VARNAME(test_fixture, test_name)\
    test_fixture##_##test_name##_var

#define IMAGE_TEST_FNAME(test_fixture, test_name)\
    L#test_fixture"_"#test_name

#define IMG_TEST_F(test_fixture, test_name)\
    BitmapWrap* IMAGE_TEST_NAME(test_fixture, test_name)();\
    TEST_F(test_fixture, test_name){\
        directgraph_testlib::test_image(\
            IMAGE_TEST_NAME(test_fixture, test_name)(),\
            IMAGE_TEST_FNAME(test_fixture, test_name)\
        );\
    }\
    int IMAGE_TEST_VARNAME(test_fixture, test_name) = \
        directgraph_testlib::add_func(\
            IMAGE_TEST_NAME(test_fixture, test_name),\
            IMAGE_TEST_FNAME(test_fixture, test_name)\
    );\
    BitmapWrap* IMAGE_TEST_NAME(test_fixture, test_name)()
#endif

namespace directgraph_testlib {
    typedef struct TestInfo{
        BitmapWrap* (*func)();
        std::wstring str;
    } TestInfo;
    std::vector<TestInfo> &getTestInfos();
    ImageTester* getTester();
    int add_func(BitmapWrap* (*func)(), const std::wstring &name);
    void test_image(BitmapWrap* hwnd, const std::wstring &name);
}

#endif //DIRECTGRAPH_TESTLIB_FUNCS_H
