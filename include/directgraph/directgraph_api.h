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

void DIRECTGRAPH_EXPORT resize(uint32_t width, uint32_t height);


void DIRECTGRAPH_EXPORT fillellipsec(int32_t x, int32_t y, uint32_t xradius, uint32_t yradius, uint32_t color);

void DIRECTGRAPH_EXPORT rectanglec(int32_t left, int32_t top, int32_t right, int32_t bottom, uint32_t color);

void DIRECTGRAPH_EXPORT linec(int32_t startx, int32_t starty, int32_t endx, int32_t endy, uint32_t color);

void DIRECTGRAPH_EXPORT linetoc(int32_t x, int32_t y, uint32_t color);

void DIRECTGRAPH_EXPORT linerelc(int32_t x, int32_t y, uint32_t color);

void DIRECTGRAPH_EXPORT drawpolyc(uint32_t numPoints, const int32_t *points, uint32_t color);

void DIRECTGRAPH_EXPORT fillpolyc(uint32_t numPoints, const int32_t *points, uint32_t color);

void DIRECTGRAPH_EXPORT ellipsec(int32_t x, int32_t y, uint32_t xradius, uint32_t yradius, uint32_t color);

void DIRECTGRAPH_EXPORT circlec(int32_t x, int32_t y, uint32_t radius, uint32_t color);

void DIRECTGRAPH_EXPORT bar3dc(int32_t left, int32_t top, int32_t right, int32_t bottom, uint32_t depth, uint32_t haveTop, uint32_t color);

void DIRECTGRAPH_EXPORT putpixelnc(int32_t x, int32_t y);


void DIRECTGRAPH_EXPORT fillellipse(int32_t x, int32_t y, uint32_t xradius, uint32_t yradius);

void DIRECTGRAPH_EXPORT setlinestyle(uint32_t linestyle, uint32_t pattern, uint32_t thickness);

void DIRECTGRAPH_EXPORT bar(int32_t left, int32_t top, int32_t right, int32_t bottom);

void DIRECTGRAPH_EXPORT rectangle(int32_t left, int32_t top, int32_t right, int32_t bottom);

void DIRECTGRAPH_EXPORT putpixel(int32_t x, int32_t y, uint32_t color);

void DIRECTGRAPH_EXPORT line(int32_t startx, int32_t starty, int32_t endx, int32_t endy);

void DIRECTGRAPH_EXPORT lineto(int32_t x, int32_t y);

void DIRECTGRAPH_EXPORT linerel(int32_t x, int32_t y);

void DIRECTGRAPH_EXPORT moveto(int32_t x, int32_t y);

void DIRECTGRAPH_EXPORT moverel(int32_t x, int32_t y);

void DIRECTGRAPH_EXPORT drawpoly(uint32_t numPoints, const int32_t *points);

void DIRECTGRAPH_EXPORT fillpoly(uint32_t numPoints, const int32_t *points);

void DIRECTGRAPH_EXPORT ellipse(int32_t x, int32_t y, uint32_t xradius, uint32_t yradius);

void DIRECTGRAPH_EXPORT circle(int32_t x, int32_t y, uint32_t radius);

void DIRECTGRAPH_EXPORT bar3d(int32_t left, int32_t top, int32_t right, int32_t bottom, uint32_t depth, uint32_t haveTop);

void DIRECTGRAPH_EXPORT setcolor(uint32_t color);

void DIRECTGRAPH_EXPORT setfillstyle(uint32_t pattern, uint32_t color);

void DIRECTGRAPH_EXPORT setfillpattern(const char *fillpattern, uint32_t color);

void DIRECTGRAPH_EXPORT setbgcolor(uint32_t color);

void DIRECTGRAPH_EXPORT directgraph_repaint();

uint32_t DIRECTGRAPH_EXPORT getcolor();

uint32_t DIRECTGRAPH_EXPORT getbgcolor();

void DIRECTGRAPH_EXPORT getlinesettings(linesettingstype *lineinfo);

void DIRECTGRAPH_EXPORT getfillsettings(fillsettingstype *fillinfo);

