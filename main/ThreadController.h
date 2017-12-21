#ifndef PROJECT_THREADCONTROLLER_H
#define PROJECT_THREADCONTROLLER_H

#include "MyWindow.h"
#include "Queue.h"
#include "CommonProps.h"

namespace directgraph {
    class ThreadController {
    private:
        enum ThreadCtrlMsg{
            REPAINT = WM_USER+1,
            STOP,
            REPLY = 65535
        };
        friend DWORD WINAPI repaintFunc(LPVOID ctrl);
        MyWindow *_window;
        Queue _queue;
        QueueReader _reader;
        LONG volatile _threadStarted;

        CRITICAL_SECTION _addCS;
        CRITICAL_SECTION _queueCS;

        DWORD _drawThreadId;
        CommonProps _currentProps;

        void repaintThread();
        void stopRepaintThread();
        void writeItemHelper(const QueueItem &item);
    public:
        ThreadController(MyWindow *window);
        ~ThreadController();
        void init();
        void clear();
        void bar(float left, float top, float right, float bottom);
        void setfillstyle(uint_fast8_t fillStyle, uint_fast32_t color);
        void putpixel(int_fast32_t x, int_fast32_t y, uint_fast32_t color);
        void repaint();
    };
}

#endif //PROJECT_THREADCONTROLLER_H
