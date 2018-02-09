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
    }
}
