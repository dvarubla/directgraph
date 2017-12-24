#include <directgraph/directgraph_api.h>
#include <dx9/DX9WindowFactory.h>
#include "ThreadController.h"

WPARAM DIRECTGRAPH_EXPORT directgraph_mainloop() {
    MSG msg;
    uint_fast32_t num_windows = 0;
    while (GetMessageW(&msg, NULL, 0, 0)){
        if(msg.message == DIRECTGRAPH_WND_CREATE){
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

static directgraph::ThreadController *lastController = NULL;
static directgraph::IWindowFactory *factory;

void DIRECTGRAPH_EXPORT directgraph_create_window(const wchar_t *name, float width, float height){
    if(lastController == NULL){
        factory = new directgraph::DX9WindowFactory();
        directgraph::MyWindow *win = factory->createPixelWindow(name, width, height);
        win->show();
        lastController = new directgraph::ThreadController(win);
        lastController->init();
    }
}

void DIRECTGRAPH_EXPORT bar(float left, float top, float right, float bottom){
    if(lastController != NULL) {
        lastController->bar(left, top, right, bottom);
    }
}

void DIRECTGRAPH_EXPORT putpixel(uint32_t x, uint32_t y, uint32_t color){
    if(lastController != NULL) {
        lastController->putpixel(x, y, color);
    }
}

void DIRECTGRAPH_EXPORT setfillstyle(fill_patterns pattern, uint32_t color){
    if(lastController != NULL) {
        lastController->setfillstyle(pattern, color);
    }
}

void DIRECTGRAPH_EXPORT delay(uint32_t msec){
    Sleep(msec);
    if(lastController != NULL) {
        lastController->repaint();
    }
}

void DIRECTGRAPH_EXPORT directgraph_repaint(){
    if(lastController != NULL) {
        lastController->repaint();
    }
}
