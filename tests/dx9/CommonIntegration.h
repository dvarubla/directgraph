#pragma once

#include <WindowManager.h>
#include <dx9/WindowFactory.h>
#include <tests/stubs/WindowFactoryStub.h>
#include <testlib/ImageTest.h>

using namespace directgraph;
using namespace directgraph_testlib;
using dx9::WindowFactory;

class CommonIntegration {
public:
    static WindowManager *wman;
    static WindowFactory *wf;
    WindowFactoryStub *wfstub;
    IMyWindow *_curWindow;
    void initWindowManager();
    IMyWindow * createWindow(const wchar_t *name, uint32_t width, uint32_t height, const CommonProps &props);
    BitmapWrap* afterTestSimple(DirectgraphWinIndex index);
    ~CommonIntegration();
};


