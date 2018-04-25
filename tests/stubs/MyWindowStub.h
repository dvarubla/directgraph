#pragma once

#include <main/IMyWindow.h>
#include <gmock/gmock.h>

namespace directgraph {
    class MyWindowStub: public IMyWindow {
    private:
    public:
        MOCK_METHOD0(getRenderer, IRenderer *());
        MOCK_METHOD0(show, void());
        MOCK_METHOD0(hide, void());
        MOCK_METHOD2(addListener, void(IWindowListener *listener, void *param));
        MOCK_METHOD0(getWidth, uint_fast32_t ());
        MOCK_METHOD0(getHeight, uint_fast32_t ());
    };
}
