#pragma once

#include <main/dx9/WindowFactory.h>
#include <testlib/testlib_funcs.h>
#include <tests/stubs/QueueReaderStub.h>
#include <tests/stubs/FeaturesStub.h>

using namespace directgraph;
using namespace directgraph::dx9;
using namespace directgraph_testlib;
using testing::NiceMock;

class CommonSimple{
public:
    CommonSimple();
    ~CommonSimple();
    static WindowFactory *_dx9Wf;
    IFeatures *_curFeatures;
    IMyWindow* createWindow(float w, float h);
    BitmapWrap* afterTestSimple(IMyWindow *win, IQueueReader *reader);
    void addOnCall(NiceMock<QueueReaderStub> &readerStub);
    FeaturesStub* setupFeatures();
    void init_factory();
};
