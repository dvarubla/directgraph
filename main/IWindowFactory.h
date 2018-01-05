#pragma once

#include <MyWindow.h>

namespace directgraph {
    class IWindowFactory {
    public:
        virtual ~IWindowFactory() {}

        virtual IMyWindow *createDPIWindow(const wchar_t *name, float width, float height) = 0;

        virtual IMyWindow *createPixelWindow(const wchar_t *name, float width, float height) = 0;

        virtual void deleteWindow(IMyWindow *win) = 0;
    };
}
