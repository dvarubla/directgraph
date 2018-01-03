#ifndef PROJECT_IMYWINDOW_H
#define PROJECT_IMYWINDOW_H

#include "IRenderer.h"
#include "IWindowListener.h"

namespace directgraph{
    class IMyWindow{
    public:
        virtual IRenderer* getRenderer() = 0;
        virtual void show() = 0;
        virtual void hide() = 0;
        virtual void addListener(IWindowListener *listener, void *param) = 0;
        virtual ~IMyWindow(){}
    };
}

#endif //PROJECT_IMYWINDOW_H
