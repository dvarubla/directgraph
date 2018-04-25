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

struct DefaultPaletteParams{
    bool havePalette;
    uint_fast32_t size;
    bool fillFirstColors;
};

struct SetDefaultPaletteParamsMsg{
    DefaultPaletteParams params;
    DWORD threadId;
};

static DefaultPaletteParams defaultPaletteParams = {false, 0, false};

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
                PostThreadMessage(static_cast<DWORD>(msg.wParam), DIRECTGRAPH_REPLY, 0, false);
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
        } else if(msg.message == DIRECTGRAPH_SET_PALETTE_PARAMS){
            SetDefaultPaletteParamsMsg *setParamsMsg = reinterpret_cast<SetDefaultPaletteParamsMsg*>(msg.wParam);
            defaultPaletteParams = setParamsMsg->params;
            PostThreadMessage(setParamsMsg->threadId, DIRECTGRAPH_REPLY, 0, false);
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return 0;
}

void DIRECTGRAPH_EXPORT directgraph_initpalette(uint32_t havePalette, uint_fast32_t size, uint32_t fillFirstColors){
    if(InterlockedCompareExchange(&needCreateWindowManager, 0, 0)){
        if(InterlockedCompareExchange(&mainThreadStarted, 0, 0)){
            SetDefaultPaletteParamsMsg paletteParamMsg;
            paletteParamMsg.threadId = GetCurrentThreadId();
            paletteParamMsg.params.havePalette = havePalette == 1;
            paletteParamMsg.params.size = size;
            paletteParamMsg.params.fillFirstColors = fillFirstColors == 1;
            PostThreadMessage(mainThreadId, DIRECTGRAPH_SET_PALETTE_PARAMS, reinterpret_cast<WPARAM>(&paletteParamMsg), 0);
            MSG msg;
            GetMessage(&msg, NULL, DIRECTGRAPH_REPLY, DIRECTGRAPH_REPLY);
        } else {
            defaultPaletteParams.havePalette = havePalette == 1;
            defaultPaletteParams.size = size;
            defaultPaletteParams.fillFirstColors = fillFirstColors == 1;
        }
    } else {
        EXC_CALL_WRAP(
                WindowManagerScopedLock lock(getWindowManager());
                lock.data.controller->initpalette(havePalette == 1, size, fillFirstColors == 1);
        )
    }
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
        directgraph_initpalettew(
                index,
                (defaultPaletteParams.havePalette) ? 1 : 0,
                defaultPaletteParams.size,
                (defaultPaletteParams.fillFirstColors) ? 1 : 0
        );
        directgraph_repaintw(index);
        return index;
    } else {
        DirectgraphWinIndex index = WindowManager::NO_CURRENT_WINDOW;
        EXC_CALL_WRAP(
                tryCreateWindowManager();
                index = createWindow(*params);
                directgraph_initpalettew(
                        index,
                        (defaultPaletteParams.havePalette) ? 1 : 0,
                        defaultPaletteParams.size,
                        (defaultPaletteParams.fillFirstColors) ? 1 : 0
                );
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

void DIRECTGRAPH_EXPORT rectangle(int32_t left, int32_t top, int32_t right, int32_t bottom){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->rectangle(left, top, right, bottom);
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

void DIRECTGRAPH_EXPORT setcolor(uint32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->setcolor(color);
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

uint32_t DIRECTGRAPH_EXPORT getcolor(){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            return lock.data.controller->getcolor();
    );
    return 0;
}

uint32_t DIRECTGRAPH_EXPORT getbgcolor(){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            return lock.data.controller->getbgcolor();
    );
    return 0;
}

void DIRECTGRAPH_EXPORT getlinesettings(linesettingstype *lineinfo){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->getlinesettings(lineinfo);
    );
}

void DIRECTGRAPH_EXPORT getfillsettings(fillsettingstype *fillinfo){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->getfillsettings(fillinfo);
    );
}

void DIRECTGRAPH_EXPORT getfillpattern(char *pattern){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->getfillpattern(pattern);
    );
}

void DIRECTGRAPH_EXPORT line(int32_t startx, int32_t starty, int32_t endx, int32_t endy){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->line(startx, starty, endx, endy);
    );
}

void DIRECTGRAPH_EXPORT lineto(int32_t x, int32_t y){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->lineto(x, y);
    );
}

void DIRECTGRAPH_EXPORT linerel(int32_t x, int32_t y){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->linerel(x, y);
    );
}

void DIRECTGRAPH_EXPORT moveto(int32_t x, int32_t y){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->moveto(x, y);
    );
}

void DIRECTGRAPH_EXPORT moverel(int32_t x, int32_t y){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->moverel(x, y);
    );
}

void DIRECTGRAPH_EXPORT drawpoly(uint32_t numPoints, int32_t *points){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->drawpoly(numPoints, points);
    );
}

void DIRECTGRAPH_EXPORT fillpoly(uint32_t numPoints, int32_t *points){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->fillpoly(numPoints, points);
    );
}

