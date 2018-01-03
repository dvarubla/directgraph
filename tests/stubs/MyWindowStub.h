#ifndef PROJECT_MYWINDOWSTUB_H
#define PROJECT_MYWINDOWSTUB_H

#include <main/IMyWindow.h>

namespace directgraph {
    class MyWindowStub: public IMyWindow {
    private:
        IRenderer *_renderer;
    public:
        MyWindowStub(IRenderer *renderer);
        ~MyWindowStub();
        IRenderer *getRenderer();
        void show();
        void hide();
        void addListener(IWindowListener *listener, void *param);
    };
}

#endif //PROJECT_MYWINDOWSTUB_H
