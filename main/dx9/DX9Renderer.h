#pragma once

#include <IRenderer.h>
#include <d3d9.h>
#include <dx9/DX9Common.h>
#include <util.h>

namespace directgraph {
    class DX9Renderer : public IRenderer {
    private:
        const static int REGISTER_SIZE = 4;
        const static int TRIANGLES_IN_QUAD = 2;
        const static int VERTICES_IN_QUAD = 4;
        const static int VERTICES_TRIANGLES_DIFF = 2;
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
        const DWORD RECT_VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
        struct TexturedVertex{
            float x, y, z, rhw;
            float tu, tv;
        };
        const DWORD TEXTURED_VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
        IDirect3DVertexBuffer9 *_vertBuffer;
        void *_vertMem;

        uint_fast32_t _pixelTextureWidth;
        uint_fast32_t _pixelTextureHeight;
        IDirect3DTexture9 *_pixelTexture;

        PixelContainerFactory *_pixContFactory;

        void createDeviceRes();

        RectVertex * genDegenerate(
                RectVertex *vertices,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY
        );

        RectVertex * genQuad(
                RectVertex *vertices,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY,
                uint_fast32_t color
        );

        TexturedVertex * genTexQuad(
                TexturedVertex *vertices,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY,
                uint_fast32_t maxX, uint_fast32_t maxY
        );
    public:
        DX9Renderer(DX9Common *common, DPIHelper *helper, float width, float height);
        virtual ~DX9Renderer();

        virtual void setWindow(HWND hwnd);

        virtual void repaint();

        virtual void draw(IQueueReader *reader, CommonProps *props);

        virtual PixelContainerFactory* getPixContFactory();
    };
}
