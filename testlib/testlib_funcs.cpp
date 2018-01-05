#include "testlib_funcs.h"
#include "ImageTester.h"
#include <gtest/gtest.h>

namespace directgraph_testlib {
    void test_image(ImageTest *test, const std::wstring &name) {
        getTester()->test(test->imageTestRun(), name);
        delete test;
    }

    int add_func(ImageTestWrap *test, const std::wstring &name) {
        TestInfo info = {test, name};
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
