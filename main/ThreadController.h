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
            STOP,
            REPLY = 65535
        };
        friend DWORD WINAPI repaintFunc(LPVOID ctrl);
        IMyWindow *_window;
        Queue _queue;
        QueueReader _reader;
        LONG volatile _threadStarted;

        CRITICAL_SECTION _addCS;
        CRITICAL_SECTION _queueCS;
        CRITICAL_SECTION _lastElemCS;

        DWORD _drawThreadId;
        CommonProps _currentProps;

        PixelContainerFactory *_pixContFactory;

        void repaintThread();
        void stopRepaintThread();
        void writeItemHelper(const QueueItem &item);
    public:
        ThreadController(IMyWindow *window);
        ~ThreadController();
        void init();
        void clear();
        void bar(float left, float top, float right, float bottom);
        void setfillstyle(uint_fast8_t fillStyle, uint_fast32_t color);
        void putpixel(int_fast32_t x, int_fast32_t y, uint_fast32_t color);
        void repaint();
    };
}
