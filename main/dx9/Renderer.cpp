#include <resource.h>
#include <Queue.h>
#include "Renderer.h"
#include <directgraph/directgraph_constants.h>
#include <math.h>
#include <main/QueueItem.h>
#include "Exception.h"
#include "PatternTexturesHelper.h"
#include <algorithm>

#undef max
#undef min

namespace directgraph {
    namespace dx9 {
        template<>
        Renderer::RectVertex
        VertexCreator::create<Renderer::RectVertex>(float x, float y, float z, float rhw, DWORD color) {
            Renderer::RectVertex v = {x, y, z, rhw, color};
            return v;
        }

        template<>
        Renderer::TexturedVertex
        VertexCreator::create<Renderer::TexturedVertex>(float x, float y, float z, float rhw, float tu, float tv) {
            Renderer::TexturedVertex v = {x, y, z, rhw, tu, tv};
            return v;
        }

        template<>
        Renderer::TexturedRectVertex VertexCreator::create(
                float x, float y, float z, float rhw, DWORD color, float tu, float tv
        ) {
            Renderer::TexturedRectVertex v = {x, y, z, rhw, color, tu, tv};
            return v;
        }

        Renderer::Renderer(Common *common, DPIHelper *helper, float width, float height)
                : _swapChain(NULL), _vertBuffer(NULL), _vertMem(NULL), _pixelTexture(NULL), _patTextHelper(NULL) {
            _helper = helper;
            _width = width;
            _height = height;
            _common = common;
        }

        void Renderer::setWindow(HWND hwnd) {
            _hwnd = hwnd;
            createDeviceRes();
        }

        void Renderer::repaint() {
            _common->lock();
            _swapChain->Present(NULL, NULL, NULL, NULL, 0);
            _common->unlock();
        }

        void Renderer::createDeviceRes() {
            uint_fast32_t pxWidth = static_cast<uint_fast32_t>(_helper->toPixelsX(_width));
            uint_fast32_t pxHeight = static_cast<uint_fast32_t>(_helper->toPixelsY(_height));
            _swapChain = _common->createSwapChain(_hwnd, pxWidth, pxHeight);
            _swapChain->GetDevice(&_device);
            _swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &_backBuffer);

            _pixelTextureWidth = static_cast<uint_fast32_t>(1 << (uint_fast32_t) ceil(log2<double>(pxWidth)));
            _pixelTextureHeight = static_cast<uint_fast32_t>(1 << (uint_fast32_t) ceil(log2<double>(pxHeight)));

            if (_device->CreateTexture(
                    _pixelTextureWidth, _pixelTextureHeight, 1, 0,
                    _common->getFormat(), D3DPOOL_MANAGED, &_pixelTexture, NULL
            ) != D3D_OK) {
                THROW_EXC_CODE(Exception, DX9_CANT_CREATE_TEXTURE, std::wstring(L"Can't create texture"));
            };

            ColorFormat::Format format;

            switch (_common->getFormat()) {
                case D3DFMT_R8G8B8:
                case D3DFMT_X8R8G8B8:
                case D3DFMT_A8R8G8B8:
                    format = ColorFormat::R8G8B8;
                    break;
                case D3DFMT_R5G6B5:
                    format = ColorFormat::R5G6B5;
                    break;
                default:
                    THROW_EXC_CODE(
                            Exception,
                            DX9_UNSUPPORTED_DISPLAY_FORMAT,
                            std::wstring(L"Unsupported display format")
                    );
            }
            _pixContFactory = new PixelContainerFactory(pxWidth, pxHeight, format);