void DIRECTGRAPH_EXPORT setpalette(uint_fast32_t index, uint_fast32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->setpalette(index, color);
    );
}

void DIRECTGRAPH_EXPORT clearpalette(){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->clearpalette();
    );
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

void DIRECTGRAPH_EXPORT rectanglew(DirectgraphWinIndex index, int32_t left, int32_t top, int32_t right, int32_t bottom){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->rectangle(left, top, right, bottom);
    )
}

void DIRECTGRAPH_EXPORT putpixelw(DirectgraphWinIndex index, int32_t x, int32_t y, uint32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->putpixel(x, y, color);
    )
}

void DIRECTGRAPH_EXPORT setcolorw(DirectgraphWinIndex index, uint32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->setcolor(color);
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

uint32_t DIRECTGRAPH_EXPORT getcolorw(DirectgraphWinIndex index){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            return lock.data.controller->getcolor();
    )
    return 0;
}

uint32_t DIRECTGRAPH_EXPORT getbgcolorw(DirectgraphWinIndex index){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            return lock.data.controller->getbgcolor();
    )
    return 0;
}

void DIRECTGRAPH_EXPORT getlinesettingsw(DirectgraphWinIndex index, linesettingstype *lineinfo){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->getlinesettings(lineinfo);
    )
}

void DIRECTGRAPH_EXPORT getfillsettingsw(DirectgraphWinIndex index, fillsettingstype *fillinfo){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->getfillsettings(fillinfo);
    )
}

void DIRECTGRAPH_EXPORT getfillpatternw(DirectgraphWinIndex index, char *pattern){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->getfillpattern(pattern);
    )
}

void DIRECTGRAPH_EXPORT directgraph_initpalettew(DirectgraphWinIndex index, uint32_t havePalette, uint_fast32_t size, uint32_t fillFirstColors){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->initpalette(havePalette == 1, size, fillFirstColors == 1);
    )
}

void DIRECTGRAPH_EXPORT setpalettew(DirectgraphWinIndex index, uint_fast32_t colorIndex, uint_fast32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->setpalette(colorIndex, color);
    )
}

void DIRECTGRAPH_EXPORT clearpalettew(DirectgraphWinIndex index){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->clearpalette();
    )
}


void DIRECTGRAPH_EXPORT linew(DirectgraphWinIndex index, int32_t startx, int32_t starty, int32_t endx, int32_t endy){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->line(startx, starty, endx, endy);
    );
}

void DIRECTGRAPH_EXPORT linetow(DirectgraphWinIndex index, int32_t x, int32_t y){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->lineto(x, y);
    );
}

void DIRECTGRAPH_EXPORT linerelw(DirectgraphWinIndex index, int32_t x, int32_t y){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->linerel(x, y);
    );
}

void DIRECTGRAPH_EXPORT movetow(DirectgraphWinIndex index, int32_t x, int32_t y){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->moveto(x, y);
    );
}

void DIRECTGRAPH_EXPORT moverelw(DirectgraphWinIndex index, int32_t x, int32_t y){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->moverel(x, y);
    );
}

void DIRECTGRAPH_EXPORT drawpolyw(DirectgraphWinIndex index, uint32_t numPoints, int32_t *points){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->drawpoly(numPoints, points);
    );
}

void DIRECTGRAPH_EXPORT fillpolyw(DirectgraphWinIndex index, uint32_t numPoints, int32_t *points){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager(), index);
            lock.data.controller->fillpoly(numPoints, points);
    );
}


void DIRECTGRAPH_EXPORT fillellipsec(int32_t x, int32_t y, uint32_t xradius, uint32_t yradius, uint_fast32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->fillellipse(x, y, xradius, yradius, true, color);
    );
}

void DIRECTGRAPH_EXPORT rectanglec(int32_t left, int32_t top, int32_t right, int32_t bottom, uint_fast32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->rectangle(left, top, right, bottom, true, color);
    );
}

void DIRECTGRAPH_EXPORT linec(int32_t startx, int32_t starty, int32_t endx, int32_t endy, uint_fast32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->line(startx, starty, endx, endy, true, color);
    );
}

void DIRECTGRAPH_EXPORT linetoc(int32_t x, int32_t y, uint_fast32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->lineto(x, y, true, color);
    );
}

void DIRECTGRAPH_EXPORT linerelc(int32_t x, int32_t y, uint_fast32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->linerel(x, y, true, color);
    );
}

void DIRECTGRAPH_EXPORT drawpolyc(uint32_t numPoints, int32_t *points, uint_fast32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->drawpoly(numPoints, points, true, color);
    );
}

void DIRECTGRAPH_EXPORT fillpolyc(uint32_t numPoints, int32_t *points, uint_fast32_t color){
    EXC_CALL_WRAP(
            WindowManagerScopedLock lock(getWindowManager());
            lock.data.controller->fillpoly(numPoints, points, true, color);
    );
}
