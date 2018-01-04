#pragma once

#include <map>
#include <vector>
#include "graphics_const_internal.h"
#include "IMyWindow.h"
#include "IController.h"
#include "IWindowFactory.h"
#include "NullWindowListener.h"
#include "RWRPLock.h"

namespace directgraph {
    struct DirectgraphWinParams{
        float width;
        float height;
        const wchar_t *name;
        DirectgraphControllerType controller;
        DirectgraphRendererType renderer;
        float dpiX;
        float dpiY;
    };

    class WindowManager: public NullWindowListener {
    private:
        struct WindowData{
            IController *ctrl;
            void *winIndexMem;
            DWORD threadId;
        };
        std::map<DirectgraphWinIndex, WindowData> _windows;
        std::vector<IWindowFactory*> _rendFactories;
        DirectgraphWinIndex _curWindowIndex;
        RWRPLock _mapLock;
        RWRPLock _curWindowLock;
        LONG volatile _curMapIndex;
    public:
        struct ControllerAndIndex{
            IController* controller;
            DirectgraphWinIndex index;
        };
        const static DirectgraphWinIndex NO_CURRENT_WINDOW;
        WindowManager(std::vector<IWindowFactory*> &rendFactories);
        void setCurrentWindow(DirectgraphWinIndex winIndex);
        DirectgraphWinIndex createWindow(const DirectgraphWinParams &params);
        void destroyWindow(DirectgraphWinIndex winIndex);
        ControllerAndIndex getCurrentWindowAndLock();
        void releaseCurrentWindowLock();
        IController *getWindowByIndexAndLock(DirectgraphWinIndex index);
        void releaseWindowLock();
        void onClose(void *param);
        ~WindowManager();
    };
}