void DIRECTGRAPH_EXPORT getfillpattern(char *pattern);

void DIRECTGRAPH_EXPORT directgraph_initpalette(uint32_t havePalette, uint32_t size, uint32_t fillFirstColors);

void DIRECTGRAPH_EXPORT setpalette(uint32_t index, uint32_t color);

void DIRECTGRAPH_EXPORT clearpalette();

int32_t DIRECTGRAPH_EXPORT getx();

int32_t DIRECTGRAPH_EXPORT gety();

uint32_t DIRECTGRAPH_EXPORT getmaxx();

uint32_t DIRECTGRAPH_EXPORT getmaxy();

void DIRECTGRAPH_EXPORT clear();


void DIRECTGRAPH_EXPORT fillellipsew(DirectgraphWinIndex index, int32_t x, int32_t y, uint32_t xradius, uint32_t yradius);

void DIRECTGRAPH_EXPORT setlinestylew(DirectgraphWinIndex index, uint32_t linestyle, uint32_t pattern, uint32_t thickness);

void DIRECTGRAPH_EXPORT barw(DirectgraphWinIndex index, int32_t left, int32_t top, int32_t right, int32_t bottom);

void DIRECTGRAPH_EXPORT rectanglew(DirectgraphWinIndex index, int32_t left, int32_t top, int32_t right, int32_t bottom);

void DIRECTGRAPH_EXPORT putpixelw(DirectgraphWinIndex index, int32_t x, int32_t y, uint32_t color);

void DIRECTGRAPH_EXPORT linew(DirectgraphWinIndex index, int32_t startx, int32_t starty, int32_t endx, int32_t endy);

void DIRECTGRAPH_EXPORT linetow(DirectgraphWinIndex index, int32_t x, int32_t y);

void DIRECTGRAPH_EXPORT linerelw(DirectgraphWinIndex index, int32_t x, int32_t y);

void DIRECTGRAPH_EXPORT movetow(DirectgraphWinIndex index, int32_t x, int32_t y);

void DIRECTGRAPH_EXPORT moverelw(DirectgraphWinIndex index, int32_t x, int32_t y);

void DIRECTGRAPH_EXPORT drawpolyw(DirectgraphWinIndex index, uint32_t numPoints, const int32_t *points);

void DIRECTGRAPH_EXPORT fillpolyw(DirectgraphWinIndex index, uint32_t numPoints, const int32_t *points);

void DIRECTGRAPH_EXPORT setcolorw(DirectgraphWinIndex index, uint32_t color);

void DIRECTGRAPH_EXPORT setfillstylew(DirectgraphWinIndex index, uint32_t pattern, uint32_t color);

void DIRECTGRAPH_EXPORT setfillpatternw(DirectgraphWinIndex index, const char *fillpattern, uint32_t color);

void DIRECTGRAPH_EXPORT setbgcolorw(DirectgraphWinIndex index, uint32_t color);

void DIRECTGRAPH_EXPORT directgraph_repaintw(DirectgraphWinIndex index);

uint32_t DIRECTGRAPH_EXPORT getcolorw(DirectgraphWinIndex index);

uint32_t DIRECTGRAPH_EXPORT getbgcolorw(DirectgraphWinIndex index);

void DIRECTGRAPH_EXPORT getlinesettingsw(DirectgraphWinIndex index, linesettingstype *lineinfo);

void DIRECTGRAPH_EXPORT getfillsettingsw(DirectgraphWinIndex index, fillsettingstype *fillinfo);

void DIRECTGRAPH_EXPORT getfillpatternw(DirectgraphWinIndex index, char *pattern);

void DIRECTGRAPH_EXPORT directgraph_initpalettew(DirectgraphWinIndex index, uint32_t havePalette, uint32_t size, uint32_t fillFirstColors);

void DIRECTGRAPH_EXPORT setpalettew(DirectgraphWinIndex index, uint32_t colorIndex, uint32_t color);

void DIRECTGRAPH_EXPORT clearpalettew(DirectgraphWinIndex index);

