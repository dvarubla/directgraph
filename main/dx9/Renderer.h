#pragma once

#include <IRenderer.h>
#include <d3d9.h>
#include <dx9/Common.h>
#include <util.h>
#include <vector>
#include "IPatternTexturesHelper.h"
#include "PrimitiveCreator.h"
#include "BufferPreparer.h"

namespace directgraph {
    namespace dx9 {
        class VertexCreator;

        class Renderer : public IRenderer {
        private:
            const static int REGISTER_SIZE = 4;
            const static int VERTEX_BUFFER_SIZE = 262144;

            BufferPreparer::DevDrawState _curState;

            IDirect3D9 *_d3d;
            IDirect3DDevice9 *_device;
            IDirect3DSwapChain9 *_swapChain;
            IDirect3DSurface9 *_backBuffer;
            Common *_common;
            DPIHelper *_helper;
            BufferPreparer *_bufPreparer;
            BufferPreparer::GenDataVars _initialVars;
            HWND _hwnd;
            float _width, _height;

            const static DWORD RECT_VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

            const static DWORD TEXTURED_VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

            const static DWORD TEXTURED_RECT_VERTEX_FVF = D3DFVF_XYZRHW |  D3DFVF_DIFFUSE | D3DFVF_TEX1;

            IDirect3DVertexBuffer9 *_vertBuffer;

            IDirect3DTexture9 *_pixelTexture;

            IPatternTexturesHelper *_patTextHelper;

            PixelContainerFactory *_pixContFactory;

            void createDeviceRes();

        public:
            Renderer(Common *common, DPIHelper *helper, float width, float height, const CommonProps &props);

            virtual ~Renderer();

            virtual void setWindow(HWND hwnd);

            virtual void repaint();

            virtual void draw(IQueueReader *reader);

            virtual PixelContainerFactory *getPixContFactory();
        };
    }
}
