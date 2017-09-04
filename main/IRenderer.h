#ifndef DIRECTGRAPH_IRENDERER_H
#define DIRECTGRAPH_IRENDERER_H

#include <common.h>
#include <windows.h>
#include <DPIHelper.h>
#include <stdint.h>

namespace directgraph {
    IFACE IRenderer {
    public:
        virtual ~IRenderer(){};

        virtual void setWindow(HWND hwnd) = 0;

        virtual void setcolor(uint_fast32_t color) = 0;

        virtual void bar(float left, float right, float top, float bottom) = 0;

        virtual void clear() = 0;

        virtual void repaint() = 0;
    };
}
#endif //DIRECTGRAPH_IRENDERER_H
