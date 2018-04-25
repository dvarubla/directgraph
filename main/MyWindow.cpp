#include "MyWindow.h"
#include "WException.h"
#include <map>
#include <common.h>

namespace directgraph {
    const static std::wstring CLASS_NAME = L"directgraph";
    static WNDCLASSEXW windowClass = {};

    typedef std::map<HWND, MyWindow *> WindowMap;
    static CRITICAL_SECTION windowMapCS;
    static WindowMap windowMap;

    LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
        EnterCriticalSection(&windowMapCS);
        WindowMap::iterator it = windowMap.find(hwnd);
        if (it != windowMap.end()) {
            MyWindow *win = it->second;
            LeaveCriticalSection(&windowMapCS);
            IRenderer *renderer = win->getRenderer();
            std::vector<MyWindow::ListenerData> listenerData = win->getListeners();
            switch (message) {
                case WM_PAINT:
                    PAINTSTRUCT s;
                    BeginPaint(hwnd, &s);
                    renderer->repaint();
                    EndPaint(hwnd, &s);
                    return 0;
                case WM_CLOSE:
                    for(uint_fast32_t i = 0; i < listenerData.size(); i++){
                        listenerData[i].listener->onClose(listenerData[i].param);
                    }
                    break;
                default:
                    break;
            }
        } else {
            LeaveCriticalSection(&windowMapCS);
        }
        return DefWindowProcW(hwnd, message, wParam, lParam);
    }

    MyWindow::MyWindow(const wchar_t *name, uint_fast32_t width, uint_fast32_t height)
            : _width(width), _height(height) {
        RECT rect;
        rect.left = rect.top = 0;
        rect.right = (long)width;
        rect.bottom = (long)height;
        AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);
        _hwnd = CreateWindowExW(
                WS_EX_APPWINDOW,
                CLASS_NAME.c_str(),
                name,
                WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                rect.right - rect.left,
                rect.bottom - rect.top,
                NULL,
                NULL,
                HINST_THISCOMPONENT,
                NULL
        );
        if(_hwnd == NULL){
            THROW_EXC_CODE(WException, CANT_CREATE_WINDOW, std::wstring(L"Can't create window"));
        }
        PostThreadMessageW(GetCurrentThreadId(), DIRECTGRAPH_WND_CREATED, 0, 0);
    }

    void MyWindow::show() {
        ShowWindow(_hwnd, SW_SHOWNORMAL);
    }

    void MyWindow::hide() {
        ShowWindow(_hwnd, SW_HIDE);
    }

    void MyWindow::setRenderer(IRenderer *renderer) {
        _renderer = renderer;
        _renderer->setWindow(_hwnd);
        EnterCriticalSection(&windowMapCS);
        windowMap.insert(std::pair<HWND, MyWindow *>(_hwnd, this));
        LeaveCriticalSection(&windowMapCS);
    }

    IRenderer *MyWindow::getRenderer() {
        return _renderer;
    }

    MyWindow::~MyWindow() {
        EnterCriticalSection(&windowMapCS);
        windowMap.erase(_hwnd);
        LeaveCriticalSection(&windowMapCS);
        DestroyWindow(_hwnd);
    }

    HWND MyWindow::getHWND() {
        return _hwnd;
    }

    void MyWindow::addListener(IWindowListener *listener, void *param) {
        ListenerData data = {listener, param};
        _listeners.push_back(data);
    }

    std::vector<MyWindow::ListenerData>& MyWindow::getListeners() {
        return _listeners;
    }

    uint_fast32_t MyWindow::getWidth() {
        return _width;
    }

    uint_fast32_t MyWindow::getHeight() {
        return _height;
    }

    static volatile LONG canCreateWinClass = 1;

    void create_window_class() {
        if(InterlockedExchange(&canCreateWinClass, 0)) {
            windowClass.cbSize = sizeof(WNDCLASSEX);
            windowClass.style = CS_HREDRAW | CS_VREDRAW;
            windowClass.lpfnWndProc = window_proc;
            windowClass.hInstance = HINST_THISCOMPONENT;
            windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
            windowClass.lpszMenuName = NULL;
            windowClass.lpszClassName = CLASS_NAME.c_str();
            windowClass.hIconSm = NULL;

            RegisterClassExW(&windowClass);
            InitializeCriticalSection(&windowMapCS);
        }
    }
}
