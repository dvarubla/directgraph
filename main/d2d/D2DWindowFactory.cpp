#include "D2DWindowFactory.h"
#include <d2d/D2DRenderer.h>

namespace directgraph {

    D2DWindowFactory::D2DWindowFactory() {
        _common = new D2DCommon();
        create_window_class();
        setDPIAware();
    }

    D2DWindowFactory::~D2DWindowFactory() {
    }

    MyWindow *D2DWindowFactory::createDPIWindow(const wchar_t *name, float width, float height) {
        float dpiX, dpiY;
        _common->getDPI(dpiX, dpiY);
        DPIHelper *helper = new DPIHelper(dpiX, dpiY);
        DPIConverter *converter = new DPIConverter(dpiX, dpiY, dpiX, dpiY);
        return commonCreateWindow(helper, converter, name, width, height);
    }

    MyWindow *D2DWindowFactory::commonCreateWindow(
            DPIHelper *helper,
            DPIConverter *converter,
            const wchar_t *name,
            float width, float height
    ) {
        D2DRenderer *renderer = new D2DRenderer(_common, helper, converter, width, height);
        MyWindow *window = new MyWindow(name, helper->toPixelsX(width), helper->toPixelsY(height));
        window->setRenderer(renderer);
        return window;
    }

    MyWindow *D2DWindowFactory::createPixelWindow(const wchar_t *name, float width, float height) {
        float dpiX, dpiY;
        _common->getDPI(dpiX, dpiY);
        DPIHelper *helper = new DPIHelper(DPIHelper::DEFAULT_DPIX, DPIHelper::DEFAULT_DPIY);
        DPIConverter *converter = new DPIConverter(DPIHelper::DEFAULT_DPIX, DPIHelper::DEFAULT_DPIY, dpiX, dpiY);
        return commonCreateWindow(helper, converter, name, width, height);
    }
}