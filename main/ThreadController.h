#pragma once

#include "MyWindow.h"
#include "Queue.h"
#include "CommonProps.h"
#include "IController.h"
#include "ParamsChecker.h"

namespace directgraph {
    class ThreadController: public IController{
    private:
        enum ThreadCtrlMsg{
            REPAINT = WM_USER+1,
            PREPARE,
            STOP,
            REPLY = 65535
        };
        friend DWORD WINAPI repaintFunc(LPVOID ctrl);
        IMyWindow *_window;
        Queue _queue;
        QueueReader _reader;
        ParamsChecker _paramsChecker;
        LONG volatile _threadStarted;
        LONG volatile _numDrawMsgs;

        CRITICAL_SECTION _addCS;
        CRITICAL_SECTION _propsCS;
        CRITICAL_SECTION _queueCS;

        DWORD _drawThreadId;
        HANDLE _drawThread;
        CommonProps _currentProps;

        PixelContainerCreator *_pixContFactory;

        void flushPixels();
        void repaintThread();
        void stopRepaintThread();
        void writeItemHelper(const QueueItem &item);
        void writeItemHelperNoLock(const QueueItem &item);
        void checkGrow();
        void sendPrepareMsg();
    public:
        ThreadController(IMyWindow *window, const CommonProps &props);
        ~ThreadController();
        void init();
        void clear();
        void bar(int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom);
        void fillellipse(int_fast32_t x, int_fast32_t y, uint_fast32_t xradius, uint_fast32_t yradius);
        void setfillstyle(uint_fast8_t fillStyle, uint_fast32_t color);
        void setlinestyle(uint_fast8_t linestyle, uint_fast16_t pattern, uint_fast32_t thickness);
        void setfillpattern(const char *fillpattern, uint_fast32_t color);
        void setcolor(uint_fast32_t color);
        void setbgcolor(uint_fast32_t color);
        uint_fast32_t getcolor();
        uint_fast32_t getbgcolor();
        void getlinesettings (linesettingstype *lineinfo);
        void getfillsettings(fillsettingstype *fillinfo);
        void getfillpattern(char *pattern);
        void line(int_fast32_t startx, int_fast32_t starty, int_fast32_t endx, int_fast32_t endy);
        void lineto(int_fast32_t x, int_fast32_t y);
        void linerel(int_fast32_t x, int_fast32_t y);
        void moveto(int_fast32_t x, int_fast32_t y);
        void moverel(int_fast32_t x, int_fast32_t y);
        int_fast32_t getx();
        int_fast32_t gety();
        void drawpoly(uint_fast32_t numPoints, const int32_t *points);
        void fillpoly(uint_fast32_t numPoints, const int32_t *points);


        void putpixel(int_fast32_t x, int_fast32_t y, uint_fast32_t color);
        void repaint();

        void rectangle(int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom);
    };
}
