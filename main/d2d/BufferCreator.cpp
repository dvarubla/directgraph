#include "BufferCreator.h"

namespace directgraph {
    BufferCreator::BufferCreator(DPIHelper *helper):_helper(helper), _curBuffer(NULL) {
    }

    void BufferCreator::setTarget(ID2D1HwndRenderTarget *rTarget) {
        _rTarget = rTarget;
    }

    ID2D1BitmapRenderTarget* BufferCreator::getBuffer(float width, float height) {
        if(_curBuffer == NULL) {
            _width = width;
            _height = height;
            _rTarget->CreateCompatibleRenderTarget(
                    D2D1::SizeF(_width, _height),
                    D2D1::SizeU(_helper->toPixelsX(_width), _helper->toPixelsY(_height)),
                    _rTarget->GetPixelFormat(),
                    &_curBuffer
            );
        }
        return _curBuffer;
    }

    BufferCreator::~BufferCreator() {
        _curBuffer->Release();
    }
}
