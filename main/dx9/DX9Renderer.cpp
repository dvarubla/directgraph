#include <resource.h>
#include <Queue.h>
#include "DX9Renderer.h"
#include <graphics_const_internal.h>

namespace directgraph {
    DX9Renderer::DX9Renderer(DX9Common *common, DPIHelper *helper, float width, float height) {
        _helper = helper;
        _width = width;
        _height = height;
        _common = common;
    }

    void DX9Renderer::setWindow(HWND hwnd) {
        _hwnd = hwnd;
        createDeviceRes();
    }

    void DX9Renderer::repaint() {
        _device->SetRenderTarget(0, _backBuffer);
        _swapChain->Present(NULL, NULL, NULL, NULL, 0);
    }

    void DX9Renderer::createDeviceRes() {
        _swapChain = _common->createSwapChain(_hwnd, _helper->toPixelsX(_width), _helper->toPixelsX(_height));
        _swapChain->GetDevice(&_device);
        _swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &_backBuffer);

        _device->CreateVertexBuffer(VERTEX_BUFFER_SIZE * sizeof(RectVertex),
                                          D3DUSAGE_DYNAMIC,
                                          0,
                                          D3DPOOL_DEFAULT,
                                          &_rectVertBuffer, NULL);
        _rectVertMem = new RectVertex[VERTEX_BUFFER_SIZE];
    }

    DX9Renderer::~DX9Renderer() {
        _rectVertBuffer->Release();
        _common->deleteSwapChain(_swapChain);
        delete _helper;
        delete [] _rectVertMem;
    }

    void DX9Renderer::draw(QueueReader *reader, CommonProps *props) {
        QueueItem &firstItem = reader->getAt(0);
        if(firstItem.type == QueueItem::CLEAR){
            reader->endReading(1);
            _device->SetRenderTarget(0, _backBuffer);
            _device->Clear(
                    0, NULL, D3DCLEAR_TARGET,
                    D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0),
                    1.0f,
                    0
            );
        } else {
            uint_fast32_t size = reader->getSize();
            uint_fast32_t readIndex = 0;
            uint_fast32_t totalNumVertices = 0;
            int_fast32_t prevX = 0, prevY = 0;
            bool isFirst = true;
            for(readIndex = 0; readIndex < size; readIndex++){
                QueueItem &item = reader->getAt(readIndex);
                if(item.type == QueueItem::SINGLE_PIXEL ||
                        (item.type == QueueItem::BAR && props->fillStyle == SOLID_FILL)){
                    uint_fast32_t curNumVertices = (isFirst) ? 4 : 6;
                    if((totalNumVertices + curNumVertices) >= VERTEX_BUFFER_SIZE){
                        break;
                    }
                    RectVertex *rectVectMem = _rectVertMem + totalNumVertices;
                    if(!isFirst) {
                        rectVectMem = genDegenerate(
                                rectVectMem,
                                prevX, prevY,
                                ((item.type == QueueItem::SINGLE_PIXEL) ?
                                 static_cast<int_fast32_t>(item.data.singlePixel.x) :
                                 _helper->toPixelsX(item.data.bar.left)),
                                ((item.type == QueueItem::SINGLE_PIXEL) ?
                                 static_cast<int_fast32_t>(item.data.singlePixel.y):
                                 _helper->toPixelsY(item.data.bar.top))
                        );
                    }
                    if(item.type == QueueItem::SINGLE_PIXEL){
                        genQuad(rectVectMem,
                                item.data.singlePixel.x,
                                item.data.singlePixel.y,
                                item.data.singlePixel.x + 1,
                                item.data.singlePixel.y + 1,
                                item.data.singlePixel.color
                        );
                        prevX = item.data.singlePixel.x + 1;
                        prevY = item.data.singlePixel.y + 1;
                    } else {
                        genQuad(rectVectMem,
                                _helper->toPixelsX(item.data.bar.left),
                                _helper->toPixelsY(item.data.bar.top),
                                _helper->toPixelsX(item.data.bar.right),
                                _helper->toPixelsY(item.data.bar.bottom),
                                props->color
                        );
                        prevX = _helper->toPixelsX(item.data.bar.right);
                        prevY = _helper->toPixelsY(item.data.bar.bottom);
                    }
                    isFirst = false;
                    totalNumVertices += curNumVertices;
                } else if(item.type == QueueItem::SETFILLSTYLE){
                    props->color = item.data.setfillstyle.color;
                    props->fillStyle = item.data.setfillstyle.fillStyle;
                } else {
                    break;
                }
            }
            if(readIndex != 0) {
                reader->endReading(readIndex);
                void* voidPointer;
                _rectVertBuffer->Lock(0, totalNumVertices * sizeof(RectVertex), &voidPointer, D3DLOCK_DISCARD);
                memcpy(voidPointer, _rectVertMem, totalNumVertices * sizeof(RectVertex));
                _rectVertBuffer->Unlock();
                _device->SetStreamSource(0, _rectVertBuffer, 0, sizeof(RectVertex));
                _device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
                _device->BeginScene();
                _device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, totalNumVertices - 2);
                _device->EndScene();
            }
        }
    }

    uint_fast32_t DX9Renderer::swapColor(uint_fast32_t color) {
        return (color & 0x00FF00) | ((color & 0xFF) << 16) | ((color & 0xFF0000) >> 16);
    }

    DX9Renderer::RectVertex *
    DX9Renderer::genDegenerate(
            DX9Renderer::RectVertex *vertices,
            int_fast32_t startX, int_fast32_t startY,
            int_fast32_t endX, int_fast32_t endY
    ) {
        *vertices = {
                static_cast<float>(startX) - 0.5f,
                static_cast<float>(startY) - 0.5f,
                0.0f,
                1.0f,
                D3DCOLOR_ARGB(0, 0, 0, 0)
        };
        vertices++;
        *vertices = {
                static_cast<float>(endX) - 0.5f,
                static_cast<float>(endY) - 0.5f,
                0.0f,
                1.0f,
                D3DCOLOR_ARGB(0, 0, 0, 0)
        };
        vertices++;
        return vertices;
    }

    DX9Renderer::RectVertex *
    DX9Renderer::genQuad(
            DX9Renderer::RectVertex *vertices,
            int_fast32_t startX, int_fast32_t startY,
            int_fast32_t endX, int_fast32_t endY,
            uint_fast32_t color
    ) {
        *vertices = {
                static_cast<float>(startX) - 0.5f,
                static_cast<float>(startY) - 0.5f,
                0.0f,
                1.0f,
                swapColor(color)
        };
        vertices++;
        *vertices = {
                static_cast<float>(endX) - 0.5f,
                static_cast<float>(startY) - 0.5f,
                0.0f,
                1.0f,
                swapColor(color)
        };
        vertices++;
        *vertices = {
                static_cast<float>(startX) - 0.5f,
                static_cast<float>(endY) - 0.5f,
                0.0f,
                1.0f,
                swapColor(color)
        };
        vertices++;
        *vertices = {
                static_cast<float>(endX) - 0.5f,
                static_cast<float>(endY) - 0.5f,
                0.0f,
                1.0f,
                swapColor(color)
        };
        vertices++;
        return vertices;
    }
}
