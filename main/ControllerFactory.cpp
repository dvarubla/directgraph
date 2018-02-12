#include "ControllerFactory.h"
#include "ThreadController.h"

namespace directgraph{

    IController *ControllerFactory::createMultThreadController(IMyWindow *window, const CommonProps &props) {
        return new ThreadController(window, props);
    }

    void ControllerFactory::deleteController(IController *ctrl) {
        delete ctrl;
    }
}