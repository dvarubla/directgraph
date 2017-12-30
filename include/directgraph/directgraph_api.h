#ifndef DIRECTGRAPH_MAINLOOP_H
#define DIRECTGRAPH_MAINLOOP_H
#include <windows.h>
#include <stdint.h>
#include "export_header.h"
#include "directgraph_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

void DIRECTGRAPH_EXPORT bar(float left, float top, float right, float bottom);

void DIRECTGRAPH_EXPORT putpixel(uint32_t x, uint32_t y, uint32_t color);

void DIRECTGRAPH_EXPORT setfillstyle(fill_patterns pattern, uint32_t color);

void DIRECTGRAPH_EXPORT delay(uint32_t msec);

void DIRECTGRAPH_EXPORT resize(float width, float height);

WPARAM DIRECTGRAPH_EXPORT directgraph_mainloop();

void DIRECTGRAPH_EXPORT directgraph_repaint();

void DIRECTGRAPH_EXPORT directgraph_create_window(const wchar_t *name, float width, float height);

void DIRECTGRAPH_EXPORT directgraph_wait_for_main_thread();

#ifndef DIRECTGRAPH_NOMAIN
void mainx();
DWORD WINAPI directgraph_thread(LPVOID param){
    (void)(param);
    directgraph_wait_for_main_thread();
    mainx();
    directgraph_repaint();
    return 0;
}
int main(){
    CreateThread(NULL, 0, directgraph_thread, NULL, 0, NULL);
    directgraph_mainloop();
}
#endif

#ifdef __cplusplus
}
#endif

#endif //DIRECTGRAPH_MAINLOOP_H
