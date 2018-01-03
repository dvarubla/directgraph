#include <directgraph/directgraph_api.h>
#include <dx9/DX9WindowFactory.h>
#include "ThreadController.h"
#include "WindowManager.h"

using namespace directgraph;

struct CreateWinMsg{
    DirectgraphWinParams params;
    DWORD threadId;
};

static LONG volatile mainThreadStarted = 0;
static DWORD mainThreadId = 0;

static PVOID windowManager = NULL;
static LONG volatile needCreateWindowManager = 1;

static void tryCreateWindowManager(){
    if(InterlockedExchange(&needCreateWindowManager, 0)) {
        IWindowFactory *factory = new DX9WindowFactory();
        std::vector<IWindowFactory *> factories;
        factories.push_back(factory);
        WindowManager *localManager = new WindowManager(factories);
        InterlockedExchangePointer(&windowManager, localManager);
    }
}

static DirectgraphWinIndex createWindow(const DirectgraphWinParams &params){
    return static_cast<WindowManager *>(
            InterlockedCompareExchangePointer(&windowManager, NULL, NULL)
    )->createWindow(params);
}

WPARAM DIRECTGRAPH_EXPORT directgraph_mainloop() {
    MSG msg;
    PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
    mainThreadId = GetCurrentThreadId();
    InterlockedExchange(&mainThreadStarted, 1);
    uint_fast32_t num_windows = 0;
    while (GetMessageW(&msg, NULL, 0, 0)){
        if(msg.message == DIRECTGRAPH_WND_CREATE){
            CreateWinMsg *createMsg = reinterpret_cast<CreateWinMsg *>(msg.wParam);
            tryCreateWindowManager();
            PostThreadMessage(createMsg->threadId, DIRECTGRAPH_REPLY, createWindow(createMsg->params), 0);
        } else if(msg.message == DIRECTGRAPH_WND_CREATED){
            num_windows++;
        } else if(msg.message == DIRECTGRAPH_WND_DESTROY){
            directgraph_destroy_window(static_cast<DirectgraphWinIndex>(msg.lParam));
            PostThreadMessage(static_cast<DWORD>(msg.wParam), DIRECTGRAPH_REPLY, 0, 0);
        } else if(msg.message == DIRECTGRAPH_WND_DESTROYED){
            num_windows--;
            if(num_windows == 0){
                break;
            }
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return 0;
}

DirectgraphWinIndex DIRECTGRAPH_EXPORT directgraph_create_window(const wchar_t *name, float width, float height){
    if(InterlockedCompareExchange(&mainThreadStarted, 0, 0)){
        MSG msg;
        CreateWinMsg createMsg = {
                {width, height, name, DIRECTGRAPH_MULT_THREAD_CTRL, DIRECTGRAPH_DX9_RENDERER},
                GetCurrentThreadId()
        };
        PostThreadMessage(mainThreadId, DIRECTGRAPH_WND_CREATE, reinterpret_cast<WPARAM>(&createMsg), 0);
        GetMessage(&msg, NULL, DIRECTGRAPH_REPLY, DIRECTGRAPH_REPLY);
        directgraph_repaint();
        return static_cast<DirectgraphWinIndex>(msg.wParam);
    } else {
        tryCreateWindowManager();
        DirectgraphWinParams params = {width, height, name};
        return createWindow(params);
    }
}

void DIRECTGRAPH_EXPORT directgraph_destroy_window(DirectgraphWinIndex index){
    WindowManager *localWindowManager = static_cast<WindowManager *>(
            InterlockedCompareExchangePointer(&windowManager, NULL, NULL)
    );
    if(localWindowManager != NULL) {
        localWindowManager->destroyWindow(index);
    }
}

void DIRECTGRAPH_EXPORT directgraph_set_window(DirectgraphWinIndex index){
    WindowManager *localWindowManager = static_cast<WindowManager *>(
            InterlockedCompareExchangePointer(&windowManager, NULL, NULL)
    );
    if(localWindowManager != NULL) {
        localWindowManager->setCurrentWindow(index);
    }
}

void DIRECTGRAPH_EXPORT directgraph_wait_for_main_thread(){
    while(!InterlockedCompareExchange(&mainThreadStarted, 0, 0)){
        Sleep(100);
    }
}

void DIRECTGRAPH_EXPORT resize(float width, float height){
    directgraph_create_window(L"Directgraph", width, height);
}

void DIRECTGRAPH_EXPORT bar(float left, float top, float right, float bottom){
    WindowManager *localWindowManager = static_cast<WindowManager *>(
            InterlockedCompareExchangePointer(&windowManager, NULL, NULL)
    );
    if(localWindowManager != NULL) {
        IController *lastController = localWindowManager->getCurrentWindowAndLock().controller;
        if(lastController != NULL) {
            lastController->bar(left, top, right, bottom);
        }
        localWindowManager->releaseCurrentWindowLock();
    }
}

void DIRECTGRAPH_EXPORT putpixel(uint32_t x, uint32_t y, uint32_t color){
    WindowManager *localWindowManager = static_cast<WindowManager *>(
            InterlockedCompareExchangePointer(&windowManager, NULL, NULL)
    );
    if(localWindowManager != NULL) {
        IController *lastController = localWindowManager->getCurrentWindowAndLock().controller;
        if(lastController != NULL) {
            lastController->putpixel(x, y, color);
        }
        localWindowManager->releaseCurrentWindowLock();
    }
}

void DIRECTGRAPH_EXPORT setfillstyle(fill_patterns pattern, uint32_t color){
    WindowManager *localWindowManager = static_cast<WindowManager *>(
            InterlockedCompareExchangePointer(&windowManager, NULL, NULL)
    );
    if(localWindowManager != NULL) {
        IController *lastController = localWindowManager->getCurrentWindowAndLock().controller;
        if(lastController != NULL) {
            lastController->setfillstyle(pattern, color);
        }
        localWindowManager->releaseCurrentWindowLock();
    }
}

void DIRECTGRAPH_EXPORT delay(uint32_t msec){
    Sleep(msec);
    directgraph_repaint();
}

void DIRECTGRAPH_EXPORT directgraph_repaint(){
    WindowManager *localWindowManager = static_cast<WindowManager *>(
            InterlockedCompareExchangePointer(&windowManager, NULL, NULL)
    );
    if(localWindowManager != NULL) {
        IController *lastController = localWindowManager->getCurrentWindowAndLock().controller;
        if(lastController != NULL) {
            lastController->repaint();
        }
        localWindowManager->releaseCurrentWindowLock();
    }
}
