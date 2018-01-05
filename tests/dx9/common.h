#ifndef PROJECT_COMMON_H
#define PROJECT_COMMON_H
#include <main/dx9/DX9WindowFactory.h>
#include <testlib/testlib_funcs.h>
#include <tests/stubs/QueueReaderStub.h>

using namespace directgraph;
using namespace directgraph_testlib;
using testing::NiceMock;

class CommonSimple{
public:
    CommonSimple();
    static DX9WindowFactory *_dx9Wf;
    IMyWindow* createWindow(float w, float h);
    BitmapWrap* afterTestSimple(IMyWindow *win, IQueueReader *reader);
    void addOnCall(NiceMock<QueueReaderStub> &readerStub);
    void init_factory();
};

#endif //PROJECT_COMMON_H
