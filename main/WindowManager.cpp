#include "WindowManager.h"
#include "ThreadController.h"
#include "WException.h"
#include "WinManException.h"
#include "error_handler.h"

#undef max

namespace directgraph{
    const DirectgraphWinIndex WindowManager::NO_CURRENT_WINDOW = std::numeric_limits<DirectgraphWinIndex>::max();

    WindowManager::WindowManager(
            std::vector<IWindowFactory *> &rendFactories,
            IControllerFactory *ctrlFactory
    ): _rendFactories(rendFactories), _ctrlFactory(ctrlFactory), _curWindowIndex(NO_CURRENT_WINDOW), _curMapIndex(0)
    {
        InitializeCriticalSection(&_dpiCS);
    }

    void WindowManager::setCurrentWindow(DirectgraphWinIndex winIndex) {
        _mapLock.startRead();
        _curWindowLock.startWrite();
        WinManMap::iterator it = _windows.find(winIndex);
        if(it == _windows.end()){
            _curWindowLock.endWrite();
            _mapLock.endRead();
            THROW_EXC_CODE(
                    WinManException,
                    WRONG_WINDOW_INDEX,
                    std::wstring(L"Wrong window index when setting window"),
                    winIndex
            );
        }
        _curWindowIndex = winIndex;
        _curWindowLock.endWrite();
        _mapLock.endRead();
    }

    DirectgraphWinIndex WindowManager::createWindow(const DirectgraphWinParams &params) {
        DirectgraphWinIndex *index = new DirectgraphWinIndex;
        *index = static_cast<DirectgraphWinIndex>(InterlockedIncrement(&_curMapIndex));
        _mapLock.startWrite();
        IMyWindow *win = NULL;
        IController *controller = NULL;
        CommonProps props = get_default_common_props();
        try {
            win = _rendFactories[params.renderer]->createDPIWindow(
                    params.name, params.width, params.height,
                    params.dpiX, params.dpiY, props
            );
            win->show();
            win->addListener(this, index);
            controller = _ctrlFactory->createMultThreadController(win, props);
            controller->init();
            WindowData data = {controller, win, params.renderer, index, GetCurrentThreadId()};
            _windows.insert(std::pair<DirectgraphWinIndex, WindowData>(
                    *index, data
            ));
        } catch (const std::exception &){
            _mapLock.endWrite();
            delete win;
            delete controller;
            throw;
        }
        if (_curWindowIndex == NO_CURRENT_WINDOW) {
            _curWindowIndex = *index;
        }
        _mapLock.endWrite();
        return *index;
    }

    void WindowManager::destroyWindow(DirectgraphWinIndex winIndex) {
        _mapLock.startRead();
        WinManMap::iterator it = _windows.find(winIndex);
        if(it == _windows.end()){
            _mapLock.endRead();
            THROW_EXC_CODE(
                    WinManException,
                    WRONG_WINDOW_INDEX,
                    std::wstring(L"Wrong window index when destroying window"),
                    winIndex
            );
        }
        DWORD threadId = _windows[winIndex].threadId;
        _mapLock.endRead();

        if(threadId != GetCurrentThreadId()){
            MSG msg;
            PostThreadMessageW(threadId, DIRECTGRAPH_WND_DESTROY, GetCurrentThreadId(), static_cast<LPARAM>(winIndex));
            GetMessage(&msg, NULL, DIRECTGRAPH_REPLY, DIRECTGRAPH_REPLY);
            rethrow_exc_wparam(msg);
            return;
        }

        _mapLock.startWrite();
        it = _windows.find(winIndex);
        if(it == _windows.end()){
            _mapLock.endWrite();
            THROW_EXC_CODE(
                    WinManException,
                    WRONG_WINDOW_INDEX,
                    std::wstring(L"Wrong window index when destroying window"),
                    winIndex
            );
        }

        delete(it->second.winIndexMem);
        _ctrlFactory->deleteController(it->second.ctrl);
        _rendFactories[it->second.renderer]->deleteWindow(it->second.win);
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
        DeleteCriticalSection(&_dpiCS);
    }

    WindowManager::ControllerAndIndex WindowManager::getCurrentWindowAndLock(bool throwExc) {
        _mapLock.startRead();
        _curWindowLock.startRead();
        if(throwExc && _curWindowIndex == NO_CURRENT_WINDOW){
            releaseCurrentWindowLock();
            THROW_EXC_CODE(WinManException, NO_WINDOWS, std::wstring(L"No windows to draw"));
        }
        IController *ctrl = (_curWindowIndex == NO_CURRENT_WINDOW) ? NULL : _windows[_curWindowIndex].ctrl;
        ControllerAndIndex retVal = {ctrl, _curWindowIndex};
        return retVal;
    }

    WindowManager::ControllerAndIndex WindowManager::getCurrentWindowAndLock() {
        return getCurrentWindowAndLock(true);
    }

    void WindowManager::onClose(void *param) {
        EXC_CALL_WRAP(destroyWindow(*static_cast<DirectgraphWinIndex *>(param));)
    }

    void WindowManager::releaseCurrentWindowLock() {
        _curWindowLock.endRead();
        _mapLock.endRead();
    }

    IController *WindowManager::getWindowByIndexAndLock(DirectgraphWinIndex index) {
        _mapLock.startRead();
        WinManMap::iterator it = _windows.find(index);
        if(it == _windows.end()){
            releaseWindowLock();
            THROW_EXC_CODE(WinManException, WRONG_WINDOW_INDEX, std::wstring(L"Wrong window index"), index);
        }
        return it->second.ctrl;
    }

    void WindowManager::releaseWindowLock() {
        _mapLock.endRead();
    }

    IWindowFactory::DPIInfo WindowManager::getDPIInfo() {
        EnterCriticalSection(&_dpiCS);
        IWindowFactory::DPIInfo info = _rendFactories[0]->getDPIInfo();
        LeaveCriticalSection(&_dpiCS);
        return info;
    }

    WindowManagerScopedLock::WindowManagerScopedLock(
            WindowManager *winMan, DirectgraphWinIndex index
    ): _winMan(winMan), _lockType(WINDOW_BY_INDEX){
        data.controller = _winMan->getWindowByIndexAndLock(index);
        data.index = index;
    }

    WindowManagerScopedLock::WindowManagerScopedLock(WindowManager *winMan): _winMan(winMan), _lockType(CURRENT_WINDOW){
        data = _winMan->getCurrentWindowAndLock();
    }

    WindowManagerScopedLock::~WindowManagerScopedLock() {
        switch (_lockType){
            case CURRENT_WINDOW:
                _winMan->releaseCurrentWindowLock();
                break;
            case WINDOW_BY_INDEX:
                _winMan->releaseWindowLock();
                break;
        }
    }
};
