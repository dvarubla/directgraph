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

        public:
            WindowFactory();

            ~WindowFactory();

            DPIInfo getDPIInfo();

            IMyWindow *createWindow(const wchar_t *name, uint32_t width, uint32_t height, const CommonProps &props);

            void deleteWindow(IMyWindow *win);

            Common *getCommon();
        };
    }
}
