#include <ControllerFactory.h>
#include "CommonIntegration.h"

WindowManager *CommonIntegration::wman = NULL;
WindowFactory *CommonIntegration::wf = NULL;

using testing::NiceMock;
using testing::Invoke;
using testing::_;

void CommonIntegration::initWindowManager() {
    if(wf == NULL){
        wf = new WindowFactory();
    }
    wfstub = new NiceMock<WindowFactoryStub>();
    ON_CALL(*wfstub, createPixelWindow(_, _, _, _)).WillByDefault(Invoke(this, &CommonIntegration::createPixelWindow));
    ON_CALL(*wfstub, deleteWindow(_)).WillByDefault(Invoke(wf, &IWindowFactory::deleteWindow));
    if(wman == NULL){
        std::vector<IWindowFactory*> vect;
        vect.push_back(wfstub);
        wman = new WindowManager(vect, new ControllerFactory());
    }
}

IMyWindow* CommonIntegration::createPixelWindow(const wchar_t *name, float width, float height,
                                                const CommonProps &props) {
    _curWindow = wf->createPixelWindow(name, width, height, props);
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
    delete wfstub;
}
