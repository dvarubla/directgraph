#include "common.h"
#include <directgraph/directgraph_constants.h>

using testing::_;
using testing::Invoke;

BitmapWrap *CommonSimple::afterTestSimple(IMyWindow *win, IQueueReader *reader) {
    CommonProps props;
    props.fillStyle = SOLID_FILL;
    props.fillColor = 0xFFFFFF;
    props.bgColor = 0xFFFFFF;
    props.userFillPattern = NULL;
    while(reader->getSize() != 0) {
        win->getRenderer()->draw(reader, &props);
    }
    win->getRenderer()->repaint();
    BitmapWrap *wrap = new BitmapWrap(dynamic_cast<MyWindow*>(win)->getHWND());
    _dx9Wf->deleteWindow(win);
    delete [] props.userFillPattern;
    return wrap;
}

void CommonSimple::init_factory() {
    if(_dx9Wf == NULL) {
        _dx9Wf = new WindowFactory();
    }
}

CommonSimple::CommonSimple(): _curFeatures(NULL) {

}

WindowFactory *CommonSimple::_dx9Wf = NULL;

IMyWindow* CommonSimple::createWindow(float w, float h) {
    init_factory();
    IMyWindow* win = _dx9Wf->createPixelWindow(L"Hello", w, h);
    win->show();
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
    ON_CALL(*fstub, getDisplayMode()).WillByDefault(Invoke(_curFeatures, &IFeatures::getDisplayMode));
    ON_CALL(*fstub, supportsTexConst()).WillByDefault(Invoke(_curFeatures, &IFeatures::supportsTexConst));
    return fstub;
}

CommonSimple::~CommonSimple() {
    if(_curFeatures != NULL){
        delete _dx9Wf->getCommon()->getFeatures();
        _dx9Wf->getCommon()->setFeatures(_curFeatures);
    }
}
