#pragma once

#include <DPIHelper.h>
#include <MyWindow.h>
#include <dx9/DX9Common.h>
#include <IWindowFactory.h>

namespace directgraph {
    class DX9WindowFactory: public IWindowFactory {
    private:
        DX9Common *_common;
        MyWindow *commonCreateWindow(DPIHelper *helper, const wchar_t *name, float width, float height);
    public:
        DX9WindowFactory();

        ~DX9WindowFactory();

        IMyWindow *createDPIWindow(const wchar_t *name, float width, float height);
        IMyWindow *createPixelWindow(const wchar_t *name, float width, float height);

        void deleteWindow(IMyWindow *win);
    };
}
