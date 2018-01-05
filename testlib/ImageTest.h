#pragma once

#include "BitmapWrap.h"

namespace directgraph_testlib {
    class ImageTest {
    public:
        virtual BitmapWrap* imageTestRun() = 0;
        virtual ~ImageTest(){}
    };

    class ImageTestWrap{
    public:
        virtual ImageTest* getObj() = 0;
    };
}


