#include "WindowFactory.h"
#include <dx9/Renderer.h>
#include <dx9/Common.h>
#include <main/WException.h>

namespace directgraph{
    namespace dx9 {
        WindowFactory::WindowFactory() {
            create_window_class();
            setDPIAware();
            _common = new Common();
        }

        WindowFactory::~WindowFactory() {
            delete _common;
        }

        IMyWindow *WindowFactory::createDPIWindow(
                const wchar_t *name,
                float width, float height,
                float dpiX, float dpiY,
                const CommonProps &props
        ) {
            DPIHelper *helper = new DPIHelper(dpiX, dpiY);
            return commonCreateWindow(helper, name, width, height, props);
        }

        IMyWindow *WindowFactory::createPixelWindow(
                const wchar_t *name, float width, float height,
                const CommonProps &props
        ) {
            DPIHelper *helper = new DPIHelper(DPIHelper::DEFAULT_DPIX, DPIHelper::DEFAULT_DPIY);
            return commonCreateWindow(helper, name, width, height, props);
        }

        MyWindow *WindowFactory::commonCreateWindow(
                DPIHelper *helper, const wchar_t *name, float width, float height,
                const CommonProps &props
        ) {
            if (width < 0 || height < 0) {
                THROW_EXC_CODE(
                        WException, CANT_CREATE_WINDOW,
                        std::wstring(L"Window width and height must be positive")
                );
            }
            Renderer *renderer = NULL;
            MyWindow *window = NULL;
            try {
                renderer = new Renderer(_common, helper, width, height, props);
                window = new MyWindow(
                        name,
                        static_cast<uint_fast32_t>(helper->toPixelsX(width)),
                        static_cast<uint_fast32_t>(helper->toPixelsY(height))
                );
                window->setRenderer(renderer);
            } catch (const std::exception &) {
                delete renderer;
                delete window;
                throw;
            }
            return window;
        }

        void WindowFactory::deleteWindow(IMyWindow *win) {
            delete win->getRenderer();
            delete win;
        }

        Common *WindowFactory::getCommon() {
            return _common;
        }

        IWindowFactory::DPIInfo WindowFactory::getDPIInfo() {
            DPIInfo info;
            _common->getDpi(info.dpiX, info.dpiY);
            return info;
        }
    }
}