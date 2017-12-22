#ifndef DIRECTGRAPH_MYWINDOW_H
#define DIRECTGRAPH_MYWINDOW_H

#include <IRenderer.h>
#include <windows.h>
#include <string>
#include "IMyWindow.h"

namespace directgraph {
    class MyWindow : public IMyWindow{
    private:
        HWND _hwnd;
        IRenderer *_renderer;
    public:
        MyWindow(const wchar_t *name, uint_fast32_t width, uint_fast32_t height);

        ~MyWindow();

        void show();

        void hide();

        void setRenderer(IRenderer *renderer);

        IRenderer* getRenderer();

        HWND getHWND();
    };

    void create_window_class();
}
#endif //DIRECTGRAPH_MYWINDOW_H
