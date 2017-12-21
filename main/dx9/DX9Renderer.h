#ifndef DIRECTGRAPH_DX9RENDERER_H
#define DIRECTGRAPH_DX9RENDERER_H

#include <IRenderer.h>
#include <d3d9.h>
#include <dx9/DX9Common.h>

namespace directgraph {
    class DX9Renderer : public IRenderer {
    private:
        const static int REGISTER_SIZE = 4;
        const static int VERTEX_BUFFER_SIZE = 256;
        IDirect3D9* _d3d;
        IDirect3DDevice9* _device;
        IDirect3DSwapChain9 *_swapChain;
        IDirect3DSurface9 *_backBuffer;
        DX9Common *_common;
        DPIHelper *_helper;
        HWND _hwnd;
        float _width, _height;

        struct RectVertex{
            float x, y, z, rhw;
            DWORD color;
        };
        IDirect3DVertexBuffer9 *_rectVertBuffer;
        RectVertex *_rectVertMem;

        void createDeviceRes();

    public:
        DX9Renderer(DX9Common *common, DPIHelper *helper, float width, float height);
        virtual ~DX9Renderer();

        virtual void setWindow(HWND hwnd);

        virtual uint_fast32_t swapColor(uint_fast32_t color);

        virtual void repaint();

        virtual void draw(QueueReader *reader, CommonProps *props);
    };
}

#endif //DIRECTGRAPH_DX9RENDERER_H
