#include <main/util.h>
#include <d3d9.h>
#include "PrimitiveCreator.h"

namespace directgraph{
    namespace dx9{
        ColorVertex *
        PrimitiveCreator::genDegenerate(
                void *verticesVoid,
                Coords startCrds,
                Coords endCrds
        ) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<ColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    0.0f,
                    1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
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
                Coords startCrds,
                Coords endCrds) {
            TexturedVertex *vertices = static_cast<TexturedVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    0.0f,
                    1.0f,
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
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
                Coords startCrds,
                Coords endCrds
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    0.0f,
                    1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
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
                Coords startCrds,
                Coords endCrds,
                uint_fast32_t color
        ) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color)
            );
            vertices++;
            return vertices;
        }

        Color2Vertex *
        PrimitiveCreator::genFillCol2Degenerate(void *verticesVoid, Coords startCrds,
                                                Coords endCrds,
                                                UCoords maxCrds) {
            float startXTrans = coordToPositionX(startCrds.x, maxCrds.x);
            float endXTrans = coordToPositionX(endCrds.x, maxCrds.x);
            float startYTrans = coordToPositionY(startCrds.y, maxCrds.y);
            float endYTrans = coordToPositionY(endCrds.y, maxCrds.y);
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
        PrimitiveCreator::genFillCol2Quad(void *verticesVoid, Coords startCrds,
                                          Coords endCrds, uint_fast32_t color1,
                                          uint_fast32_t color2, UCoords maxCrds) {
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            float textureRight = 1.0f * (endCrds.x - startCrds.x) / FPATTERN_SIZE;
            float textureBottom = 1.0f * (endCrds.y - startCrds.y) / FPATTERN_SIZE;
            float startXTrans = coordToPositionX(startCrds.x, maxCrds.x);
            float endXTrans = coordToPositionX(endCrds.x, maxCrds.x);
            float startYTrans = coordToPositionY(startCrds.y, maxCrds.y);
            float endYTrans = coordToPositionY(endCrds.y, maxCrds.y);
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
                Coords startCrds,
                Coords endCrds,
                uint_fast32_t color
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            float textureRight = 1.0f * (endCrds.x - startCrds.x) / FPATTERN_SIZE;
            float textureBottom = 1.0f * (endCrds.y - startCrds.y) / FPATTERN_SIZE;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color),
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color),
                    textureRight,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    0.0f,
                    1.0f,
                    swap_color(color),
                    0.0f,
                    textureBottom
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
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
                Coords startCrds,
                Coords endCrds,
                UCoords maxCrds
        ) {
            TexturedVertex *vertices = static_cast<TexturedVertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedVertex>(
                    startCrds.x - 0.5f, startCrds.y - 0.5f, 0.0f, 1.0f,
                    static_cast<float>(startCrds.x) / maxCrds.x, static_cast<float>(startCrds.y) / maxCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedVertex>(
                    endCrds.x - 0.5f, startCrds.y - 0.5f, 0.0f, 1.0f,
                    static_cast<float>(endCrds.x) / maxCrds.x, static_cast<float>(startCrds.y) / maxCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedVertex>(
                    startCrds.x - 0.5f, endCrds.y - 0.5f, 0.0f, 1.0f,
                    static_cast<float>(startCrds.x) / maxCrds.x, static_cast<float>(endCrds.y) / maxCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedVertex>(
                    endCrds.x - 0.5f, endCrds.y - 0.5f, 0.0f, 1.0f,
                    static_cast<float>(endCrds.x) / maxCrds.x, static_cast<float>(endCrds.y) / maxCrds.y
            );
            vertices++;
            return vertices;
        }

        PrimitiveCreator::PrimitiveCreator() {
        }

        ColorVertex *
        PrimitiveCreator::genEllipseDegenerate(
                void *verticesVoid, Coords startCrds,
                Coords endCrds,
                UCoords maxCrds
        ) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<ColorVertex>(
                    coordToPositionX(startCrds.x, maxCrds.x),
                    coordToPositionY(startCrds.y, maxCrds.y),
                    0.0f, 0.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    coordToPositionX(endCrds.x, maxCrds.x),
                    coordToPositionY(endCrds.y, maxCrds.y),
                    0.0f, 0.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            return vertices;
        }

        ColorVertex *
        PrimitiveCreator::genEllipseQuad(
                void *verticesVoid,
                Coords centerCrds,
                UCoords radiusCrds,
                UCoords maxCrds,
                uint_fast32_t color
        ) {
            float centerXTrans = coordToPositionX(centerCrds.x, maxCrds.x);
            float radiusXTrans = static_cast<float>(1.0 * radiusCrds.x / maxCrds.x);
            float centerYTrans = coordToPositionY(centerCrds.y, maxCrds.y);
            float radiusYTrans = static_cast<float>(1.0 * radiusCrds.y / maxCrds.y);
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
        PrimitiveCreator::genTexEllipseDegenerate(void *verticesVoid, 
                                                  Coords startCrds, Coords endCrds, 
                                                  UCoords maxCrds) {
            TexturedColor2Vertex *vertices = static_cast<TexturedColor2Vertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedColor2Vertex>(
                    coordToPositionX(startCrds.x, maxCrds.x),
                    coordToPositionY(startCrds.y, maxCrds.y),
                    0.0f, 0.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColor2Vertex>(
                    coordToPositionX(endCrds.x, maxCrds.x),
                    coordToPositionY(endCrds.y, maxCrds.y),
                    0.0f, 0.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f, 0.0f
            );
            vertices++;
            return vertices;
        }

        TexturedColor2Vertex *
        PrimitiveCreator::genTexEllipseQuad(void *verticesVoid, 
                                            Coords centerCrds, UCoords radiusCrds,
                                            uint_fast32_t color1, uint_fast32_t color2,
                                            UCoords maxCrds) {
            float centerXTrans = coordToPositionX(centerCrds.x, maxCrds.x);
            float radiusXTrans = static_cast<float>(1.0 * radiusCrds.x / maxCrds.x);
            float centerYTrans = coordToPositionY(centerCrds.y, maxCrds.y);
            float radiusYTrans = static_cast<float>(1.0 * radiusCrds.y / maxCrds.y);
            float textureRight = (2.0f * radiusCrds.x) / FPATTERN_SIZE;
            float textureBottom = (2.0f * radiusCrds.y) / FPATTERN_SIZE;
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
                                     Coords centerCrds, UCoords radiusCrds, uint_fast32_t color
        ) {
            return _ellipseHelper.genEllipse(verticesVoid, centerCrds.x, centerCrds.y, radiusCrds.x, radiusCrds.y, color);
        }

        uint_fast32_t PrimitiveCreator::getNumEllipseVertices(UCoords radiusCrds) {
            return _ellipseHelper.getNumEllipseVertices(radiusCrds.x, radiusCrds.y);
        }

        float PrimitiveCreator::coordToPositionX(int_fast32_t coord, uint_fast32_t width) {
            return static_cast<float>(2.0 * (coord - 0.5) / width - 1);
        }

        float PrimitiveCreator::coordToPositionY(int_fast32_t coord, uint_fast32_t height) {
            return static_cast<float>(1 - 2.0 * (coord - 0.5) / height);
        }
    }
}
