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

        virtual IMyWindow *createDPIWindow(
                const wchar_t *name, float width, float height, float dpiX, float dpiY, const CommonProps &props
        ) = 0;

        virtual IMyWindow *createPixelWindow(const wchar_t *name, float width, float height, const CommonProps &props) = 0;

        virtual void deleteWindow(IMyWindow *win) = 0;
    };
}
