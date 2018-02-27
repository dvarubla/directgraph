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
        EllipseVertex
        VertexCreator::create<EllipseVertex>(float x, float y, float z, float rhw, DWORD color) {
            EllipseVertex v = {x, y, z, rhw, color};
            return v;
        }

        template<>
        TexturedVertex
        VertexCreator::create<TexturedVertex>(float x, float y, float z, float rhw, float tu, float tv) {
            TexturedVertex v = {x, y, z, rhw, tu, tv};
            return v;
        }

        template<>
        TexturedRectVertex VertexCreator::create<TexturedRectVertex>(
                float x, float y, float z, float rhw, DWORD color, float tu, float tv
        ) {
            TexturedRectVertex v = {x, y, z, rhw, color, tu, tv};
            return v;
        }
    }
}