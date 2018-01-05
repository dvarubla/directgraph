#include "ControllerFactory.h"
#include "ThreadController.h"

namespace directgraph{

    IController *ControllerFactory::createMultThreadController(IMyWindow *window) {
        return new ThreadController(window);
    }

    void ControllerFactory::deleteController(IController *ctrl) {
        delete ctrl;
    }
}