uint32_t DIRECTGRAPH_EXPORT getmaxxw(DirectgraphWinIndex index);

uint32_t DIRECTGRAPH_EXPORT getmaxyw(DirectgraphWinIndex index);

void DIRECTGRAPH_EXPORT ellipsew(DirectgraphWinIndex index, int32_t x, int32_t y, uint32_t xradius, uint32_t yradius);

void DIRECTGRAPH_EXPORT circlew(DirectgraphWinIndex index, int32_t x, int32_t y, uint32_t radius);

void DIRECTGRAPH_EXPORT bar3dw(DirectgraphWinIndex index, int32_t left, int32_t top, int32_t right, int32_t bottom, uint32_t depth, uint32_t haveTop);

void DIRECTGRAPH_EXPORT clearw(DirectgraphWinIndex index);

int32_t DIRECTGRAPH_EXPORT getxw(DirectgraphWinIndex index);

int32_t DIRECTGRAPH_EXPORT getyw(DirectgraphWinIndex index);

uint32_t DIRECTGRAPH_EXPORT directgraph_mainloop();


void DIRECTGRAPH_EXPORT directgraph_get_dpi(float *dpix, float *dpiy);

DIRECTGRAPH_EXPORT DirectgraphWinParams* directgraph_create_winparams();

void DIRECTGRAPH_EXPORT directgraph_winparams_set_size(DirectgraphWinParams *params, uint32_t width, uint32_t height);

void DIRECTGRAPH_EXPORT directgraph_winparams_set_name(DirectgraphWinParams *params, const wchar_t *name);

void DIRECTGRAPH_EXPORT directgraph_destroy_winparams(DirectgraphWinParams *params);

DirectgraphWinIndex DIRECTGRAPH_EXPORT directgraph_create_window_params(const DirectgraphWinParams *params);


DirectgraphWinIndex DIRECTGRAPH_EXPORT directgraph_create_window(const wchar_t *name, uint32_t width, uint32_t height);

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
#if defined(MAXCOLOR16)
    directgraph_initpalette(1, 16, 1);
#elif defined(MAXCOLOR256)
    directgraph_initpalette(1, 256, 0);
#endif
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

#ifdef __cplusplus
static inline void fillellipse(int32_t x, int32_t y, uint32_t xradius, uint32_t yradius, uint32_t color){
    fillellipsec(x, y, xradius, yradius, color);
}

static inline void rectangle(int32_t left, int32_t top, int32_t right, int32_t bottom, uint32_t color){
    rectanglec(left, top, right, bottom, color);
}

static inline void line(int32_t startx, int32_t starty, int32_t endx, int32_t endy, uint32_t color){
    linec(startx, starty, endx, endy, color);
}

static inline void lineto(int32_t x, int32_t y, uint32_t color){
    linetoc(x, y, color);
}

static inline void linerel(int32_t x, int32_t y, uint32_t color){
    linerelc(x, y, color);
}

static inline void drawpoly(uint32_t numPoints, const int32_t *points, uint32_t color){
    drawpolyc(numPoints, points, color);
}

static inline void fillpoly(uint32_t numPoints, const int32_t *points, uint32_t color){
    fillpolyc(numPoints, points, color);
}

static inline void ellipse(int32_t x, int32_t y, uint32_t xradius, uint32_t yradius, uint32_t color){
    ellipsec(x, y, xradius, yradius, color);
}

static inline void circle(int32_t x, int32_t y, uint32_t radius, uint32_t color){
    circlec(x, y, radius, color);
;}

static inline void bar3d(int32_t left, int32_t top, int32_t right, int32_t bottom, uint32_t depth, uint32_t haveTop, uint32_t color){
    bar3dc(left, top, right, bottom, depth, haveTop, color);
}

static inline void putpixel(int32_t x, int32_t y){
    putpixelnc(x, y);
}

static inline void setlinestyle(uint32_t linestyle){
    setlinestyle(linestyle, 0, 1);
}

#endif

#endif //DIRECTGRAPH_API_H
