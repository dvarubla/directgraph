#include "ThreadController.h"
#include "Queue.h"
#include "QueueItem.h"
#include "WException.h"

namespace directgraph{

    ThreadController::ThreadController(IMyWindow *window, const CommonProps &props)
            : _window(window), _queue(), _reader(&_queue, &_queueCS, &_lastElemCS),
              _threadStarted(0), _pixContFactory(_window->getRenderer()->getPixContFactory()) {
        _currentProps = props;
        InitializeCriticalSection(&_addCS);
        InitializeCriticalSection(&_queueCS);
        InitializeCriticalSection(&_lastElemCS);
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
        QueueItem item = QueueItemCreator::create<QueueItem::CLEAR>();
        writeItemHelper(item);
    }

    void ThreadController::bar(float left, float top, float right, float bottom) {
        QueueItem item = QueueItemCreator::create<QueueItem::BAR>(left, top, right, bottom);
        writeItemHelper(item);
    }

    void ThreadController::setfillstyle(uint_fast8_t fillStyle, uint_fast32_t color) {
        QueueItem item = QueueItemCreator::create<QueueItem::SETFILLSTYLE>(fillStyle, color);
        writeItemHelper(item);
    }

    void ThreadController::setbgcolor(uint_fast32_t color) {
        QueueItem item = QueueItemCreator::create<QueueItem::BGCOLOR>(color);
        writeItemHelper(item);
    }

    void ThreadController::setfillpattern(const char *fillpattern, uint_fast32_t color) {
        QueueItem item = QueueItemCreator::create<QueueItem::SETFILLPATTERN>(fillpattern, color);
        writeItemHelper(item);
    }

    void ThreadController::putpixel(int_fast32_t x, int_fast32_t y, uint_fast32_t color) {
        if(
                x < 0 || y < 0 ||
                static_cast<uint_fast32_t>(x) >= _pixContFactory->getMaxWidth() ||
                static_cast<uint_fast32_t>(y) >= _pixContFactory->getMaxHeight()
        ){
            return;
        }

        bool addSinglePixel = true;
        EnterCriticalSection(&_lastElemCS);
        try {
            if (_queue.getReadSize() != 0) {
                QueueItem &prevItem = _queue.getItemAt(_queue.transformIndex((int_fast32_t) _queue.getPutIndex() - 1));
                if (prevItem.type == QueueItem::SINGLE_PIXEL) {
                    PixelContainerFactory::ContainerOpt contOpt = _pixContFactory->tryGetContainer(
                            prevItem.data.singlePixel.x,
                            prevItem.data.singlePixel.y,
                            prevItem.data.singlePixel.color,
                            static_cast<uint_fast32_t>(x), static_cast<uint_fast32_t>(y), color
                    );
                    if (contOpt.containerCreated) {
                        addSinglePixel = false;
                        prevItem.type = QueueItem::PIXEL_CONTAINER;
                        prevItem.data.pixelContainer = contOpt.container;
                    }
                } else if (prevItem.type == QueueItem::PIXEL_CONTAINER) {
                    addSinglePixel = !prevItem.data.pixelContainer->tryAddPixel(
                            static_cast<uint_fast32_t>(x), static_cast<uint_fast32_t>(y), color
                    );
                }
            }
        } catch (const std::exception &){
            LeaveCriticalSection(&_lastElemCS);
            throw;
        }
        LeaveCriticalSection(&_lastElemCS);

        if(addSinglePixel) {
            QueueItem item = QueueItemCreator::create<QueueItem::SINGLE_PIXEL>(
                    static_cast<uint32_t>(x), static_cast<uint32_t>(y), color
            );
            writeItemHelper(item);
        }
    }

    void ThreadController::repaint() {
        while(!InterlockedCompareExchange(&_threadStarted, 0, 0)){
            Sleep(100);
        }
        MSG msg;
        PostThreadMessage(_drawThreadId, REPAINT, GetCurrentThreadId(), 0);
        GetMessage(&msg, NULL, REPLY, REPLY);
        rethrow_exc_wparam(msg);
    }

    void ThreadController::repaintThread() {
        MSG msg;
        PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
        InterlockedExchange(&_threadStarted, 1);
        while(true) {
            GetMessage(&msg, NULL, 0, 0);
            if(msg.message == STOP){
                PostThreadMessage(static_cast<DWORD>(msg.wParam), REPLY, 0, 0);
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
                try {
                    _window->getRenderer()->draw(&_reader);
                } catch (const std::exception &){
                    LeaveCriticalSection(&_queueCS);
                    std::exception_ptr *ptrMem = new std::exception_ptr;
                    *ptrMem = std::current_exception();
                    PostThreadMessage(
                            static_cast<DWORD>(msg.wParam), REPLY,
                            reinterpret_cast<WPARAM>(ptrMem), true
                    );
                    break;
                }
            }
            _window->getRenderer()->repaint();
            PostThreadMessage(static_cast<DWORD>(msg.wParam), REPLY, 0, 0);
        }
    }

    void ThreadController::stopRepaintThread() {
        while(!InterlockedCompareExchange(&_threadStarted, 0, 0)){
            Sleep(100);
        }
        MSG msg;
        PostThreadMessage(_drawThreadId, STOP, GetCurrentThreadId(), 0);
        GetMessage(&msg, NULL, REPLY, REPLY);
    }

    ThreadController::~ThreadController() {
        stopRepaintThread();
        DeleteCriticalSection(&_addCS);
        DeleteCriticalSection(&_queueCS);
        delete [] _currentProps.userFillPattern;
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