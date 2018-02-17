#include <main/util.h>
#include <d3d9.h>
#include "PrimitiveCreator.h"

namespace directgraph{
    namespace dx9{
        template<>
        PrimitiveCreator::RectVertex
        VertexCreator::create<PrimitiveCreator::RectVertex>(float x, float y, float z, float rhw, DWORD color) {
            PrimitiveCreator::RectVertex v = {x, y, z, rhw, color};
            return v;
        }

        template<>
        PrimitiveCreator::EllipseVertex
        VertexCreator::create<PrimitiveCreator::EllipseVertex>(float x, float y, float z, float rhw, DWORD color) {
            PrimitiveCreator::EllipseVertex v = {x, y, z, rhw, color};
            return v;
        }
        
        template<>
        PrimitiveCreator::TexturedVertex
        VertexCreator::create<PrimitiveCreator::TexturedVertex>(float x, float y, float z, float rhw, float tu, float tv) {
            PrimitiveCreator::TexturedVertex v = {x, y, z, rhw, tu, tv};
            return v;
        }

        template<>
        PrimitiveCreator::TexturedRectVertex VertexCreator::create(
                float x, float y, float z, float rhw, DWORD color, float tu, float tv
        ) {
            PrimitiveCreator::TexturedRectVertex v = {x, y, z, rhw, color, tu, tv};
            return v;
        }

        PrimitiveCreator::RectVertex *
        PrimitiveCreator::genDegenerate(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY
        ) {
            RectVertex *vertices = static_cast<RectVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<RectVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            *vertices = VertexCreator::create<RectVertex>(
                    static_cast<float>(endX) - 0.5f,
                    static_cast<float>(endY) - 0.5f,
                    0.0f,
                    1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            return vertices;
        }

        PrimitiveCreator::TexturedVertex *
        PrimitiveCreator::genTexDegenerate(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY) {
            TexturedVertex *vertices = static_cast<TexturedVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedVertex>(
                    static_cast<float>(endX) - 0.5f,
                    static_cast<float>(endY) - 0.5f,
                    0.0f,
                    1.0f,
                    0.0f,
                    0.0f
            );
            vertices++;
            return vertices;
        }
        
        PrimitiveCreator::TexturedRectVertex *
        PrimitiveCreator::genFillDegenerate(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY
        ) {
            TexturedRectVertex *vertices = static_cast<TexturedRectVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedRectVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedRectVertex>(
                    static_cast<float>(endX) - 0.5f,
                    static_cast<float>(endY) - 0.5f,
                    0.0f,
                    1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f,
                    0.0f
            );
            vertices++;
            return vertices;
        }

        PrimitiveCreator::RectVertex *
        PrimitiveCreator::genQuad(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY,
                uint_fast32_t color
        ) {
            RectVertex *vertices = static_cast<RectVertex*>(verticesVoid);
            *vertices = VertexCreator::create<RectVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<RectVertex>(
                    static_cast<float>(endX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<RectVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(endY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<RectVertex>(
                    static_cast<float>(endX) - 0.5f,
                    static_cast<float>(endY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color)
            );
            vertices++;
            return vertices;
        }

        PrimitiveCreator::TexturedRectVertex *
        PrimitiveCreator::genFillQuad(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY,
                uint_fast32_t color
        ) {
            TexturedRectVertex *vertices = static_cast<TexturedRectVertex*>(verticesVoid);
            float textureRight = (endX - startX) / 8.0f;
            float textureBottom = (endY - startY) / 8.0f;
            *vertices = VertexCreator::create<TexturedRectVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color),
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedRectVertex>(
                    static_cast<float>(endX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color),
                    textureRight,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedRectVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(endY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color),
                    0.0f,
                    textureBottom
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedRectVertex>(
                    static_cast<float>(endX) - 0.5f,
                    static_cast<float>(endY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color),
                    textureRight,
                    textureBottom
            );
            vertices++;
            return vertices;
        }

        PrimitiveCreator::TexturedVertex *
        PrimitiveCreator::genTexQuad(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY,
                uint_fast32_t maxX, uint_fast32_t maxY
        ) {
            TexturedVertex *vertices = static_cast<TexturedVertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedVertex>(
                    startX - 0.5f, startY - 0.5f, 0.0f, 1.0f,
                    static_cast<float>(startX) / maxX, static_cast<float>(startY) / maxY
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedVertex>(
                    endX - 0.5f, startY - 0.5f, 0.0f, 1.0f,
                    static_cast<float>(endX) / maxX, static_cast<float>(startY) / maxY
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedVertex>(
                    startX - 0.5f, endY - 0.5f, 0.0f, 1.0f,
                    static_cast<float>(startX) / maxX, static_cast<float>(endY) / maxY
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedVertex>(
                    endX - 0.5f, endY - 0.5f, 0.0f, 1.0f,
                    static_cast<float>(endX) / maxX, static_cast<float>(endY) / maxY
            );
            vertices++;
            return vertices;
        }

        PrimitiveCreator::PrimitiveCreator() {
        }

        PrimitiveCreator::EllipseVertex *
        PrimitiveCreator::genEllipseDegenerate(
                void *verticesVoid, int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY,
                uint_fast32_t width, uint_fast32_t height
        ) {
            EllipseVertex *vertices = static_cast<EllipseVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<EllipseVertex>(
                    static_cast<float>(2.0 * (startX - 0.5) / width - 1),
                    static_cast<float>(1 - 2.0 * (startY - 0.5) / height),
                    0.0f, 0.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            *vertices = VertexCreator::create<EllipseVertex>(
                    static_cast<float>(2.0f * (endX - 0.5) / width - 1),
                    static_cast<float>(1 - 2.0f * (endY - 0.5) / height),
                    0.0f, 0.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            return vertices;
        }

        PrimitiveCreator::EllipseVertex *
        PrimitiveCreator::genEllipseQuad(
                void *verticesVoid,
                int_fast32_t centerX, int_fast32_t centerY,
                int_fast32_t radiusX, int_fast32_t radiusY,
                uint_fast32_t width, uint_fast32_t height,
                uint_fast32_t color
        ) {
            float centerXTrans = static_cast<float>(2.0 * (centerX - 0.5)/ width - 1);
            float radiusXTrans = static_cast<float>(1.0 * radiusX / width);
            float centerYTrans = static_cast<float>(1 - 2.0 * (centerY - 0.5) / height);
            float radiusYTrans = static_cast<float>(1.0 * radiusY / height);
            EllipseVertex *vertices = static_cast<EllipseVertex*>(verticesVoid);
            *vertices = VertexCreator::create<EllipseVertex>(
                    centerXTrans, centerYTrans, -radiusXTrans, -radiusYTrans, swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<EllipseVertex>(
                    centerXTrans, centerYTrans, radiusXTrans, -radiusYTrans, swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<EllipseVertex>(
                    centerXTrans, centerYTrans, -radiusXTrans, radiusYTrans, swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<EllipseVertex>(
                    centerXTrans, centerYTrans, radiusXTrans, radiusYTrans, swap_color(color)
            );
            vertices++;
            return vertices;
        }
    }
}
