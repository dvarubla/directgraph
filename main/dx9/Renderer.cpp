#include <resource.h>
#include <Queue.h>
#include "Renderer.h"
#include <directgraph/directgraph_constants.h>
#include <math.h>
#include <main/QueueItem.h>
#include "Exception.h"
#include "PatternTexturesHelper.h"
#include "BufferPreparer.h"
#include <algorithm>

#undef max
#undef min

namespace directgraph {
    namespace dx9 {
        Renderer::Renderer(Common *common, DPIHelper *helper, float width, float height, const CommonProps &props)
                : _swapChain(NULL), _vertBuffer(NULL), _pixelTexture(NULL), _patTextHelper(NULL), _bufPreparer(NULL), 
                  _shaderMan(NULL), _bufPrepParams(NULL), _props(props)
        {
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

            uint_fast32_t pixelTextureWidth = static_cast<uint_fast32_t>(1 << (uint_fast32_t) ceil(log2<double>(pxWidth)));
            uint_fast32_t pixelTextureHeight = static_cast<uint_fast32_t>(1 << (uint_fast32_t) ceil(log2<double>(pxHeight)));

            ColorFormat::Format pxFormat = _common->getFeatures()->getImageTexFormat();

            D3DFORMAT pxTexFormat;
            switch(pxFormat){
                case ColorFormat::A8R8G8B8: pxTexFormat = D3DFMT_A8R8G8B8; break;
                case ColorFormat::AR5G5B5: pxTexFormat = D3DFMT_A1R5G5B5; break;
                default:
                    THROW_EXC_CODE(WException, UNREACHABLE_CODE, L"Unknown format");
            }

            if (_device->CreateTexture(
                    pixelTextureWidth, pixelTextureHeight, 1, 0,
                    pxTexFormat, D3DPOOL_MANAGED, &_pixelTexture, NULL
            ) != D3D_OK) {
                THROW_EXC_CODE(Exception, DX9_CANT_CREATE_TEXTURE, std::wstring(L"Can't create texture"));
            };

            _pixContFactory = new PixelContainerCreator(pxWidth, pxHeight, pxFormat);

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

            _device->CreateDepthStencilSurface(
                    pxWidth, pxHeight, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &_depthStencil, NULL
            );

            _shaderMan = new ShaderManager(
                    _common->getFeatures(),
                    _device
            );

            _bufPrepParams = new BufferPreparerParams(
                    _shaderMan,
                    !_common->getFeatures()->supportsTexConst(),
                    pxWidth, pxHeight,
                    pixelTextureWidth, pixelTextureHeight,
                    65536
            );

            _bufPreparer = new BufferPreparer(
                    VERTEX_BUFFER_SIZE, _helper, _props, _bufPrepParams
            );
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
            delete _bufPreparer;
            delete _shaderMan;
            delete _pixContFactory;
            delete _bufPrepParams;
        }

        void Renderer::prepare(IQueueReader *reader) {
             _bufPreparer->prepareBuffer(reader, 0, VERTEX_BUFFER_SIZE);
            if(_bufPreparer->isFull()){
                draw(reader);
            } else {
                reader->endReading(_bufPreparer->getLastOffset());
                _bufPreparer->resetLastOffset();
            }
        }

        void Renderer::draw(IQueueReader *reader) {
            _device->SetRenderTarget(0, _backBuffer);
            _device->SetDepthStencilSurface(_depthStencil);
            _device->BeginScene();
            if(_bufPreparer->isFull()){
                _bufPreparer->genOpsAndMemBlocks();
                copyToVBuffer();
                doRender();
                _bufPreparer->clear();
            }
            while (true) {
                _bufPreparer->prepareBuffer(reader, _bufPreparer->getLastOffset(), VERTEX_BUFFER_SIZE);
                _bufPreparer->genOpsAndMemBlocks();
                copyToVBuffer();
                doRender();
                _bufPreparer->clear();
                if (_bufPreparer->isEmpty()) {
                    break;
                }
            }
            restoreDevice();
            _device->EndScene();
            reader->endReading(_bufPreparer->getLastOffset());
            _bufPreparer->resetLastOffset();
        }

        void Renderer::restoreDevice() {
            _patTextHelper->unsetPattern();
            _device->SetTexture(0, NULL);
            _device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        }

        PixelContainerCreator *Renderer::getPixContFactory() {
            return _pixContFactory;
        }

