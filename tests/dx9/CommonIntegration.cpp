#include <ControllerFactory.h>
#include "CommonIntegration.h"

WindowManager *CommonIntegration::wman = NULL;
WindowFactory *CommonIntegration::wf = NULL;
WindowFactoryStub *CommonIntegration::wfstub = NULL;
using testing::NiceMock;
using testing::Invoke;
using testing::_;

void CommonIntegration::initWindowManager() {
    if(wf == NULL){
        wf = new WindowFactory();
    }
    wfstub = new NiceMock<WindowFactoryStub>();
    ON_CALL(*wfstub, createWindow(_, _, _, _)).WillByDefault(Invoke(this, &CommonIntegration::createWindow));
    ON_CALL(*wfstub, deleteWindow(_)).WillByDefault(Invoke(wf, &IWindowFactory::deleteWindow));
    if(wman == NULL){
        std::vector<IWindowFactory*> vect;
        vect.push_back(wfstub);
        wman = new WindowManager(vect, new ControllerFactory());
    }
}

IMyWindow* CommonIntegration::createWindow(const wchar_t *name, uint32_t width, uint32_t height,
                                                const CommonProps &props) {
    _curWindow = wf->createWindow(name, width, height, props);
    return _curWindow;
}

BitmapWrap *CommonIntegration::afterTestSimple(DirectgraphWinIndex index) {
    MSG msg;
    while (PeekMessageW(&msg, NULL, 0, 0, PM_NOREMOVE)){
        GetMessageW(&msg, NULL, 0, 0);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    BitmapWrap *ret = new BitmapWrap(dynamic_cast<MyWindow*>(_curWindow)->getHWND());
    wman->destroyWindow(index);
    return ret;
}

CommonIntegration::~CommonIntegration() {
    delete wman;
    wman = NULL;
}
