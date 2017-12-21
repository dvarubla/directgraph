#include "ThreadController.h"

namespace directgraph{

    ThreadController::ThreadController(MyWindow *window)
            : _window(window), _queue(), _reader(&_queue, &_queueCS),
              _threadStarted(0) {
        _currentProps.color = 0xFFFFFF;
        InitializeCriticalSection(&_addCS);
        InitializeCriticalSection(&_queueCS);
    }

    void ThreadController::writeItemHelper(const QueueItem &item) {
        EnterCriticalSection(&_addCS);
        if(_queue.needGrow()) {
            EnterCriticalSection(&_queueCS);
            _queue.grow();
            LeaveCriticalSection(&_queueCS);
        }
        _queue.writeItem(item);
        LeaveCriticalSection(&_addCS);
    }

    void ThreadController::clear() {
        QueueItem item;
        item.type = QueueItem::CLEAR;
        writeItemHelper(item);
    }

    void ThreadController::bar(float left, float top, float right, float bottom) {
        QueueItem item;
        item.type = QueueItem::BAR;
        item.data.bar = {
                left, top, right, bottom
        };
        writeItemHelper(item);
    }

    void ThreadController::setfillstyle(uint_fast8_t fillStyle, uint_fast32_t color) {
        QueueItem item;
        item.type = QueueItem::SETFILLSTYLE;
        item.data.setfillstyle = {
                fillStyle,
                color
        };
        writeItemHelper(item);
    }

    void ThreadController::repaint() {
        while(!InterlockedCompareExchange(&_threadStarted, 0, 0)){
            Sleep(100);
        }
        MSG msg;
        PostThreadMessage(_drawThreadId, REPAINT, GetCurrentThreadId(), 0);
        GetMessage(&msg, NULL, REPLY, REPLY);
    }

    void ThreadController::repaintThread() {
        MSG msg;
        PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
        InterlockedExchange(&_threadStarted, 1);
        while(true) {
            GetMessage(&msg, NULL, 0, 0);
            if(msg.message == STOP){
                PostThreadMessage(msg.wParam, REPLY, 0, 0);
                break;
            }
            while(true) {
                EnterCriticalSection(&_queueCS);
                uint_fast32_t readSize = _queue.getReadSize();
                if(readSize == 0){
                    LeaveCriticalSection(&_queueCS);
                    break;
                }
                _reader.setGetIndex(_queue.getGetIndex(), readSize);
                _window->getRenderer()->draw(&_reader, &_currentProps);
            }
            _window->getRenderer()->repaint();
            PostThreadMessage(msg.wParam, REPLY, 0, 0);
        }
    }

    void ThreadController::stopRepaintThread() {
        MSG msg;
        PostThreadMessage(_drawThreadId, STOP, GetCurrentThreadId(), 0);
        GetMessage(&msg, NULL, REPLY, REPLY);
    }

    ThreadController::~ThreadController() {
        stopRepaintThread();
        DeleteCriticalSection(&_addCS);
        DeleteCriticalSection(&_queueCS);
        delete _window;
    }

    void ThreadController::init() {
        clear();
        CreateThread(NULL, 0, repaintFunc, (LPVOID)this, 0, &_drawThreadId);
    }

    DWORD WINAPI repaintFunc(LPVOID ctrl){
        ((ThreadController*)ctrl)->repaintThread();
        return 0;
    }
};