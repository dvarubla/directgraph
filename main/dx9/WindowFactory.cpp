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

        IMyWindow *
        WindowFactory::createWindow(const wchar_t *name, uint32_t width, uint32_t height, const CommonProps &props) {
            Renderer *renderer = NULL;
            MyWindow *window = NULL;
            try {
                renderer = new Renderer(_common, width, height, props);
                window = new MyWindow(
                        name, width, height
                );
                window->setRenderer(renderer);
            } catch (const std::exception &) {
                delete renderer;
                delete window;
                throw;
            }
            return window;
        }
    }
}