            if (_device->CreateVertexBuffer(VERTEX_BUFFER_SIZE,
                                            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
                                            0,
                                            D3DPOOL_DEFAULT,
                                            &_vertBuffer, NULL) != D3D_OK
                    ) {
                THROW_EXC_CODE(
                        Exception,
                        DX9_CANT_CREATE_VBUFFER,
                        std::wstring(L"Can't create vertex buffer")
                );
            }
            _vertMem = malloc(VERTEX_BUFFER_SIZE);
            if (_vertMem == NULL) {
                THROW_EXC_CODE(
                        Exception,
                        CANT_ALLOC,
                        std::wstring(L"Can't allocate memory for vertices")
                );
            }
            switch(_common->getFeatures()->getPatternTexFormat()){
                case ColorFormat::A8:
                    _patTextHelper = new PatternTexturesHelper<ColorFormat::A8>(
                            _device, D3DFMT_A8,
                            _common->getFeatures()->supportsTexConst()
                    );
                    break;
                case ColorFormat::AR5G5B5:
                    _patTextHelper = new PatternTexturesHelper<ColorFormat::AR5G5B5>(
                            _device, D3DFMT_A1R5G5B5,
                            _common->getFeatures()->supportsTexConst()
                    );
                    break;
                default:
                    THROW_EXC_CODE(WException, UNREACHABLE_CODE, L"Unknown format");
            }
        }

        Renderer::~Renderer() {
            if (_pixelTexture != NULL) {
                _pixelTexture->Release();
            }
            if (_vertBuffer != NULL) {
                _vertBuffer->Release();
            }
            delete _patTextHelper;
            _common->deleteSwapChain(_swapChain);
            delete _helper;
            delete _pixContFactory;
            free(_vertMem);
        }

