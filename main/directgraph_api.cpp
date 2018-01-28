#include <dx9/WindowFactory.h>
#include <sstream>
#include "ThreadController.h"
#include "WindowManager.h"
#include "ControllerFactory.h"
#include "WinManException.h"
#include <directgraph/directgraph_api.h>
#include "error_handler.h"

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
        IWindowFactory *factory = new dx9::WindowFactory();
        std::vector<IWindowFactory *> factories;
        IControllerFactory *ctrlFactory = new ControllerFactory();
        factories.push_back(factory);
        WindowManager *localManager = new WindowManager(factories, ctrlFactory);
        InterlockedExchangePointer(&windowManager, localManager);
    }
}

static WindowManager * getWindowManager(){
    WindowManager *ret = static_cast<WindowManager *>(
            InterlockedCompareExchangePointer(&windowManager, NULL, NULL)
    );
    if(ret == NULL){
        THROW_EXC_CODE(WinManException, NO_WINDOWS, std::wstring(L"No window manager"));
    }
    return ret;
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
            try {
                tryCreateWindowManager();
                PostThreadMessage(createMsg->threadId, DIRECTGRAPH_REPLY, createWindow(createMsg->params), false);
            } catch(const std::exception &) {
                std::exception_ptr *ptrMem = new std::exception_ptr;
                *ptrMem = std::current_exception();
                PostThreadMessage(
                        createMsg->threadId, DIRECTGRAPH_REPLY,
                        reinterpret_cast<WPARAM>(ptrMem), true
                );
            }
        } else if(msg.message == DIRECTGRAPH_WND_CREATED){
            num_windows++;
        } else if(msg.message == DIRECTGRAPH_WND_DESTROY){
            try {
                directgraph_destroy_window(static_cast<DirectgraphWinIndex>(msg.lParam));
                PostThreadMessage(static_cast<DWORD>(msg.wParam), DIRECTGRAPH_REPLY, NULL, false);
            } catch(const std::exception &){
                std::exception_ptr *ptrMem = new std::exception_ptr;
                *ptrMem = std::current_exception();
                PostThreadMessage(
                        static_cast<DWORD>(msg.wParam), DIRECTGRAPH_REPLY,
                        reinterpret_cast<WPARAM>(ptrMem), true
                );
            }
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
        EXC_CALL_WRAP_EXTRA(
                rethrow_exc_wparam(msg);
        ) {
            return WindowManager::NO_CURRENT_WINDOW;
        }
        DirectgraphWinIndex index = static_cast<DirectgraphWinIndex>(msg.wParam);
        directgraph_repaintw(index);
        return index;
    } else {
        DirectgraphWinParams params = {width, height, name, DIRECTGRAPH_MULT_THREAD_CTRL, DIRECTGRAPH_DX9_RENDERER};
        DirectgraphWinIndex index = WindowManager::NO_CURRENT_WINDOW;
        EXC_CALL_WRAP(
                tryCreateWindowManager();
                index = createWindow(params);
        )
        return index;
    }
}

void DIRECTGRAPH_EXPORT directgraph_destroy_window(DirectgraphWinIndex index){
    EXC_CALL_WRAP(
            getWindowManager()->destroyWindow(index);
    )
}

void DIRECTGRAPH_EXPORT directgraph_set_window(DirectgraphWinIndex index){
    EXC_CALL_WRAP(
    getWindowManager()->setCurrentWindow(index);
    )
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
    EXC_CALL_WRAP(
    getWindowManager()->getCurrentWindowAndLock().controller->bar(left, top, right, bottom);
    getWindowManager()->releaseCurrentWindowLock();
    )
}

void DIRECTGRAPH_EXPORT putpixel(uint32_t x, uint32_t y, uint32_t color){
    EXC_CALL_WRAP(
    getWindowManager()->getCurrentWindowAndLock().controller->putpixel(x, y, color);
    getWindowManager()->releaseCurrentWindowLock();
    )
}

void DIRECTGRAPH_EXPORT setfillstyle(fill_patterns pattern, uint32_t color){
    EXC_CALL_WRAP(
    getWindowManager()->getCurrentWindowAndLock().controller->setfillstyle(pattern, color);
    getWindowManager()->releaseCurrentWindowLock();
    )
}

void DIRECTGRAPH_EXPORT delay(uint32_t msec){
    Sleep(msec);
    directgraph_repaint();
}

void DIRECTGRAPH_EXPORT directgraph_repaint(){
    EXC_CALL_WRAP(
    getWindowManager()->getCurrentWindowAndLock().controller->repaint();
    getWindowManager()->releaseCurrentWindowLock();
    )
}

void DIRECTGRAPH_EXPORT directgraph_repaintw(DirectgraphWinIndex index){
    EXC_CALL_WRAP(
    getWindowManager()->getWindowByIndexAndLock(index)->repaint();
    getWindowManager()->releaseWindowLock();
    )
}
