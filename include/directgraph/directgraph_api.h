#ifndef DIRECTGRAPH_API_H
#define DIRECTGRAPH_API_H
#include <windows.h>
#include "directgraph_stdint.h"
#include "export_header.h"
#include "directgraph_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

void DIRECTGRAPH_EXPORT delay(uint32_t msec);

void DIRECTGRAPH_EXPORT resize(float width, float height);


void DIRECTGRAPH_EXPORT fillellipse(float x, float y, float xradius, float yradius);

void DIRECTGRAPH_EXPORT setlinestyle(line_styles linestyle, uint32_t pattern, uint32_t thickness);

void DIRECTGRAPH_EXPORT bar(float left, float top, float right, float bottom);

void DIRECTGRAPH_EXPORT putpixel(uint32_t x, uint32_t y, uint32_t color);

void DIRECTGRAPH_EXPORT setfillstyle(fill_patterns pattern, uint32_t color);

void DIRECTGRAPH_EXPORT setfillpattern(const char *fillpattern, uint32_t color);

void DIRECTGRAPH_EXPORT setbgcolor(uint32_t color);

void DIRECTGRAPH_EXPORT directgraph_repaint();


void DIRECTGRAPH_EXPORT fillellipsew(DirectgraphWinIndex index, float x, float y, float xradius, float yradius);

void DIRECTGRAPH_EXPORT setlinestylew(DirectgraphWinIndex index, line_styles linestyle, uint32_t pattern, uint32_t thickness);

void DIRECTGRAPH_EXPORT barw(DirectgraphWinIndex index, float left, float top, float right, float bottom);

void DIRECTGRAPH_EXPORT putpixelw(DirectgraphWinIndex index, uint32_t x, uint32_t y, uint32_t color);

void DIRECTGRAPH_EXPORT setfillstylew(DirectgraphWinIndex index, fill_patterns pattern, uint32_t color);

void DIRECTGRAPH_EXPORT setfillpatternw(DirectgraphWinIndex index, const char *fillpattern, uint32_t color);

void DIRECTGRAPH_EXPORT setbgcolorw(DirectgraphWinIndex index, uint32_t color);

void DIRECTGRAPH_EXPORT directgraph_repaintw(DirectgraphWinIndex index);


WPARAM DIRECTGRAPH_EXPORT directgraph_mainloop();


void DIRECTGRAPH_EXPORT directgraph_get_dpi(float *dpix, float *dpiy);

DIRECTGRAPH_EXPORT DirectgraphWinParams* directgraph_create_winparams();

void DIRECTGRAPH_EXPORT directgraph_winparams_set_size(DirectgraphWinParams *params, float width, float height);

void DIRECTGRAPH_EXPORT directgraph_winparams_set_dpi(DirectgraphWinParams *params, float dpix, float dpiy);

void DIRECTGRAPH_EXPORT directgraph_winparams_set_name(DirectgraphWinParams *params, const wchar_t *name);

void DIRECTGRAPH_EXPORT directgraph_destroy_winparams(DirectgraphWinParams *params);

DirectgraphWinIndex DIRECTGRAPH_EXPORT directgraph_create_window_params(const DirectgraphWinParams *params);


DirectgraphWinIndex DIRECTGRAPH_EXPORT directgraph_create_window(const wchar_t *name, float width, float height);

void DIRECTGRAPH_EXPORT directgraph_destroy_window(DirectgraphWinIndex index);

void DIRECTGRAPH_EXPORT directgraph_set_window(DirectgraphWinIndex index);

void DIRECTGRAPH_EXPORT directgraph_wait_for_main_thread();

#ifdef __cplusplus
}
#endif

#ifndef DIRECTGRAPH_NOMAIN
extern void mainx();
DWORD WINAPI directgraph_thread(LPVOID param){
    (void)(param);
    directgraph_wait_for_main_thread();
    mainx();
    directgraph_repaint();
    return 0;
}
int
#if defined(UNICODE) || defined(_UNICODE)
WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, INT nCmdShow)
#else
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, INT nCmdShow)
#endif
{
    (void)hInstance;
    (void)hPrevInstance;
    (void)pCmdLine;
    (void)nCmdShow;
    CreateThread(NULL, 0, directgraph_thread, NULL, 0, NULL);
    directgraph_mainloop();
    return 0;
}
#endif

#endif //DIRECTGRAPH_API_H