        void Renderer::doRender() {
            _device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
            _device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
            _device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
            uint_fast32_t offset = 0;
            for(BufferPreparer::DrawOpVector::iterator it = _bufPreparer->drawOpsBegin(); it != _bufPreparer->drawOpsEnd(); ++it){
                switch(it->type){
                    case BufferPreparer::ITEMS: {
                        UINT stride;
                        DWORD fvf;
                        bool setFVF;
                        setFVF = false;
                        switch (it->data.items.type) {
                            case BufferPreparer::COLOR_VERTEX:
                                stride = sizeof(ColorVertex);
                                fvf = RECT_VERTEX_FVF;
                                setFVF = true;
                                break;
                            case BufferPreparer::TEXTURED_COLOR_VERTEX:
                                stride = sizeof(TexturedColorVertex);
                                fvf = TEXTURED_RECT_VERTEX_FVF;
                                setFVF = true;
                                break;
                            case BufferPreparer::TEXTURED_VERTEX:
                                stride = sizeof(TexturedVertex);
                                fvf = TEXTURED_VERTEX_FVF;
                                setFVF = true;
                                break;
                            case BufferPreparer::ELLIPSE_VERTEX:
                                stride = sizeof(TexturedColorVertexNoRHW);
                                setFVF = false;
                                break;
                            case BufferPreparer::COLOR2_VERTEX:
                                stride = sizeof(Color2Vertex);
                                setFVF = false;
                                break;
                            case BufferPreparer::TEXTURED_ELLIPSE_VERTEX:
                                stride = sizeof(TexturedColor2Vertex);
                                setFVF = false;
                                break;
                        }
                        _device->SetStreamSource(
                                0, _vertBuffer,
                                offset,
                                stride
                        );
                        if (setFVF) {
                            _shaderMan->removeShaders();
                            _device->SetFVF(fvf);
                        } else {
                            switch(it->data.items.type){
                                case BufferPreparer::COLOR2_VERTEX:
                                    _shaderMan->setTexturedBar();
                                    break;
                                case BufferPreparer::ELLIPSE_VERTEX:
                                    _shaderMan->setEllipse();
                                    break;
                                case BufferPreparer::TEXTURED_ELLIPSE_VERTEX:
                                    _shaderMan->setTexturedEllipse();
                                    break;
                                default: break;
                            }
                        }
                        _device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, it->data.items.numItems);
                        offset += it->data.items.size;
                    }
                        break;
                    case BufferPreparer::SET_FILL_PATTERN: {
                        _patTextHelper->setFillPattern(it->data.fillPattern);
                    }
                        break;
                    case BufferPreparer::SET_FILL_PATTERN_COLOR: {
                        _patTextHelper->setFillPatternBgColor(
                                it->data.fillPatternColor.fillPattern,
                                it->data.fillPatternColor.bgColor
                        );
                    }
                        break;
                    case BufferPreparer::SET_USER_FILL_PATTERN: {
                        _patTextHelper->setUserFillPattern(it->data.userFillPattern);
                    }
                        break;
                    case BufferPreparer::SET_TEX_BG_COLOR: {
                        _patTextHelper->setBgColor(it->data.bgColor);
                    }
                        break;
                    case BufferPreparer::SET_PIXEL_TEXTURE: {
                        IPixelContainer *cont = it->data.pixelContainer;
                        Rectangle coords = cont->getCoords();
                        uint_fast32_t height = coords.bottom - coords.top;
                        RECT lockRect = {
                                static_cast<LONG>(coords.left),
                                static_cast<LONG>(coords.top),
                                static_cast<LONG>(coords.right),
                                static_cast<LONG>(coords.bottom)
                        };
                        char *contBuffer = static_cast<char *>(cont->getBuffer()) + cont->getStartOffset();
                        D3DLOCKED_RECT outRect;
                        _pixelTexture->LockRect(0, &outRect, &lockRect, 0);
                        char *textBuffer = static_cast<char *>(outRect.pBits);
                        for (uint_fast32_t y = 0; y < height; ++y) {
                            memcpy(textBuffer, contBuffer, cont->getStride());
                            textBuffer += outRect.Pitch;
                            contBuffer += cont->getNextLineOffset();
                        }
                        _pixelTexture->UnlockRect(0);
                        delete cont;
                        _device->SetTexture(0, _pixelTexture);
                        _device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                        _device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                        _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
                        _device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
                    }
                        break;
                    case BufferPreparer::REMOVE_PATTERN_TEXTURE:
                        _patTextHelper->unsetPattern();
                        break;
                    case BufferPreparer::REMOVE_PIXEL_TEXTURE:
                        _device->SetTexture(0, NULL);
                        _device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
                        break;
                    case BufferPreparer::START_TRANSPARENT_DATA:
                        _device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
                        break;
                }
            }
        }

        void Renderer::copyToVBuffer() {
            void *voidPointer;
            uint8_t *curPointer;
            _vertBuffer->Lock(
                    0, _bufPreparer->getUsedSize(),
                    &voidPointer, D3DLOCK_DISCARD
            );
            curPointer = static_cast<uint8_t *>(voidPointer);
            for(
                    BufferPreparer::MemBlockVector::iterator it = _bufPreparer->memBlocksBegin();
                    it != _bufPreparer->memBlocksEnd();
                    ++it
            ) {
                std::copy(it->mem, it->mem + it->size, curPointer);
                curPointer += it->size;
            }
            _vertBuffer->Unlock();
        }
    }
}
