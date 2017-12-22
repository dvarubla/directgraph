#ifndef PROJECT_IMYWINDOW_H
#define PROJECT_IMYWINDOW_H

#include "IRenderer.h"

namespace directgraph{
    class IMyWindow{
    public:
        virtual IRenderer* getRenderer() = 0;
        virtual ~IMyWindow(){}
    };
}

#endif //PROJECT_IMYWINDOW_H
