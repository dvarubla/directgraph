#include "WindowFactoryStub.h"
#include "MyWindowStub.h"
#include "RendererStub.h"

namespace directgraph{
    IMyWindow *WindowFactoryStub::createDPIWindow(const wchar_t *, float, float) {
        return new MyWindowStub(new RendererStub());
    }

    IMyWindow *WindowFactoryStub::createPixelWindow(const wchar_t *, float, float) {
        return new MyWindowStub(new RendererStub());
    }

    WindowFactoryStub::WindowFactoryStub() {

    }
}