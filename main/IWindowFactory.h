#pragma once

#include <MyWindow.h>

namespace directgraph {
    class IWindowFactory {
    public:
        struct DPIInfo{
            float dpiX;
            float dpiY;
        };

        virtual DPIInfo getDPIInfo() = 0;

        virtual ~IWindowFactory() {}

        virtual IMyWindow *createWindow(const wchar_t *name, uint32_t width, uint32_t height, const CommonProps &props) = 0;

        virtual void deleteWindow(IMyWindow *win) = 0;
    };
}
