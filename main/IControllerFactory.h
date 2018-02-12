#pragma once

#include "IController.h"
#include "IMyWindow.h"

namespace directgraph{
    class IControllerFactory{
    public:
        virtual IController *createMultThreadController(IMyWindow *window, const CommonProps &props) = 0;
        virtual void deleteController(IController *ctrl) = 0;
        virtual ~IControllerFactory(){}
    };
}