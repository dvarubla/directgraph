#include <resource.h>
#include "../../resources/common_res.h"
#include <cstdio>
#include <iostream>
#include "DX9Renderer.h"

namespace directgraph {
    DX9Renderer::DX9Renderer(DX9Common *common, DPIHelper *helper, float width, float height) {
        _helper = helper;
        _width = width;
        _height = height;
        _color[0] = 0;
        _color[1] = 0;
        _color[2] = 0;
        _common = common;
    }

    void DX9Renderer::setWindow(HWND hwnd) {
        _hwnd = hwnd;
        createDeviceRes();
        clear();
    }

    void DX9Renderer::setcolor(uint_fast32_t color) {
        _color[0] = static_cast<float>(color&0xFF)/0xFF;
        _color[1] = static_cast<float>((color>>8)&0xFF)/0xFF;
        _color[2] = static_cast<float>((color>>16)&0xFF)/0xFF;
    }

    void DX9Renderer::bar(float left, float top, float right, float bottom) {
        _device->SetRenderTarget(0, _backBuffer);
        _device->SetStreamSource(0, _rectVertBuffer, 0, sizeof(RectVertex));
        _device->SetVertexShader(_rectMoveShader);
        _device->SetVertexDeclaration(_rectVertexDecl);
        float shaderData[3*REGISTER_SIZE] = {
                static_cast<float>(_helper->toPixelsX(left)),
                static_cast<float>(_helper->toPixelsY(top)),
                static_cast<float>(_helper->toPixelsX(right)),
                static_cast<float>(_helper->toPixelsY(bottom)),
                _color[0],_color[1],_color[2],0,
                static_cast<float>(_helper->toPixelsX(_width)),
                static_cast<float>(_helper->toPixelsY(_height)),
                0, 0
        };
        _device->SetVertexShaderConstantF(0, shaderData, 3);
        _device->BeginScene();
        const UINT NUM_TRIANGLES = 2;
        _device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_TRIANGLES);
        _device->EndScene();
    }

    void DX9Renderer::clear() {
        _device->SetRenderTarget(0, _backBuffer);
        _device->Clear(
                0, NULL, D3DCLEAR_TARGET,
                D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0),
                1.0f,
                0
        );
    }

    void DX9Renderer::repaint() {
        _device->SetRenderTarget(0, _backBuffer);
        _swapChain->Present(NULL, NULL, NULL, NULL, 0);
    }

    void DX9Renderer::createDeviceRes() {
        _swapChain = _common->createSwapChain(_hwnd, _helper->toPixelsX(_width), _helper->toPixelsX(_height));
        _swapChain->GetDevice(&_device);
        _swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &_backBuffer);

        uint_fast32_t size;
        void *buffer;
        load_resource(RECT_MOVE_V2_0, IDR_VERTEX_SHADER, size, buffer);
        _device->CreateVertexShader(static_cast<DWORD*>(buffer), &_rectMoveShader);

        const int NUM_RECT_VERTICES = 4;
        RectVertex vertices[NUM_RECT_VERTICES] = {-0.25f, -1.0f, 0.25f,  1.0f};

        _device->CreateVertexBuffer(NUM_RECT_VERTICES * sizeof(RectVertex),
                                          D3DDECLMETHOD_DEFAULT,
                                          0,
                                          D3DPOOL_DEFAULT,
                                          &_rectVertBuffer, NULL);

        void* void_pointer;
        _rectVertBuffer->Lock(0, 0, &void_pointer, 0);
        memcpy(void_pointer, vertices, NUM_RECT_VERTICES * sizeof(RectVertex));
        _rectVertBuffer->Unlock();

        D3DVERTEXELEMENT9 decl[] = {
                { 0, 0 , D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
                D3DDECL_END()
        };
        _device->CreateVertexDeclaration(decl, &_rectVertexDecl);
    }

    DX9Renderer::~DX9Renderer() {
        _rectVertexDecl->Release();
        _rectVertBuffer->Release();
        _rectMoveShader->Release();
        _common->deleteSwapChain(_swapChain);
        delete _helper;
    }
}
