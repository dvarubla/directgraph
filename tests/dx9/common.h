#ifndef PROJECT_COMMON_H
#define PROJECT_COMMON_H
#include <main/dx9/DX9WindowFactory.h>
#include <testlib/testlib_funcs.h>

using namespace directgraph;
using namespace directgraph_testlib;

static DX9WindowFactory *_dx9Wf = NULL;

void init_factory(){
    if(_dx9Wf == NULL) {
        _dx9Wf = new DX9WindowFactory();
    }
}

BitmapWrap* afterTestSimple(MyWindow *win, IQueueReader *reader){
    CommonProps props;
    while(reader->getSize() != 0) {
        win->getRenderer()->draw(reader, &props);
    }
    win->getRenderer()->repaint();
    BitmapWrap *wrap = new BitmapWrap(win->getHWND());
    delete win;
    return wrap;
}

#endif //PROJECT_COMMON_H
