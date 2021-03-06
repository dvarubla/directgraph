#pragma once

#include <map>
#include <vector>
#include <directgraph/directgraph_constants.h>
#include "IMyWindow.h"
#include "IController.h"
#include "IWindowFactory.h"
#include "NullWindowListener.h"
#include "RWRPLock.h"
#include "IControllerFactory.h"

struct DirectgraphWinParams{
    uint32_t width;
    uint32_t height;
    const wchar_t *name;
    DirectgraphControllerType controller;
    DirectgraphRendererType renderer;
};

namespace directgraph {
    class WindowManager: public NullWindowListener {
    private:
        struct WindowData{
            IController *ctrl;
            IMyWindow *win;
            DirectgraphRendererType renderer;
            DirectgraphWinIndex *winIndexMem;
            DWORD threadId;
        };
        typedef std::map<DirectgraphWinIndex, WindowData> WinManMap;
        WinManMap _windows;
        std::vector<IWindowFactory*> _rendFactories;
        IControllerFactory *_ctrlFactory;
        DirectgraphWinIndex _curWindowIndex;
        RWRPLock _mapLock;
        RWRPLock _curWindowLock;
        CRITICAL_SECTION _dpiCS;
        LONG volatile _curMapIndex;
    public:
        struct ControllerAndIndex{
            IController* controller;
            DirectgraphWinIndex index;
        };
        const static DirectgraphWinIndex NO_CURRENT_WINDOW;
        WindowManager(std::vector<IWindowFactory*> &rendFactories, IControllerFactory *ctrlFactory);
        void setCurrentWindow(DirectgraphWinIndex winIndex);
        DirectgraphWinIndex createWindow(const DirectgraphWinParams &params);
        void destroyWindow(DirectgraphWinIndex winIndex);
        ControllerAndIndex getCurrentWindowAndLock();
        ControllerAndIndex getCurrentWindowAndLock(bool throwExc);
        void releaseCurrentWindowLock();
        IController *getWindowByIndexAndLock(DirectgraphWinIndex index);
        void releaseWindowLock();
        IWindowFactory::DPIInfo getDPIInfo();
        void onClose(void *param);
        ~WindowManager();
    };

    class WindowManagerScopedLock{
    private:
        WindowManager *_winMan;
        enum LockType{
            CURRENT_WINDOW,
            WINDOW_BY_INDEX
        } _lockType;
    public:
        WindowManagerScopedLock(WindowManager *winMan, DirectgraphWinIndex index);
        WindowManagerScopedLock(WindowManager *winMan);
        ~WindowManagerScopedLock();
        WindowManager::ControllerAndIndex data;
    };
}

