#pragma once

#include <IRenderer.h>
#include <d3d9.h>
#include <dx9/Common.h>
#include <util.h>
#include <vector>
#include "IPatternTexturesHelper.h"
#include "BufferPreparer.h"
#include "ShaderManager.h"

namespace directgraph {
    namespace dx9 {
        class VertexCreator;

        class Renderer : public IRenderer {
        private:
            const static int REGISTER_SIZE = 4;
            const static int VERTEX_BUFFER_SIZE = 1024 * 1024;

            IDirect3D9 *_d3d;
            IDirect3DDevice9 *_device;
            IDirect3DSwapChain9 *_swapChain;
            IDirect3DSurface9 *_backBuffer;
            Common *_common;
            BufferPreparer *_bufPreparer;
            ShaderManager *_shaderMan;
            BufferPreparerParams *_bufPrepParams;
            CommonProps _props;
            HWND _hwnd;
            uint32_t _width, _height;

            const static DWORD COLOR_VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

            const static DWORD TEXTURED_VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

            const static DWORD TEXTURED_COLOR_VERTEX_FVF = D3DFVF_XYZRHW |  D3DFVF_DIFFUSE | D3DFVF_TEX1;

            IDirect3DVertexBuffer9 *_vertBuffer;

            IDirect3DTexture9 *_pixelTexture;

            IDirect3DSurface9 *_depthStencil;

            IPatternTexturesHelper *_patTextHelper, *_transpPatTextHelper;

            PixelContainerCreator *_pixContFactory;

            void createDeviceRes();

            void copyToVBuffer();

            void doRender();

            void restoreDevice();

        public:
            Renderer(Common *common, uint32_t width, uint32_t height, const CommonProps &props);

            void prepare(IQueueReader *reader);

            virtual ~Renderer();

            virtual void setWindow(HWND hwnd);

            virtual void repaint();

            virtual void draw(IQueueReader *reader);

            virtual PixelContainerCreator *getPixContFactory();
        };
    }
}
