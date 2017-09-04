#include <cstdio>
#include "D2DRenderer.h"

namespace directgraph {
    D2DRenderer::D2DRenderer(D2DCommon *common, DPIHelper *helper, DPIConverter *converter, float width, float height) {
        _width = width;
        _height = height;
        _helper = helper;
        _converter = converter;
        _bufferCreator = new BufferCreator(helper);
        _common = common;
        _D2DFactory = _common->getFactory();
    }

    void D2DRenderer::createDeviceRes() {
        HRESULT hr = _D2DFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(
                        _hwnd,
                        D2D1::SizeU(
                                _helper->toPixelsX(_width),
                                _helper->toPixelsY(_height)
                        )
                ),
                &_rTarget
        );
        _rTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
        _rTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::Red),
                &_solidBrush
        );
        _bufferCreator->setTarget(_rTarget);
        _buffer = _bufferCreator->getBuffer(_width, _height);
        _currentBrush = _solidBrush;
        clear();
    }

    void D2DRenderer::setWindow(HWND hwnd) {
        _hwnd = hwnd;
        createDeviceRes();
    }

    void D2DRenderer::repaint() {
        _rTarget->BeginDraw();
        ID2D1Bitmap *bitmap;
        _buffer->GetBitmap(&bitmap);
        _rTarget->DrawBitmap(bitmap);
        bitmap->Release();
        _rTarget->EndDraw();
    }

    void D2DRenderer::bar(float left, float right, float top, float bottom) {
        _buffer->BeginDraw();
        _buffer->FillRectangle(
                D2D1::RectF(
                    _converter->convertX(left), _converter->convertY(right),
                    _converter->convertX(top), _converter->convertY(bottom)
                ),
                _currentBrush
        );
        _buffer->EndDraw();
    }

    void D2DRenderer::clear() {
        _buffer->BeginDraw();
        _buffer->Clear(D2D1::ColorF(D2D1::ColorF::White));
        _buffer->EndDraw();
    }

    void D2DRenderer::setcolor(uint_fast32_t color) {
         uint_fast32_t invColor =
                 ((color&0xFF0000) >> 16) |
                 (color&0xFF00) |
                 ((color&0xFF) << 16);
        _solidBrush->SetColor(D2D1::ColorF(invColor));
    }

    D2DRenderer::~D2DRenderer() {
        delete _bufferCreator;
        _solidBrush->Release();
        _rTarget->Release();
        delete _helper;
        delete _converter;
    }
}