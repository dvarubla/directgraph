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
    IMyWindow * createPixelWindow(const wchar_t *name, float width, float height, const CommonProps &props);
    IMyWindow * createDPIWindow(const wchar_t *name, float width, float height, float dpiX, float dpiY, const CommonProps &props);
    BitmapWrap* afterTestSimple(DirectgraphWinIndex index);
    ~CommonIntegration();
};


