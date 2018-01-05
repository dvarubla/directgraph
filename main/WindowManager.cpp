#include "WindowManager.h"
#include "ThreadController.h"

#undef max

namespace directgraph{
    const DirectgraphWinIndex WindowManager::NO_CURRENT_WINDOW = std::numeric_limits<DirectgraphWinIndex>::max();

    WindowManager::WindowManager(
            std::vector<IWindowFactory *> &rendFactories,
            IControllerFactory *ctrlFactory
    ): _rendFactories(rendFactories), _ctrlFactory(ctrlFactory), _curWindowIndex(NO_CURRENT_WINDOW), _curMapIndex(0)
    {
    }

    void WindowManager::setCurrentWindow(DirectgraphWinIndex winIndex) {
        _mapLock.startRead();
        _curWindowLock.startWrite();
        _curWindowIndex = winIndex;
        _curWindowLock.endWrite();
        _mapLock.endRead();
    }

    DirectgraphWinIndex WindowManager::createWindow(const DirectgraphWinParams &params) {
        DirectgraphWinIndex *index = new DirectgraphWinIndex;
        *index = static_cast<DirectgraphWinIndex>(InterlockedIncrement(&_curMapIndex));
        IMyWindow *win = _rendFactories[params.renderer]->createPixelWindow(params.name, params.width, params.height);
        win->show();
        win->addListener(this, index);
        IController *controller = _ctrlFactory->createMultThreadController(win);
        controller->init();
        _mapLock.startWrite();
        _windows.insert(std::pair<DirectgraphWinIndex, WindowData>(
                *index, {controller, win, params.renderer, index, GetCurrentThreadId()}
        ));
        if(_curWindowIndex == NO_CURRENT_WINDOW){
            _curWindowIndex = *index;
        }
        _mapLock.endWrite();
        return *index;
    }

    void WindowManager::destroyWindow(DirectgraphWinIndex winIndex) {
        _mapLock.startRead();
        DWORD threadId = _windows[winIndex].threadId;
        _mapLock.endRead();

        if(threadId != GetCurrentThreadId()){
            MSG msg;
            PostThreadMessageW(threadId, DIRECTGRAPH_WND_DESTROY, GetCurrentThreadId(), static_cast<LPARAM>(winIndex));
            GetMessage(&msg, NULL, DIRECTGRAPH_REPLY, DIRECTGRAPH_REPLY);
            return;
        }

        _mapLock.startWrite();
        delete(_windows[winIndex].winIndexMem);
        _ctrlFactory->deleteController(_windows[winIndex].ctrl);
        _rendFactories[_windows[winIndex].renderer]->deleteWindow(_windows[winIndex].win);
        _windows.erase(winIndex);
        if(_curWindowIndex == winIndex){
            if(_windows.empty()) {
                _curWindowIndex = NO_CURRENT_WINDOW;
            } else {
                _curWindowIndex = _windows.begin()->first;
            }
        }
        _mapLock.endWrite();

        PostThreadMessageW(threadId, DIRECTGRAPH_WND_DESTROYED, winIndex, 0);
    }
    WindowManager::~WindowManager() {
        for(uint_fast32_t i = 0; i < _rendFactories.size(); i++){
            delete(_rendFactories[i]);
        }
        delete _ctrlFactory;
    }

    WindowManager::ControllerAndIndex WindowManager::getCurrentWindowAndLock() {
        _mapLock.startRead();
        _curWindowLock.startRead();
        IController *ctrl = (_curWindowIndex == NO_CURRENT_WINDOW) ? NULL : _windows[_curWindowIndex].ctrl;
        return {ctrl, _curWindowIndex};
    }

    void WindowManager::onClose(void *param) {
        destroyWindow(*static_cast<DirectgraphWinIndex*>(param));
    }

    void WindowManager::releaseCurrentWindowLock() {
        _curWindowLock.endRead();
        _mapLock.endRead();
    }

    IController *WindowManager::getWindowByIndexAndLock(DirectgraphWinIndex index) {
        _mapLock.startRead();
        return _windows[index].ctrl;
    }

    void WindowManager::releaseWindowLock() {
        _mapLock.endRead();
    }
};
