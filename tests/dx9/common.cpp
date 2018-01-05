#include "common.h"
using testing::_;
using testing::Invoke;

BitmapWrap *CommonSimple::afterTestSimple(IMyWindow *win, IQueueReader *reader) {
    CommonProps props;
    while(reader->getSize() != 0) {
        win->getRenderer()->draw(reader, &props);
    }
    win->getRenderer()->repaint();
    BitmapWrap *wrap = new BitmapWrap(dynamic_cast<MyWindow*>(win)->getHWND());
    _dx9Wf->deleteWindow(win);
    return wrap;
}

void CommonSimple::init_factory() {
    if(_dx9Wf == NULL) {
        _dx9Wf = new DX9WindowFactory();
    }
}

CommonSimple::CommonSimple() {

}

DX9WindowFactory *CommonSimple::_dx9Wf = NULL;

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
