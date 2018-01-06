#include <resource.h>
#include <Queue.h>
#include "DX9Renderer.h"
#include <graphics_const_internal.h>
#include <math.h>
#include <main/QueueItem.h>
#include <algorithm>

#undef max
#undef min

namespace directgraph {
    template<>
    DX9Renderer::RectVertex VertexCreator::create<DX9Renderer::RectVertex>(float x, float y, float z, float rhw, DWORD color) {
        DX9Renderer::RectVertex v = {x ,y, z, rhw, color};
        return v;
    }

    template<>
    DX9Renderer::TexturedVertex VertexCreator::create<DX9Renderer::TexturedVertex>(float x, float y, float z, float rhw, float tu, float tv) {
        DX9Renderer::TexturedVertex v = {x ,y, z, rhw,tu, tv};
        return v;
    }

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
        _common->lock();
        _swapChain->Present(NULL, NULL, NULL, NULL, 0);
        _common->unlock();
    }

    void DX9Renderer::createDeviceRes() {
        uint_fast32_t pxWidth = static_cast<uint_fast32_t>(_helper->toPixelsX(_width));
        uint_fast32_t pxHeight = static_cast<uint_fast32_t>(_helper->toPixelsY(_height));
        _swapChain = _common->createSwapChain(_hwnd, pxWidth, pxHeight);
        _swapChain->GetDevice(&_device);
        _swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &_backBuffer);

        _pixelTextureWidth = static_cast<uint_fast32_t>(1 << (uint_fast32_t) ceil(log2<double>(pxWidth)));
        _pixelTextureHeight = static_cast<uint_fast32_t>(1 << (uint_fast32_t) ceil(log2<double>(pxHeight)));

        _device->CreateTexture(
                _pixelTextureWidth, _pixelTextureHeight, 1, 0,
                _common->getFormat(), D3DPOOL_MANAGED, &_pixelTexture, NULL
        );

        IPixelContainer::Format format = IPixelContainer::R5G6B5;

        switch(_common->getFormat()){
            case D3DFMT_R8G8B8: case D3DFMT_X8R8G8B8: case D3DFMT_A8R8G8B8:
                format = IPixelContainer::R8G8B8;
            break;
            case D3DFMT_R5G6B5: format = IPixelContainer::R5G6B5; break;
            default: break;
        }
        _pixContFactory = new PixelContainerFactory(pxWidth, pxHeight, format);

        _device->CreateVertexBuffer(VERTEX_BUFFER_SIZE * sizeof(RectVertex),
                                          D3DUSAGE_DYNAMIC,
                                          0,
                                          D3DPOOL_DEFAULT,
                                          &_vertBuffer, NULL);
        _vertMem = malloc(VERTEX_BUFFER_SIZE * sizeof(RectVertex));
    }

    DX9Renderer::~DX9Renderer() {
        _pixelTexture->Release();
        _vertBuffer->Release();
        _common->deleteSwapChain(_swapChain);
        delete _helper;
        delete _pixContFactory;
        free(_vertMem);
    }

    void DX9Renderer::draw(IQueueReader *reader, CommonProps *props) {
        QueueItem &firstItem = reader->getAt(0);
        if (firstItem.type == QueueItem::CLEAR) {
            reader->endReading(1);
            _device->SetRenderTarget(0, _backBuffer);
            _device->Clear(
                    0, NULL, D3DCLEAR_TARGET,
                    D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0),
                    1.0f,
                    0
            );
        } else if(firstItem.type == QueueItem::PIXEL_CONTAINER) {
            IPixelContainer *cont = firstItem.data.pixelContainer;
            reader->endReading(1);
            bool haveLastLine = cont->getLastStride() != cont->getStride();
            Rectangle firstCoords = cont->getFirstCoords();
            Rectangle lastCoords = cont->getLastCoords();
            firstCoords.right++;
            firstCoords.bottom++;
            lastCoords.right++;
            lastCoords.bottom++;
            RECT lockRect = {
                    static_cast<LONG>(firstCoords.left),
                    static_cast<LONG>(std::min(firstCoords.top, lastCoords.top)),
                    static_cast<LONG>(firstCoords.right),
                    static_cast<LONG>(std::max(firstCoords.bottom, lastCoords.top))
            };
            uint_fast32_t pxHeight = cont->getHeight();
            if(haveLastLine){
                pxHeight--;
            }
            uint_fast32_t stride = cont->getStride();
            uint_fast32_t nextLineOffset = cont->getNextLineOffset();
            char *contBuffer = static_cast<char *>(cont->getBuffer()) + cont->getStartOffset();
            D3DLOCKED_RECT outRect;
            _pixelTexture->LockRect(0, &outRect, &lockRect, 0);
            char *textBuffer = static_cast<char *>(outRect.pBits);
            if(firstCoords.top > lastCoords.top){
                memcpy(textBuffer, contBuffer, cont->getLastStride());
                textBuffer += outRect.Pitch;
                contBuffer += nextLineOffset;
            }
            for (uint_fast32_t y = 0; y < pxHeight; ++y) {
                memcpy(textBuffer, contBuffer, stride);
                textBuffer += outRect.Pitch;
                contBuffer += nextLineOffset;
            }
            if(firstCoords.bottom < lastCoords.bottom){
                memcpy(textBuffer, contBuffer, cont->getLastStride());
            }
            _pixelTexture->UnlockRect(0);
            TexturedVertex *texVertMem = static_cast<TexturedVertex *>(_vertMem);
            uint_fast32_t numVertices = 4;
            if(_height != 0) {
                texVertMem = genTexQuad(
                        texVertMem,
                        firstCoords.left, firstCoords.top, firstCoords.right, firstCoords.bottom,
                        _pixelTextureWidth, _pixelTextureHeight
                );
            }
            if(haveLastLine) {
                numVertices += 4;
                genTexQuad(
                        texVertMem,
                        lastCoords.left, lastCoords.top, lastCoords.right, lastCoords.bottom,
                        _pixelTextureWidth, _pixelTextureHeight
                );
            }
            void* voidPointer;
            _vertBuffer->Lock(0, numVertices * sizeof(TexturedVertex), &voidPointer, D3DLOCK_DISCARD);
            memcpy(voidPointer, _vertMem, numVertices * sizeof(TexturedVertex));
            _vertBuffer->Unlock();
            _device->SetRenderTarget(0, _backBuffer);
            _device->SetStreamSource(0, _vertBuffer, 0, sizeof(TexturedVertex));
            _device->SetTexture(0, _pixelTexture);
            _device->SetFVF(TEXTURED_VERTEX_FVF);
            _device->BeginScene();
            _device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, TRIANGLES_IN_QUAD);
            if(haveLastLine){
                _device->DrawPrimitive(D3DPT_TRIANGLESTRIP, VERTICES_IN_QUAD, TRIANGLES_IN_QUAD);
            }
            _device->EndScene();
            delete cont;
            _device->SetTexture(0, NULL);
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
                    uint_fast32_t curNumVertices =
                            (isFirst) ?
                            VERTICES_IN_QUAD :
                            VERTICES_IN_QUAD * 2 - VERTICES_TRIANGLES_DIFF
                    ;
                    if((totalNumVertices + curNumVertices) >= VERTEX_BUFFER_SIZE){
                        break;
                    }
                    RectVertex *rectVectMem = static_cast<RectVertex *>(_vertMem) + totalNumVertices;
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
                _vertBuffer->Lock(0, totalNumVertices * sizeof(RectVertex), &voidPointer, D3DLOCK_DISCARD);
                memcpy(voidPointer, _vertMem, totalNumVertices * sizeof(RectVertex));
                _vertBuffer->Unlock();
                _device->SetRenderTarget(0, _backBuffer);
                _device->SetStreamSource(0, _vertBuffer, 0, sizeof(RectVertex));
                _device->SetFVF(RECT_VERTEX_FVF);
                _device->BeginScene();
                _device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, totalNumVertices - VERTICES_TRIANGLES_DIFF);
                _device->EndScene();
            }
        }
    }

    DX9Renderer::RectVertex *
    DX9Renderer::genDegenerate(
            DX9Renderer::RectVertex *vertices,
            int_fast32_t startX, int_fast32_t startY,
            int_fast32_t endX, int_fast32_t endY
    ) {

        (*vertices) = VertexCreator::create<RectVertex>(
                static_cast<float>(startX) - 0.5f,
                static_cast<float>(startY) - 0.5f,
                0.0f,
                1.0f,
                D3DCOLOR_ARGB(0, 0, 0, 0)
        );
        vertices++;
        *vertices = VertexCreator::create<RectVertex>(
                static_cast<float>(endX) - 0.5f,
                static_cast<float>(endY) - 0.5f,
                0.0f,
                1.0f,
                D3DCOLOR_ARGB(0, 0, 0, 0)
        );
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
        *vertices = VertexCreator::create<RectVertex>(
                static_cast<float>(startX) - 0.5f,
                static_cast<float>(startY) - 0.5f,
                0.0f,
                1.0f,
                swap_color(color)
        );
        vertices++;
        *vertices = VertexCreator::create<RectVertex>(
                static_cast<float>(endX) - 0.5f,
                static_cast<float>(startY) - 0.5f,
                0.0f,
                1.0f,
                swap_color(color)
        );
        vertices++;
        *vertices = VertexCreator::create<RectVertex>(
                static_cast<float>(startX) - 0.5f,
                static_cast<float>(endY) - 0.5f,
                0.0f,
                1.0f,
                swap_color(color)
        );
        vertices++;
        *vertices = VertexCreator::create<RectVertex>(
                static_cast<float>(endX) - 0.5f,
                static_cast<float>(endY) - 0.5f,
                0.0f,
                1.0f,
                swap_color(color)
        );
        vertices++;
        return vertices;
    }

    DX9Renderer::TexturedVertex *
    DX9Renderer::genTexQuad(
            DX9Renderer::TexturedVertex *vertices,
            int_fast32_t startX, int_fast32_t startY,
            int_fast32_t endX, int_fast32_t endY,
            uint_fast32_t maxX, uint_fast32_t maxY
    ) {
        *vertices = VertexCreator::create<TexturedVertex>(
                startX - 0.5f, startY - 0.5f, 0.0f, 1.0f,
                static_cast<float>(startX) / maxX, static_cast<float>(startY) / maxY
        );
        vertices++;
        *vertices = VertexCreator::create<TexturedVertex>(
                endX - 0.5f, startY - 0.5f, 0.0f, 1.0f,
                static_cast<float>(endX) / maxX, static_cast<float>(startY) / maxY
        );
        vertices++;
        *vertices = VertexCreator::create<TexturedVertex>(
                startX - 0.5f, endY - 0.5f, 0.0f, 1.0f,
                static_cast<float>(startX) / maxX, static_cast<float>(endY) / maxY
        );
        vertices++;
        *vertices = VertexCreator::create<TexturedVertex>(
                endX - 0.5f, endY - 0.5f, 0.0f, 1.0f,
                static_cast<float>(endX) / maxX, static_cast<float>(endY) / maxY
        );
        vertices++;
        return vertices;
    }

    PixelContainerFactory *DX9Renderer::getPixContFactory() {
        return _pixContFactory;
    }
}
