#ifndef DIRECTGRAPH_D2DWINDOWFACTORY_H
#define DIRECTGRAPH_D2DWINDOWFACTORY_H

#include <DPIHelper.h>
#include <MyWindow.h>
#include <DPIConverter.h>
#include <IWindowFactory.h>
#include "D2DCommon.h"

namespace directgraph {
    class D2DWindowFactory: public IWindowFactory {
        private:
            D2DCommon *_common;
            MyWindow *commonCreateWindow(
                    DPIHelper *helper,
                    DPIConverter *converter,
                    const wchar_t *name, float width, float height
            );
        public:
            D2DWindowFactory();

            ~D2DWindowFactory();

            MyWindow *createDPIWindow(const wchar_t *name, float width, float height);
            MyWindow *createPixelWindow(const wchar_t *name, float width, float height);
    };
}


#endif //DIRECTGRAPH_D2DWINDOWFACTORY_H
