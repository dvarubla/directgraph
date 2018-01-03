#pragma once

#include <IWindowFactory.h>

namespace directgraph {
    class WindowFactoryStub: public IWindowFactory {
    public:
        WindowFactoryStub();

        IMyWindow *createDPIWindow(const wchar_t *name, float width, float height) override;

        IMyWindow *createPixelWindow(const wchar_t *name, float width, float height) override;
    };
}


