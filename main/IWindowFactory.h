#ifndef DIRECTGRAPH_IWINDOWFACTORY_H
#define DIRECTGRAPH_IWINDOWFACTORY_H

#include <MyWindow.h>

namespace directgraph {
    class IWindowFactory {
    public:
        virtual ~IWindowFactory() {}

        virtual MyWindow *createDPIWindow(const wchar_t *name, float width, float height) = 0;

        virtual MyWindow *createPixelWindow(const wchar_t *name, float width, float height) = 0;
    };
}

#endif //DIRECTGRAPH_IWINDOWFACTORY_H
