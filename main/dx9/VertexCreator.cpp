#include "VertexCreator.h"

namespace directgraph{
    namespace dx9{
        template<>
        ColorVertex
        VertexCreator::create<ColorVertex>(float x, float y, float z, float rhw, DWORD color) {
            ColorVertex v = {x, y, z, rhw, color};
            return v;
        }

        template<>
        TexturedVertex
        VertexCreator::create<TexturedVertex>(float x, float y, float z, float rhw, float tu, float tv) {
            TexturedVertex v = {x, y, z, rhw, tu, tv};
            return v;
        }

        template<>
        TexturedColorVertex VertexCreator::create<TexturedColorVertex>(
                float x, float y, float z, float rhw, DWORD color, float tu, float tv
        ) {
            TexturedColorVertex v = {x, y, z, rhw, color, tu, tv};
            return v;
        }

        template<>
        Color2VertexNoRHW VertexCreator::create<Color2VertexNoRHW>(
                float x, float y, float z, DWORD color1, DWORD color2, float tu, float tv
        ) {
            Color2VertexNoRHW v = {x, y, z, color1, color2, tu, tv};
            return v;
        }

        template<>
        Color2Vertex VertexCreator::create<Color2Vertex>(
                float x, float y, float z, float rhw, DWORD color1, DWORD color2, float tu, float tv
        ) {
            Color2Vertex v = {x, y, z, rhw, color1, color2, tu, tv};
            return v;
        }

        template<>
        Color3Vertex VertexCreator::create<Color3Vertex>(
                float x, float y, float z, float rhw, DWORD color1, DWORD color2, DWORD color3, float tu, float tv
        ) {
            Color3Vertex v = {x, y, z, rhw, color1, color2, color3, tu, tv};
            return v;
        }

        template<>
        TexturedColorVertexNoRHW VertexCreator::create<TexturedColorVertexNoRHW>(
                float x, float y, float z, DWORD color, float tu, float tv
        ) {
            TexturedColorVertexNoRHW v = {x, y, z, color, tu, tv};
            return v;
        }
    }
}