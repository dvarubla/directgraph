#include "MyWindow.h"
#include <map>
#include <graphics_const_internal.h>
#include <common.h>

namespace directgraph {
    const static std::wstring CLASS_NAME = L"directgraph";
    static WNDCLASSEXW windowClass = {};

    typedef std::map<HWND, IRenderer *> WindowMap;
    static WindowMap windowMap;

    LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
        WindowMap::iterator it = windowMap.find(hwnd);
        if (it != windowMap.end()) {
            IRenderer *renderer = it->second;
            switch (message) {
                case WM_PAINT:
                    PAINTSTRUCT s;
                    BeginPaint(hwnd, &s);
                    renderer->repaint();
                    EndPaint(hwnd, &s);
                    break;
                case WM_CLOSE:
                    PostThreadMessageW(GetCurrentThreadId(), DIRECTGRAPH_WND_QUIT, 0, 0);
                    break;
                default:
                    break;
            }
        }
        return DefWindowProcW(hwnd, message, wParam, lParam);
    }

    MyWindow::MyWindow(const wchar_t *name, uint_fast32_t width, uint_fast32_t height) {
        RECT rect;
        rect.left = rect.top = 0;
        rect.right = (long)width;
        rect.bottom = (long)height;
        AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);
        PostThreadMessageW(GetCurrentThreadId(), DIRECTGRAPH_WND_CREATED, 0, 0);
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
        windowMap.insert(std::pair<HWND, IRenderer *>(_hwnd, _renderer));
    }

    IRenderer *MyWindow::getRenderer() {
        return _renderer;
    }

    MyWindow::~MyWindow() {
        delete _renderer;
        DestroyWindow(_hwnd);
    }

    HWND MyWindow::getHWND() {
        return _hwnd;
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
        }
    }
}
