#pragma once

#include <IRenderer.h>
#include <d3d9.h>
#include <dx9/Common.h>
#include <util.h>
#include "IPatternTexturesHelper.h"

namespace directgraph {
    namespace dx9 {
        class VertexCreator;

        class Renderer : public IRenderer {
            friend class VertexCreator;

        private:
            const static int REGISTER_SIZE = 4;
            const static int TRIANGLES_IN_QUAD = 2;
            const static int VERTICES_IN_QUAD = 4;
            const static int VERTICES_TRIANGLES_DIFF = 2;
            const static int VERTEX_BUFFER_SIZE = 262144;
            IDirect3D9 *_d3d;
            IDirect3DDevice9 *_device;
            IDirect3DSwapChain9 *_swapChain;
            IDirect3DSurface9 *_backBuffer;
            Common *_common;
            DPIHelper *_helper;
            HWND _hwnd;
            float _width, _height;

            struct RectVertex {
                float x, y, z, rhw;
                DWORD color;
            };
            const static DWORD RECT_VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
            struct TexturedVertex {
                float x, y, z, rhw;
                float tu, tv;
            };
            const static DWORD TEXTURED_VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
            struct TexturedRectVertex {
                float x, y, z, rhw;
                DWORD color;
                float tu, tv;
            };
            const static DWORD TEXTURED_RECT_VERTEX_FVF = D3DFVF_XYZRHW |  D3DFVF_DIFFUSE | D3DFVF_TEX1;

            IDirect3DVertexBuffer9 *_vertBuffer;
            void *_vertMem;

            uint_fast32_t _pixelTextureWidth;
            uint_fast32_t _pixelTextureHeight;
            IDirect3DTexture9 *_pixelTexture;

            IPatternTexturesHelper *_patTextHelper;

            PixelContainerFactory *_pixContFactory;

            void createDeviceRes();

            RectVertex *genDegenerate(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY
            );

            RectVertex *genQuad(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY,
                    uint_fast32_t color
            );

            TexturedRectVertex *genFillDegenerate(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY
            );

            TexturedRectVertex *genFillQuad(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY,
                    uint_fast32_t color
            );

            TexturedVertex *genTexQuad(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY,
                    uint_fast32_t maxX, uint_fast32_t maxY
            );

            bool isColorVertexFill(uint_fast8_t patt);

        public:
            Renderer(Common *common, DPIHelper *helper, float width, float height);

            virtual ~Renderer();

            virtual void setWindow(HWND hwnd);

            virtual void repaint();

            virtual void draw(IQueueReader *reader, CommonProps *props);

            virtual PixelContainerFactory *getPixContFactory();
        };

        class VertexCreator {
        public:
            template<typename T>
            static T create(float x, float y, float z, float rhw, DWORD color);

            template<typename T>
            static T create(float x, float y, float z, float rhw, float tu, float tv);

            template<typename T>
            static T create(float x, float y, float z, float rhw, DWORD color, float tu, float tv);
        };
    }
}
