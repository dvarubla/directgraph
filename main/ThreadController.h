#pragma once

#include "MyWindow.h"
#include "Queue.h"
#include "CommonProps.h"
#include "IController.h"

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
        LONG volatile _threadStarted;
        LONG volatile _numDrawMsgs;

        CRITICAL_SECTION _addCS;
        CRITICAL_SECTION _queueCS;

        DWORD _drawThreadId;
        CommonProps _currentProps;

        PixelContainerCreator *_pixContFactory;

        void flushPixels();
        void repaintThread();
        void stopRepaintThread();
        void writeItemHelper(const QueueItem &item);
        void checkGrow();
        void sendPrepareMsg();
    public:
        ThreadController(IMyWindow *window, const CommonProps &props);
        ~ThreadController();
        void init();
        void clear();
        void bar(float left, float top, float right, float bottom);
        void fillellipse(float x, float y, float xradius, float yradius);
        void setfillstyle(uint_fast8_t fillStyle, uint_fast32_t color);
        void setlinestyle(uint_fast8_t linestyle, uint_fast32_t pattern, uint_fast32_t thickness);
        void setfillpattern(const char *fillpattern, uint_fast32_t color);
        void setbgcolor(uint_fast32_t color);
        void putpixel(int_fast32_t x, int_fast32_t y, uint_fast32_t color);
        void repaint();
    };
}
