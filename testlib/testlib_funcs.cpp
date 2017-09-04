#include "testlib_funcs.h"
#include "ImageTester.h"
#include <gtest/gtest.h>

namespace directgraph_testlib {
    void test_image(BitmapWrap *bitmapWrap, const std::wstring &name) {
        getTester()->test(bitmapWrap, name);
    }

    int add_func(BitmapWrap* (*func)(), const std::wstring &name) {
        TestInfo info = {func, name};
        getTestInfos().push_back(info);
        return 0;
    }

    std::vector<TestInfo>& getTestInfos(){
        static std::vector<TestInfo> *v = new std::vector<TestInfo>();
        return *v;
    }

    ImageTester *getTester() {
        static ImageTester *tester = new ImageTester();
        return tester;
    }
}
