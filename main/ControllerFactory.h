#pragma once

#include "IControllerFactory.h"

namespace directgraph {
    class ControllerFactory: public IControllerFactory {
    public:
        IController *createMultThreadController(IMyWindow *window, const CommonProps &props);
        void deleteController(IController *ctrl);
    };
}


