#pragma once

#include <IWindowFactory.h>
#include <gmock/gmock.h>

namespace directgraph {
    class WindowFactoryStub: public IWindowFactory {
    public:

        MOCK_METHOD4(createDPIWindow, IMyWindow *(const wchar_t *name, float width, float height, const CommonProps &props));

        MOCK_METHOD4(createPixelWindow, IMyWindow *(const wchar_t *name, float width, float height, const CommonProps &props));

        MOCK_METHOD1(deleteWindow, void (IMyWindow *win));
    };
}


