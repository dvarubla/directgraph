#ifndef DIRECTGRAPH_MAINLOOP_H
#define DIRECTGRAPH_MAINLOOP_H
#include <windows.h>
#include <stdint.h>
#include "export_header.h"
#include "graphics_const.h"

void DIRECTGRAPH_EXPORT bar(float left, float top, float right, float bottom);

void DIRECTGRAPH_EXPORT putpixel(uint32_t x, uint32_t y, uint32_t color);

void DIRECTGRAPH_EXPORT setfillstyle(fill_patterns pattern, uint32_t color);

void DIRECTGRAPH_EXPORT delay(uint32_t msec);

enum DirectGraphEvent{
    DIRECTGRAPH_WND_CREATE = 60000,
    DIRECTGRAPH_WND_QUIT
};

WPARAM DIRECTGRAPH_EXPORT directgraph_mainloop();

void DIRECTGRAPH_EXPORT directgraph_repaint();

void DIRECTGRAPH_EXPORT directgraph_create_window(const wchar_t *name, float width, float height);

#endif //DIRECTGRAPH_MAINLOOP_H
