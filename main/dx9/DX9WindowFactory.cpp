#include "DX9WindowFactory.h"
#include <dx9/DX9Renderer.h>
#include <dx9/DX9Common.h>

namespace directgraph{
    DX9WindowFactory::DX9WindowFactory() {
        create_window_class();
        setDPIAware();
        _common = new DX9Common();
    }

    DX9WindowFactory::~DX9WindowFactory() {
    }

    MyWindow *DX9WindowFactory::createDPIWindow(const wchar_t *name, float width, float height) {
        float dpiX, dpiY;
        _common->getDpi(dpiX, dpiY);
        DPIHelper *helper = new DPIHelper(dpiX, dpiY);
        return commonCreateWindow(helper, name, width, height);
    }

    MyWindow *DX9WindowFactory::createPixelWindow(const wchar_t *name, float width, float height) {
        DPIHelper *helper = new DPIHelper(DPIHelper::DEFAULT_DPIX, DPIHelper::DEFAULT_DPIY);
        return commonCreateWindow(helper, name, width, height);
    }

    MyWindow *DX9WindowFactory::commonCreateWindow(DPIHelper *helper, const wchar_t *name, float width, float height) {
        DX9Renderer *renderer = new DX9Renderer(_common, helper, width, height);
        MyWindow *window = new MyWindow(name, helper->toPixelsX(width), helper->toPixelsY(height));
        window->setRenderer(renderer);
        return window;
    }
}