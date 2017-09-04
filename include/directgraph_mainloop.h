#ifndef DIRECTGRAPH_MAINLOOP_H
#define DIRECTGRAPH_MAINLOOP_H
//#gtest <directgraph_d2d_export.h>
#include <windows.h>

enum DirectGraphEvent{
    DIRECTGRAPH_WND_CREATE = WM_USER+1,
    DIRECTGRAPH_WND_QUIT
};

WPARAM directgraph_mainloop();

#endif //DIRECTGRAPH_MAINLOOP_H
