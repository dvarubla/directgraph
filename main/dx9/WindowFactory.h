#pragma once

#include <DPIHelper.h>
#include <MyWindow.h>
#include <dx9/Common.h>
#include <IWindowFactory.h>

namespace directgraph {
    namespace dx9 {
        class WindowFactory : public IWindowFactory {
        private:
            Common *_common;

            MyWindow *commonCreateWindow(
                    DPIHelper *helper,
                    const wchar_t *name,
                    float width, float height,
                    const CommonProps &props
            );

        public:
            WindowFactory();

            ~WindowFactory();

            IMyWindow *createDPIWindow(
                    const wchar_t *name,
                    float width, float height, float dpiX, float dpiY, const CommonProps &props
            );

            DPIInfo getDPIInfo();

            IMyWindow *createPixelWindow(const wchar_t *name, float width, float height, const CommonProps &props);

            void deleteWindow(IMyWindow *win);

            Common *getCommon();
        };
    }
}
