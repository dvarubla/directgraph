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

struct GetDPIMsg{
    IWindowFactory::DPIInfo *dpiInfo;
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
        } else if(msg.message == DIRECTGRAPH_GET_DPI){
            GetDPIMsg *dpiMsg = reinterpret_cast<GetDPIMsg *>(msg.wParam);
            try {
                tryCreateWindowManager();
                *dpiMsg->dpiInfo = getWindowManager()->getDPIInfo();
                PostThreadMessage(dpiMsg->threadId, DIRECTGRAPH_REPLY, 0, false);
            } catch(const std::exception &) {
                std::exception_ptr *ptrMem = new std::exception_ptr;
                *ptrMem = std::current_exception();
                PostThreadMessage(
                        dpiMsg->threadId, DIRECTGRAPH_REPLY,
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

DirectgraphWinIndex DIRECTGRAPH_EXPORT directgraph_create_window(const wchar_t *name, uint32_t width, uint32_t height){
    DirectgraphWinParams *params = directgraph_create_winparams();
    params->name = name;
    params->width = width;
    params->height = height;
    DirectgraphWinIndex index = directgraph_create_window_params(params);
    directgraph_destroy_winparams(params);
    return index;
}

void DIRECTGRAPH_EXPORT directgraph_get_dpi(float *dpix, float *dpiy){
    IWindowFactory::DPIInfo info = {0, 0};
    if(InterlockedCompareExchange(&mainThreadStarted, 0, 0)){
        MSG msg;
        GetDPIMsg dpiMsg = {&info, GetCurrentThreadId()};
        PostThreadMessage(mainThreadId, DIRECTGRAPH_GET_DPI, reinterpret_cast<WPARAM>(&dpiMsg), 0);
        GetMessage(&msg, NULL, DIRECTGRAPH_REPLY, DIRECTGRAPH_REPLY);
        EXC_CALL_WRAP(
                rethrow_exc_wparam(msg);
        )
    } else {
        EXC_CALL_WRAP(
                tryCreateWindowManager();
                info = getWindowManager()->getDPIInfo();
        )
    }
    *dpix = info.dpiX;
    *dpiy = info.dpiY;
}

DIRECTGRAPH_EXPORT DirectgraphWinParams* directgraph_create_winparams(){
    DirectgraphWinParams *params = new DirectgraphWinParams;
    params->width = 200;
    params->height = 200;
    params->name = L"Directgraph";
    params->controller = DIRECTGRAPH_MULT_THREAD_CTRL;
    params->renderer = DIRECTGRAPH_DX9_RENDERER;
    return params;
}

void DIRECTGRAPH_EXPORT directgraph_winparams_set_size(DirectgraphWinParams *params, uint32_t width, uint32_t height){
    params->width = width;
    params->height = height;
}

void DIRECTGRAPH_EXPORT directgraph_winparams_set_name(DirectgraphWinParams *params, const wchar_t *name){
    params->name = name;
}

void DIRECTGRAPH_EXPORT directgraph_destroy_winparams(DirectgraphWinParams *params){
    delete params;
}

DirectgraphWinIndex DIRECTGRAPH_EXPORT directgraph_create_window_params(const DirectgraphWinParams *params){
    if(InterlockedCompareExchange(&mainThreadStarted, 0, 0)){
        MSG msg;
        CreateWinMsg createMsg = {
                *params,
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
        DirectgraphWinIndex index = WindowManager::NO_CURRENT_WINDOW;
        EXC_CALL_WRAP(
                tryCreateWindowManager();
                index = createWindow(*params);
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

void DIRECTGRAPH_EXPORT resize(uint32_t width, uint32_t height){
    directgraph_create_window(L"Directgraph", width, height);
}

void DIRECTGRAPH_EXPORT bar(int32_t left, int32_t top, int32_t right, int32_t bottom){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->bar(left, top, right, bottom);
    )
}

void DIRECTGRAPH_EXPORT putpixel(int32_t x, int32_t y, uint32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->putpixel(x, y, color);
    )
}

void DIRECTGRAPH_EXPORT fillellipse(int32_t x, int32_t y, uint32_t xradius, uint32_t yradius) {
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->fillellipse(x, y, xradius, yradius);
    )
}

void DIRECTGRAPH_EXPORT setlinestyle(line_styles linestyle, uint32_t pattern, uint32_t thickness) {
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->setlinestyle(linestyle, pattern, thickness);
    )
}

void DIRECTGRAPH_EXPORT setfillstyle(fill_patterns pattern, uint32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->setfillstyle(pattern, color);
    )
}

void DIRECTGRAPH_EXPORT setfillpattern(const char *fillpattern, uint32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->setfillpattern(fillpattern, color);
    )
}

void DIRECTGRAPH_EXPORT setbgcolor(uint32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->setbgcolor(color);
    )
}

void DIRECTGRAPH_EXPORT delay(uint32_t msec){
    Sleep(msec);
    directgraph_repaint();
}

void DIRECTGRAPH_EXPORT directgraph_repaint(){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->repaint();
    )
}

void DIRECTGRAPH_EXPORT fillellipsew(DirectgraphWinIndex index, int32_t x, int32_t y, uint32_t xradius, uint32_t yradius){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->fillellipse(x, y, xradius, yradius);
    )
}

void DIRECTGRAPH_EXPORT setlinestylew(DirectgraphWinIndex index, line_styles linestyle, uint32_t pattern, uint32_t thickness){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->setlinestyle(linestyle, pattern, thickness);
    )
}

void DIRECTGRAPH_EXPORT barw(DirectgraphWinIndex index, int32_t left, int32_t top, int32_t right, int32_t bottom){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->bar(left, top, right, bottom);
    )
}

void DIRECTGRAPH_EXPORT putpixelw(DirectgraphWinIndex index, int32_t x, int32_t y, uint32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->putpixel(x, y, color);
    )
}

void DIRECTGRAPH_EXPORT setfillstylew(DirectgraphWinIndex index, fill_patterns pattern, uint32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->setfillstyle(pattern, color);
    )
}

void DIRECTGRAPH_EXPORT setfillpatternw(DirectgraphWinIndex index, const char *fillpattern, uint32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->setfillpattern(fillpattern, color);
    )
}

void DIRECTGRAPH_EXPORT setbgcolorw(DirectgraphWinIndex index, uint32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->setbgcolor(color);
    )
}

void DIRECTGRAPH_EXPORT directgraph_repaintw(DirectgraphWinIndex index){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->repaint();
    )
}
