#include "MyWindowStub.h"

namespace directgraph {
    IRenderer *MyWindowStub::getRenderer() {
        return _renderer;
    }

    MyWindowStub::MyWindowStub(IRenderer *renderer): _renderer(renderer) {
    }

    MyWindowStub::~MyWindowStub() {
        delete _renderer;
    }

    void MyWindowStub::show() {
    }

    void MyWindowStub::hide() {
    }

    void MyWindowStub::addListener(IWindowListener *, void *) {
    }
}