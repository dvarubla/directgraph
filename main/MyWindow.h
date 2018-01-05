#pragma once

#include <IRenderer.h>
#include <windows.h>
#include <string>
#include <vector>
#include "IMyWindow.h"
#include "IWindowListener.h"

namespace directgraph {
    class MyWindow : public IMyWindow{
    public:
        struct ListenerData{
            IWindowListener* listener;
            void *param;
        };
    private:
        HWND _hwnd;
        IRenderer *_renderer;
        std::vector<ListenerData> _listeners;
    public:
        MyWindow(const wchar_t *name, uint_fast32_t width, uint_fast32_t height);

        ~MyWindow();

        void show();

        void hide();

        void setRenderer(IRenderer *renderer);

        void addListener(IWindowListener *listener, void *param);

        IRenderer* getRenderer();

        std::vector<ListenerData>& getListeners();

        HWND getHWND();
    };

    void create_window_class();
}
