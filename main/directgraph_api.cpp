#include <directgraph/directgraph_api.h>
#include <dx9/DX9WindowFactory.h>
#include "ThreadController.h"

struct CreateWinMsg{
    const wchar_t *name;
    float width;
    float height;
    DWORD threadId;
};

static LONG volatile mainThreadStarted = 0;
static DWORD mainThreadId = 0;

static PVOID lastController = NULL;
static directgraph::IWindowFactory *factory;

static void createWindow(const wchar_t *name, float width, float height){
    factory = new directgraph::DX9WindowFactory();
    directgraph::MyWindow *win = factory->createPixelWindow(name, width, height);
    win->show();
    directgraph::ThreadController *localLastController = new directgraph::ThreadController(win);
    localLastController->init();
    InterlockedExchangePointer(&lastController, localLastController);
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
            createWindow(createMsg->name, createMsg->width, createMsg->height);
            PostThreadMessage(createMsg->threadId, DIRECTGRAPH_REPLY, 0, 0);
        } else if(msg.message == DIRECTGRAPH_WND_CREATED){
            num_windows++;
        } else if(msg.message == DIRECTGRAPH_WND_QUIT){
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

void DIRECTGRAPH_EXPORT directgraph_create_window(const wchar_t *name, float width, float height){
    if(InterlockedCompareExchangePointer(&lastController, NULL, NULL) == NULL){
        if(InterlockedCompareExchange(&mainThreadStarted, 0, 0)){
            MSG msg;
            CreateWinMsg createMsg = {name, width, height, GetCurrentThreadId()};
            PostThreadMessage(mainThreadId, DIRECTGRAPH_WND_CREATE, reinterpret_cast<WPARAM>(&createMsg), 0);
            GetMessage(&msg, NULL, DIRECTGRAPH_REPLY, DIRECTGRAPH_REPLY);
            static_cast<directgraph::ThreadController *>(
                    InterlockedCompareExchangePointer(&lastController, NULL, NULL)
            )->repaint();
        } else {
            createWindow(name, width, height);
        }
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
    directgraph::ThreadController *localLastController = static_cast<directgraph::ThreadController *>(
            InterlockedCompareExchangePointer(&lastController, NULL, NULL)
    );
    if(localLastController != NULL) {
        localLastController->bar(left, top, right, bottom);
    }
}

void DIRECTGRAPH_EXPORT putpixel(uint32_t x, uint32_t y, uint32_t color){
    directgraph::ThreadController *localLastController = static_cast<directgraph::ThreadController *>(
            InterlockedCompareExchangePointer(&lastController, NULL, NULL)
    );
    if(localLastController != NULL) {
        localLastController->putpixel(x, y, color);
    }
}

void DIRECTGRAPH_EXPORT setfillstyle(fill_patterns pattern, uint32_t color){
    directgraph::ThreadController *localLastController = static_cast<directgraph::ThreadController *>(
            InterlockedCompareExchangePointer(&lastController, NULL, NULL)
    );
    if(localLastController != NULL) {
        localLastController->setfillstyle(pattern, color);
    }
}

void DIRECTGRAPH_EXPORT delay(uint32_t msec){
    Sleep(msec);
    directgraph::ThreadController *localLastController = static_cast<directgraph::ThreadController *>(
            InterlockedCompareExchangePointer(&lastController, NULL, NULL)
    );
    if(localLastController != NULL) {
        localLastController->repaint();
    }
}

void DIRECTGRAPH_EXPORT directgraph_repaint(){
    directgraph::ThreadController *localLastController = static_cast<directgraph::ThreadController *>(
            InterlockedCompareExchangePointer(&lastController, NULL, NULL)
    );
    if(localLastController != NULL) {
        localLastController->repaint();
    }
}
