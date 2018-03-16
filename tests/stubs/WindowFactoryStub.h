#pragma once

#include <IWindowFactory.h>
#include <gmock/gmock.h>

namespace directgraph {
    class WindowFactoryStub: public IWindowFactory {
    public:
        MOCK_METHOD0(getDPIInfo, DPIInfo ());

        MOCK_METHOD4(createWindow, IMyWindow *(const wchar_t *name, uint32_t width, uint32_t height, const CommonProps &props));

        MOCK_METHOD1(deleteWindow, void (IMyWindow *win));
    };
}


