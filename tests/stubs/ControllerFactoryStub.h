#pragma once

#include <gmock/gmock.h>
#include <IControllerFactory.h>

namespace directgraph {
    class ControllerFactoryStub: public IControllerFactory {
    public:
        MOCK_METHOD2(createMultThreadController, IController *(IMyWindow *win, const CommonProps &props));
        MOCK_METHOD1(deleteController, void (IController *ctrl));
    };
}


