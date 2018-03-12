#include "common.h"
#include <directgraph/directgraph_constants.h>

using testing::_;
using testing::Invoke;

static const int MSG_CODE = WM_USER + 6000;
static const int MSG_CREATE = WM_USER + 6001;
static const int MSG_DESTROY = WM_USER + 6002;
static const int MSG_CREATE_DPI = WM_USER + 6003;

BitmapWrap *CommonSimple::afterTestSimple(IMyWindow *win, IQueueReader *reader) {
    while(reader->getSize() != 0) {
        win->getRenderer()->draw(reader);
    }
    win->getRenderer()->repaint();
    BitmapWrap *wrap = new BitmapWrap(dynamic_cast<MyWindow*>(win)->getHWND());
    PostThreadMessage(windowThreadId, MSG_DESTROY, reinterpret_cast<WPARAM>(win), 0);
    MSG msg;
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    return wrap;
}

CommonSimple::CommonSimple(): _curFeatures(NULL) {

}

WindowFactory *CommonSimple::_dx9Wf = NULL;
bool CommonSimple::threadStarted = false;
DWORD CommonSimple::windowThreadId;

struct Param{
    DWORD threadId;
};

struct DrawParam{
    float w;
    float h;
};

struct DrawDPIParam{
    float w;
    float h;
    float dpiX;
    float dpiY;
};

static DWORD WINAPI mainWindowThread(LPVOID param){
    Param p = *reinterpret_cast<Param*>(param);
    MSG msg;
    CommonProps props = get_default_common_props();
    CommonSimple::_dx9Wf = new WindowFactory();
    PostThreadMessage(p.threadId, MSG_CODE, 0, 0);
    bool windowSent = false;
    IMyWindow* win = NULL;
    while (GetMessageW(&msg, NULL, 0, 0)){
        if(msg.message == MSG_CREATE){
            DrawParam *drawParam = reinterpret_cast<DrawParam*>(msg.wParam);
            win = CommonSimple::_dx9Wf->createPixelWindow(L"Hello", drawParam->w, drawParam->h, props);
            win->show();
            windowSent = false;
        } else if(msg.message == MSG_CREATE_DPI){
            DrawDPIParam *drawDPIParam = reinterpret_cast<DrawDPIParam*>(msg.wParam);
            win = CommonSimple::_dx9Wf->createDPIWindow(
                    L"Hello", drawDPIParam->w, drawDPIParam->h,
                    drawDPIParam->dpiX, drawDPIParam->dpiY,
                    props
            );
            win->show();
            windowSent = false;
        } else if(msg.message == MSG_DESTROY){
            CommonSimple::_dx9Wf->deleteWindow(reinterpret_cast<IMyWindow *>(msg.wParam));
            PostThreadMessage(p.threadId, MSG_CODE, 0, 0);
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_PAINT && !windowSent) {
                windowSent = true;
                PostThreadMessage(p.threadId, MSG_CODE, reinterpret_cast<WPARAM>(win), 0);
            }
        }
    }
    return 0;
}

void CommonSimple::startThread() {
    if(!threadStarted){
        MSG msg;
        Param p = {GetCurrentThreadId()};
        CreateThread(NULL, 0, mainWindowThread, &p, 0, &windowThreadId);
        GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
        threadStarted = true;
    }
}

IMyWindow* CommonSimple::createWindow(float w, float h) {
    startThread();
    MSG msg;
    DrawParam dp = {w, h};
    PostThreadMessage(windowThreadId, MSG_CREATE, reinterpret_cast<WPARAM>(&dp), 0);
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    IMyWindow *win = reinterpret_cast<IMyWindow *>(msg.wParam);
    return win;
}

IMyWindow* CommonSimple::createDPIWindow(float w, float h, float dpiX, float dpiY) {
    startThread();
    MSG msg;
    DrawDPIParam dp = {w, h, dpiX, dpiY};
    PostThreadMessage(windowThreadId, MSG_CREATE_DPI, reinterpret_cast<WPARAM>(&dp), 0);
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    IMyWindow *win = reinterpret_cast<IMyWindow *>(msg.wParam);
    return win;
}

void CommonSimple::addOnCall(NiceMock<QueueReaderStub> &readerStub) {
    ON_CALL(readerStub, addItems(_, _)).WillByDefault(Invoke(&readerStub, &QueueReaderStub::addItemsImpl));
    ON_CALL(readerStub, getSize()).WillByDefault(Invoke(&readerStub, &QueueReaderStub::getSizeImpl));
    ON_CALL(readerStub, getAt(_)).WillByDefault(Invoke(&readerStub, &QueueReaderStub::getAtImpl));
    ON_CALL(readerStub, endReading(_)).WillByDefault(Invoke(&readerStub, &QueueReaderStub::endReadingImpl));
}

FeaturesStub *CommonSimple::setupFeatures() {
    FeaturesStub *fstub = new NiceMock<FeaturesStub>();
    _curFeatures = _dx9Wf->getCommon()->getFeatures();
    _dx9Wf->getCommon()->setFeatures(fstub);
    ON_CALL(*fstub, getDeviceFlags(_, _)).WillByDefault(Invoke(_curFeatures, &IFeatures::getDeviceFlags));
    ON_CALL(*fstub, getPatternTexFormat()).WillByDefault(Invoke(_curFeatures, &IFeatures::getPatternTexFormat));
    ON_CALL(*fstub, getTranspPatternTexFormat()).WillByDefault(Invoke(_curFeatures, &IFeatures::getTranspPatternTexFormat));
    ON_CALL(*fstub, getDisplayMode()).WillByDefault(Invoke(_curFeatures, &IFeatures::getDisplayMode));
    ON_CALL(*fstub, supportsTexConst()).WillByDefault(Invoke(_curFeatures, &IFeatures::supportsTexConst));
    ON_CALL(*fstub, getVertexShaderVer()).WillByDefault(Invoke(_curFeatures, &IFeatures::getVertexShaderVer));
    ON_CALL(*fstub, getPixelShaderVer()).WillByDefault(Invoke(_curFeatures, &IFeatures::getPixelShaderVer));
    return fstub;
}

CommonSimple::~CommonSimple() {
    if(_curFeatures != NULL){
        delete _dx9Wf->getCommon()->getFeatures();
        _dx9Wf->getCommon()->setFeatures(_curFeatures);
    }
}
