#pragma once

#include <IRenderer.h>
#include <d3d9.h>
#include <dx9/Common.h>
#include <util.h>
#include <vector>
#include "IPatternTexturesHelper.h"
#include "PrimitiveCreator.h"

namespace directgraph {
    namespace dx9 {
        class VertexCreator;

        class Renderer : public IRenderer {
        public:
            enum DrawOpType{
                CLEAR,
                SET_FILL_PATTERN,
                SET_USER_FILL_PATTERN,
                SET_BG_COLOR,
                ITEMS,
                SET_PIXEL_TEXTURE
            };
            enum DrawDataType{
                RECT_VERTEX,
                TEXTURED_RECT_VERTEX,
                TEXTURED_VERTEX
            };
            struct DrawOp{
                DrawOpType type;
                union {
                    uint8_t fillPattern;
                    uint32_t bgColor;
                    char *userFillPattern;
                    struct Items{
                        uint32_t offset;
                        uint32_t numItems;
                        DrawDataType type;
                    } items;
                    IPixelContainer *pixelContainer;
                } data;
            };
            struct DevDrawState{
                uint_fast8_t fillPattern;
                uint_fast32_t bgColor;
                char *userFillPattern;
            };

            struct GenDataVars{
                uint_fast32_t fillColor;
                uint_fast32_t bgColor;
                uint_fast8_t fillStyle;
            };
        private:
            const static int REGISTER_SIZE = 4;
            const static int TRIANGLES_IN_QUAD = 2;
            const static int VERTICES_IN_QUAD = 4;
            const static int VERTICES_TRIANGLES_DIFF = 2;
            const static int VERTEX_BUFFER_SIZE = 262144;

            GenDataVars _curGenDataVars;
            DevDrawState _curState, _lastState;
            typedef std::vector<DrawOp> DrawOpVector;
            DrawOpVector _drawOps;
            typedef std::vector<char*> CharPVector;
            CharPVector _patterns;

            IDirect3D9 *_d3d;
            IDirect3DDevice9 *_device;
            IDirect3DSwapChain9 *_swapChain;
            IDirect3DSurface9 *_backBuffer;
            Common *_common;
            DPIHelper *_helper;
            PrimitiveCreator _primCreator;
            HWND _hwnd;
            float _width, _height;

            const static DWORD RECT_VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

            const static DWORD TEXTURED_VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

            const static DWORD TEXTURED_RECT_VERTEX_FVF = D3DFVF_XYZRHW |  D3DFVF_DIFFUSE | D3DFVF_TEX1;

            IDirect3DVertexBuffer9 *_vertBuffer;
            void *_vertMem;

            uint_fast32_t _pixelTextureWidth;
            uint_fast32_t _pixelTextureHeight;
            IDirect3DTexture9 *_pixelTexture;

            IPatternTexturesHelper *_patTextHelper;

            PixelContainerFactory *_pixContFactory;

            void createDeviceRes();

            bool isColorVertexFill(uint_fast8_t patt);

        public:
            Renderer(Common *common, DPIHelper *helper, float width, float height);

            virtual ~Renderer();

            virtual void setWindow(HWND hwnd);

            virtual void repaint();

            virtual void draw(IQueueReader *reader, CommonProps *props);

            virtual PixelContainerFactory *getPixContFactory();
        };
    }
}