        void Renderer::draw(IQueueReader *reader, CommonProps *props) {
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
                return;
            }
            if (firstItem.type == QueueItem::PIXEL_CONTAINER) {
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
                if (haveLastLine) {
                    pxHeight--;
                }
                uint_fast32_t stride = cont->getStride();
                uint_fast32_t nextLineOffset = cont->getNextLineOffset();
                char *contBuffer = static_cast<char *>(cont->getBuffer()) + cont->getStartOffset();
                D3DLOCKED_RECT outRect;
                _pixelTexture->LockRect(0, &outRect, &lockRect, 0);
                char *textBuffer = static_cast<char *>(outRect.pBits);
                if (firstCoords.top > lastCoords.top) {
                    memcpy(textBuffer, contBuffer, cont->getLastStride());
                    textBuffer += outRect.Pitch;
                    contBuffer += nextLineOffset;
                }
                for (uint_fast32_t y = 0; y < pxHeight; ++y) {
                    memcpy(textBuffer, contBuffer, stride);
                    textBuffer += outRect.Pitch;
                    contBuffer += nextLineOffset;
                }
                if (firstCoords.bottom < lastCoords.bottom) {
                    memcpy(textBuffer, contBuffer, cont->getLastStride());
                }
                _pixelTexture->UnlockRect(0);
                TexturedVertex *texVertMem = static_cast<TexturedVertex *>(_vertMem);
                uint_fast32_t numVertices = 4;
                if (_height != 0) {
                    texVertMem = genTexQuad(
                            texVertMem,
                            firstCoords.left, firstCoords.top, firstCoords.right, firstCoords.bottom,
                            _pixelTextureWidth, _pixelTextureHeight
                    );
                }
                if (haveLastLine) {
                    numVertices += 4;
                    genTexQuad(
                            texVertMem,
                            lastCoords.left, lastCoords.top, lastCoords.right, lastCoords.bottom,
                            _pixelTextureWidth, _pixelTextureHeight
                    );
                }
                void *voidPointer;
                _vertBuffer->Lock(0, numVertices * sizeof(TexturedVertex), &voidPointer, D3DLOCK_DISCARD);
                memcpy(voidPointer, _vertMem, numVertices * sizeof(TexturedVertex));
                _vertBuffer->Unlock();
                _patTextHelper->unsetPattern();

                _device->SetRenderTarget(0, _backBuffer);
                _device->SetStreamSource(0, _vertBuffer, 0, sizeof(TexturedVertex));
                _device->SetTexture(0, _pixelTexture);
                _device->SetFVF(TEXTURED_VERTEX_FVF);
                _device->BeginScene();
                _device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, TRIANGLES_IN_QUAD);
                if (haveLastLine) {
                    _device->DrawPrimitive(D3DPT_TRIANGLESTRIP, VERTICES_IN_QUAD, TRIANGLES_IN_QUAD);
                }
                _device->EndScene();
                delete cont;
                _device->SetTexture(0, NULL);
                return;
            }
            uint_fast32_t size = reader->getSize();
            uint_fast32_t readIndex = 0;
            uint_fast32_t totalNumVertices = 0;
            int_fast32_t prevX = 0, prevY = 0;
            bool isFirst = true;
            uint_fast8_t curFillStyle = SOLID_FILL;
            bool useFillTexture = false;
            void *curVertMem = _vertMem;
            for (readIndex = 0; readIndex < size; readIndex++) {
                QueueItem &item = reader->getAt(readIndex);
                if (
                    (item.type == QueueItem::SINGLE_PIXEL && !useFillTexture) ||
                    item.type == QueueItem::BAR
                ) {
                    if(item.type != QueueItem::SINGLE_PIXEL && curFillStyle != props->fillStyle){
                        if(!isFirst) {
                            break;
                        }
                        curFillStyle = props->fillStyle;
                        useFillTexture = true;
                    }
                    uint_fast32_t curNumVertices =
                            (isFirst) ?
                            VERTICES_IN_QUAD :
                            VERTICES_IN_QUAD * 2 - VERTICES_TRIANGLES_DIFF;
                    uint_fast32_t curUsedSize = ((uint8_t*)curVertMem - (uint8_t*)_vertMem);
                    if(useFillTexture){
                        curUsedSize += curNumVertices * sizeof(TexturedRectVertex);
                    } else {
                        curUsedSize += curNumVertices * sizeof(RectVertex);
                    }
                    if (curUsedSize > VERTEX_BUFFER_SIZE) {
                        break;
                    }
                    if (!isFirst) {
                        if(useFillTexture) {
                            curVertMem = genFillDegenerate(
                                    curVertMem,
                                    prevX, prevY,
                                    ((item.type == QueueItem::SINGLE_PIXEL) ?
                                     static_cast<int_fast32_t>(item.data.singlePixel.x) :
                                     _helper->toPixelsX(item.data.bar.left)),
                                    ((item.type == QueueItem::SINGLE_PIXEL) ?
                                     static_cast<int_fast32_t>(item.data.singlePixel.y) :
                                     _helper->toPixelsY(item.data.bar.top))
                            );
                        } else {
                            curVertMem = genDegenerate(
                                    curVertMem,
                                    prevX, prevY,
                                    ((item.type == QueueItem::SINGLE_PIXEL) ?
                                     static_cast<int_fast32_t>(item.data.singlePixel.x) :
                                     _helper->toPixelsX(item.data.bar.left)),
                                    ((item.type == QueueItem::SINGLE_PIXEL) ?
                                     static_cast<int_fast32_t>(item.data.singlePixel.y) :
                                     _helper->toPixelsY(item.data.bar.top))
                            );
                        }
                    }
                    if (item.type == QueueItem::SINGLE_PIXEL) {
                        curVertMem = genQuad(curVertMem,
                                item.data.singlePixel.x,
                                item.data.singlePixel.y,
                                item.data.singlePixel.x + 1,
                                item.data.singlePixel.y + 1,
                                item.data.singlePixel.color
                        );
                        prevX = item.data.singlePixel.x + 1;
                        prevY = item.data.singlePixel.y + 1;
                    } else {
                        if(useFillTexture) {
                            curVertMem= genFillQuad(curVertMem,
                                    _helper->toPixelsX(item.data.bar.left),
                                    _helper->toPixelsY(item.data.bar.top),
                                    _helper->toPixelsX(item.data.bar.right),
                                    _helper->toPixelsY(item.data.bar.bottom),
                                    props->color
                            );
                        } else {
                            curVertMem = genQuad(curVertMem,
                                    _helper->toPixelsX(item.data.bar.left),
                                    _helper->toPixelsY(item.data.bar.top),
                                    _helper->toPixelsX(item.data.bar.right),
                                    _helper->toPixelsY(item.data.bar.bottom),
                                    props->color
                            );
                        }
                        prevX = _helper->toPixelsX(item.data.bar.right);
                        prevY = _helper->toPixelsY(item.data.bar.bottom);
                    }
                    isFirst = false;
                    totalNumVertices += curNumVertices;
                } else if (item.type == QueueItem::SETFILLSTYLE) {
                    if(!(isFirst || (props->fillStyle == item.data.setfillstyle.fillStyle))){
                        break;
                    }
                    props->color = item.data.setfillstyle.color;
                    props->fillStyle = item.data.setfillstyle.fillStyle;
                } else if (item.type == QueueItem::SETFILLPATTERN) {
                    if(!isFirst){
                        break;
                    }
                    props->color = item.data.setfillpattern.color;
                    props->fillStyle = USER_FILL;
                    delete [] props->userFillPattern;
                    props->userFillPattern = item.data.setfillpattern.fillPattern;
                    _patTextHelper->setUserFillPattern(props->userFillPattern);
                } else if (item.type == QueueItem::BGCOLOR){
                    if(!(
                         isFirst || props->fillStyle == SOLID_FILL ||
                         (props->bgColor == item.data.bgColor)
                    )){
                        break;
                    }
                    props->bgColor = item.data.bgColor;
                } else {
                    break;
                }
            }

            if (readIndex != 0 && totalNumVertices != 0) {
                reader->endReading(readIndex);
                if (useFillTexture) {
                    void *voidPointer;
                    _vertBuffer->Lock(0, totalNumVertices * sizeof(TexturedRectVertex), &voidPointer, D3DLOCK_DISCARD);
                    memcpy(voidPointer, _vertMem, totalNumVertices * sizeof(TexturedRectVertex));
                    _vertBuffer->Unlock();

                    _patTextHelper->setFillPattern(curFillStyle, props->bgColor);
                    _device->SetRenderTarget(0, _backBuffer);
                    _device->SetStreamSource(0, _vertBuffer, 0, sizeof(TexturedRectVertex));
                    _device->SetFVF(TEXTURED_RECT_VERTEX_FVF);
                } else {
                    void *voidPointer;
                    _vertBuffer->Lock(0, totalNumVertices * sizeof(RectVertex), &voidPointer, D3DLOCK_DISCARD);
                    memcpy(voidPointer, _vertMem, totalNumVertices * sizeof(RectVertex));
                    _vertBuffer->Unlock();

                    _device->SetRenderTarget(0, _backBuffer);
                    _device->SetStreamSource(0, _vertBuffer, 0, sizeof(RectVertex));
                    _device->SetFVF(RECT_VERTEX_FVF);
                }
                _device->BeginScene();
                _device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, totalNumVertices - VERTICES_TRIANGLES_DIFF);
                _device->EndScene();
                if (useFillTexture) {
                    _patTextHelper->unsetPattern();
                }
                return;
            }
        }

        Renderer::RectVertex *
        Renderer::genDegenerate(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY
        ) {
            RectVertex *vertices = static_cast<RectVertex*>(verticesVoid);
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

        Renderer::TexturedRectVertex *
        Renderer::genFillDegenerate(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY
        ) {
            TexturedRectVertex *vertices = static_cast<TexturedRectVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedRectVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedRectVertex>(
                    static_cast<float>(endX) - 0.5f,
                    static_cast<float>(endY) - 0.5f,
                    0.0f,
                    1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f,
                    0.0f
            );
            vertices++;
            return vertices;
        }

        Renderer::RectVertex *
        Renderer::genQuad(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY,
                uint_fast32_t color
        ) {
            RectVertex *vertices = static_cast<RectVertex*>(verticesVoid);
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

        Renderer::TexturedRectVertex *
        Renderer::genFillQuad(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY,
                uint_fast32_t color
        ) {
            TexturedRectVertex *vertices = static_cast<TexturedRectVertex*>(verticesVoid);
            float textureRight = (endX - startX) / 8.0f;
            float textureBottom = (endY - startY) / 8.0f;
            *vertices = VertexCreator::create<TexturedRectVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color),
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedRectVertex>(
                    static_cast<float>(endX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color),
                    textureRight,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedRectVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(endY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color),
                    0.0f,
                    textureBottom
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedRectVertex>(
                    static_cast<float>(endX) - 0.5f,
                    static_cast<float>(endY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color),
                    textureRight,
                    textureBottom
            );
            vertices++;
            return vertices;
        }

        Renderer::TexturedVertex *
        Renderer::genTexQuad(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY,
                uint_fast32_t maxX, uint_fast32_t maxY
        ) {
            TexturedVertex *vertices = static_cast<TexturedVertex*>(verticesVoid);
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

        PixelContainerFactory *Renderer::getPixContFactory() {
            return _pixContFactory;
        }
    }
}
