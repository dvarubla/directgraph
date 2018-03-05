#include <main/util.h>
#include <d3d9.h>
#include "PrimitiveCreator.h"

namespace directgraph{
    namespace dx9{
        ColorVertex *
        PrimitiveCreator::genDegenerate(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY
        ) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<ColorVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(endX) - 0.5f,
                    static_cast<float>(endY) - 0.5f,
                    0.0f,
                    1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            return vertices;
        }

        TexturedVertex *
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
        
        TexturedColorVertex *
        PrimitiveCreator::genFillDegenerate(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
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

        ColorVertex *
        PrimitiveCreator::genQuad(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY,
                uint_fast32_t color
        ) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(endX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(endY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(endX) - 0.5f,
                    static_cast<float>(endY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color)
            );
            vertices++;
            return vertices;
        }

        Color2Vertex *
        PrimitiveCreator::genFillCol2Degenerate(void *verticesVoid, int_fast32_t startX, int_fast32_t startY,
                                                int_fast32_t endX, int_fast32_t endY,
                                                uint_fast32_t width, uint_fast32_t height) {
            float startXTrans = coordToPositionX(startX, width);
            float endXTrans = coordToPositionX(endX, width);
            float startYTrans = coordToPositionY(startY, height);
            float endYTrans = coordToPositionY(endY, height);
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<Color2Vertex>(
                    startXTrans,
                    startYTrans,
                    0.0f,
                    0.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    endXTrans,
                    endYTrans,
                    0.0f,
                    0.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            return vertices;
        }

        Color2Vertex *
        PrimitiveCreator::genFillCol2Quad(void *verticesVoid, int_fast32_t startX, int_fast32_t startY,
                                          int_fast32_t endX, int_fast32_t endY, uint_fast32_t color1,
                                          uint_fast32_t color2, uint_fast32_t width, uint_fast32_t height) {
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            float textureRight = 1.0f * (endX - startX) / FPATTERN_SIZE;
            float textureBottom = 1.0f * (endY - startY) / FPATTERN_SIZE;
            float startXTrans = coordToPositionX(startX, width);
            float endXTrans = coordToPositionX(endX, width);
            float startYTrans = coordToPositionY(startY, height);
            float endYTrans = coordToPositionY(endY, height);
            *vertices = VertexCreator::create<Color2Vertex>(
                    startXTrans,
                    startYTrans,
                    0.0f,
                    0.0f,
                    static_cast<DWORD>(swap_color(color1)),
                    static_cast<DWORD>(swap_color(color2))
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    endXTrans,
                    startYTrans,
                    textureRight,
                    0.0f,
                    static_cast<DWORD>(swap_color(color1)),
                    static_cast<DWORD>(swap_color(color2))
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    startXTrans,
                    endYTrans,
                    0.0f,
                    textureBottom,
                    static_cast<DWORD>(swap_color(color1)),
                    static_cast<DWORD>(swap_color(color2))
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    endXTrans,
                    endYTrans,
                    textureRight,
                    textureBottom,
                    static_cast<DWORD>(swap_color(color1)),
                    static_cast<DWORD>(swap_color(color2))
            );
            vertices++;
            return vertices;
        }

        TexturedColorVertex *
        PrimitiveCreator::genFillQuad(
                void *verticesVoid,
                int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY,
                uint_fast32_t color
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            float textureRight = 1.0f * (endX - startX) / FPATTERN_SIZE;
            float textureBottom = 1.0f * (endY - startY) / FPATTERN_SIZE;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color),
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endX) - 0.5f,
                    static_cast<float>(startY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color),
                    textureRight,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startX) - 0.5f,
                    static_cast<float>(endY) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color),
                    0.0f,
                    textureBottom
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
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

        TexturedVertex *
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

        ColorVertex *
        PrimitiveCreator::genEllipseDegenerate(
                void *verticesVoid, int_fast32_t startX, int_fast32_t startY,
                int_fast32_t endX, int_fast32_t endY,
                uint_fast32_t width, uint_fast32_t height
        ) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<ColorVertex>(
                    coordToPositionX(startX, width),
                    coordToPositionY(startY, height),
                    0.0f, 0.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    coordToPositionX(endX, width),
                    coordToPositionY(endY, height),
                    0.0f, 0.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            return vertices;
        }

        ColorVertex *
        PrimitiveCreator::genEllipseQuad(
                void *verticesVoid,
                int_fast32_t centerX, int_fast32_t centerY,
                int_fast32_t radiusX, int_fast32_t radiusY,
                uint_fast32_t width, uint_fast32_t height,
                uint_fast32_t color
        ) {
            float centerXTrans = coordToPositionX(centerX, width);
            float radiusXTrans = static_cast<float>(1.0 * radiusX / width);
            float centerYTrans = coordToPositionY(centerY, height);
            float radiusYTrans = static_cast<float>(1.0 * radiusY / height);
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<ColorVertex>(
                    centerXTrans, centerYTrans, -radiusXTrans, -radiusYTrans, swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    centerXTrans, centerYTrans, radiusXTrans, -radiusYTrans, swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    centerXTrans, centerYTrans, -radiusXTrans, radiusYTrans, swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    centerXTrans, centerYTrans, radiusXTrans, radiusYTrans, swap_color(color)
            );
            vertices++;
            return vertices;
        }

        TexturedColor2Vertex *
        PrimitiveCreator::genTexEllipseDegenerate(void *verticesVoid, int_fast32_t startX, int_fast32_t startY,
                                                  int_fast32_t endX, int_fast32_t endY, uint_fast32_t width,
                                                  uint_fast32_t height) {
            TexturedColor2Vertex *vertices = static_cast<TexturedColor2Vertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedColor2Vertex>(
                    coordToPositionX(startX, width),
                    coordToPositionY(startY, height),
                    0.0f, 0.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColor2Vertex>(
                    coordToPositionX(endX, width),
                    coordToPositionY(endY, height),
                    0.0f, 0.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f, 0.0f
            );
            vertices++;
            return vertices;
        }

        TexturedColor2Vertex *
        PrimitiveCreator::genTexEllipseQuad(void *verticesVoid, int_fast32_t centerX, int_fast32_t centerY,
                                            int_fast32_t radiusX, int_fast32_t radiusY,
                                            uint_fast32_t color1, uint_fast32_t color2,
                                            uint_fast32_t width, uint_fast32_t height) {
            float centerXTrans = coordToPositionX(centerX, width);
            float radiusXTrans = static_cast<float>(1.0 * radiusX / width);
            float centerYTrans = coordToPositionY(centerY, height);
            float radiusYTrans = static_cast<float>(1.0 * radiusY / height);
            float textureRight = (2.0f * radiusX) / FPATTERN_SIZE;
            float textureBottom = (2.0f * radiusY) / FPATTERN_SIZE;
            TexturedColor2Vertex *vertices = static_cast<TexturedColor2Vertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColor2Vertex>(
                    centerXTrans, centerYTrans, -radiusXTrans, -radiusYTrans,
                    swap_color(color1), swap_color(color2),
                    0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColor2Vertex>(
                    centerXTrans, centerYTrans, radiusXTrans, -radiusYTrans,
                    swap_color(color1), swap_color(color2),
                    textureRight, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColor2Vertex>(
                    centerXTrans, centerYTrans, -radiusXTrans, radiusYTrans,
                    swap_color(color1), swap_color(color2),
                    0.0f, textureBottom
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColor2Vertex>(
                    centerXTrans, centerYTrans, radiusXTrans, radiusYTrans,
                    swap_color(color1), swap_color(color2),
                    textureRight, textureBottom
            );
            vertices++;
            return vertices;
        }

        ColorVertex *
        PrimitiveCreator::genEllipse(void *verticesVoid,
                                     int_fast32_t centerX, int_fast32_t centerY,
                                     uint_fast32_t rx, uint_fast32_t ry, uint_fast32_t color
        ) {
            return _ellipseHelper.genEllipse(verticesVoid, centerX, centerY, rx, ry, color);
        }

        uint_fast32_t PrimitiveCreator::getNumEllipseVertices(uint_fast32_t rx, uint_fast32_t ry) {
            return _ellipseHelper.getNumEllipseVertices(rx, ry);
        }

        float PrimitiveCreator::coordToPositionX(int_fast32_t coord, uint_fast32_t width) {
            return static_cast<float>(2.0 * (coord - 0.5) / width - 1);
        }

        float PrimitiveCreator::coordToPositionY(int_fast32_t coord, uint_fast32_t height) {
            return static_cast<float>(1 - 2.0 * (coord - 0.5) / height);
        }
    }
}
