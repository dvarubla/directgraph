#pragma once

#include "IControllerFactory.h"

namespace directgraph {
    class ControllerFactory: public IControllerFactory {
    public:
        IController *createMultThreadController(IMyWindow *window);
        void deleteController(IController *ctrl);
    };
}


