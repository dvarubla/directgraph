#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include "ImageTester.h"
#include "BitmapWrap.h"
#include "ImageTest.h"

#define IMAGE_TEST_NAME(test_fixture, test_name)\
    test_fixture##_##test_name##_image_test

#define IMAGE_TEST_NAME_WRAP_CLASS(test_fixture, test_name)\
    test_fixture##_##test_name##_image_test_wrap

#define IMAGE_TEST_NAME_WRAP(test_fixture, test_name)\
    test_fixture##_##test_name##_image_test_wrap_obj

#define IMAGE_TEST_VARNAME(test_fixture, test_name)\
    test_fixture##_##test_name##_var

#define LSTR(x) L ## x

#define IMAGE_TEST_FNAME(test_fixture, test_name)\
    LSTR(#test_fixture) L"_" LSTR(#test_name)

#define IMG_TEST_F(test_fixture, test_name)\
    class IMAGE_TEST_NAME(test_fixture, test_name): public test_fixture{\
    public:\
        BitmapWrap* imageTestRun();\
        virtual ~IMAGE_TEST_NAME(test_fixture, test_name)(){}\
    };\
    class IMAGE_TEST_NAME_WRAP_CLASS(test_fixture, test_name): public ImageTestWrap{\
    public:\
        ImageTest* getObj(){\
            return new IMAGE_TEST_NAME(test_fixture, test_name);\
        }\
    } IMAGE_TEST_NAME_WRAP(test_fixture, test_name);\
    TEST(test_fixture, test_name){\
        directgraph_testlib::test_image(\
            IMAGE_TEST_NAME_WRAP(test_fixture, test_name).getObj(),\
            IMAGE_TEST_FNAME(test_fixture, test_name)\
        );\
    }\
    int IMAGE_TEST_VARNAME(test_fixture, test_name) = \
        directgraph_testlib::add_func(\
            &IMAGE_TEST_NAME_WRAP(test_fixture, test_name),\
            IMAGE_TEST_FNAME(test_fixture, test_name)\
        )\
    ;\
    BitmapWrap* IMAGE_TEST_NAME(test_fixture, test_name)::imageTestRun()

namespace directgraph_testlib {
    typedef struct TestInfo{
        ImageTestWrap *test;
        std::wstring str;
    } TestInfo;
    std::vector<TestInfo> &getTestInfos();
    ImageTester* getTester();
    int add_func(ImageTestWrap *test, const std::wstring &name);
    void test_image(ImageTest *test, const std::wstring &name);
}
