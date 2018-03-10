#include <main/util.h>
#include <d3d9.h>
#include "PrimitiveCreator.h"

namespace directgraph{
    namespace dx9{
        void * PrimitiveCreator::genDegenerate(
                void *verticesVoid,
                const Coords &startCrds,
                const Coords &endCrds,
                float z
        ) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<ColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    z, 1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    z, 1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0)
            );
            vertices++;
            return vertices;
        }
        
        void * PrimitiveCreator::genFillDegenerate(
                void *verticesVoid,
                const Coords &startCrds,
                const Coords &endCrds,
                float z
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    z, 1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    z, 1.0f,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f,
                    0.0f
            );
            vertices++;
            return vertices;
        }

        void * PrimitiveCreator::genQuad(
                void *verticesVoid,
                const Coords &startCrds,
                const Coords &endCrds,
                float z,
                uint_fast32_t color
        ) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            return vertices;
        }

        void * PrimitiveCreator::genFillCol2Degenerate(void *verticesVoid, const Coords &startCrds,
                                                const Coords &endCrds,
                                                const UCoords &maxCrds,
                                                float z
        ) {
            float startXTrans = coordToPositionX(startCrds.x, maxCrds.x);
            float endXTrans = coordToPositionX(endCrds.x, maxCrds.x);
            float startYTrans = coordToPositionY(startCrds.y, maxCrds.y);
            float endYTrans = coordToPositionY(endCrds.y, maxCrds.y);
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<Color2Vertex>(
                    startXTrans,
                    startYTrans,
                    z,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    endXTrans,
                    endYTrans,
                    z,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f, 0.0f
            );
            vertices++;
            return vertices;
        }

        void * PrimitiveCreator::genFillCol2Quad(void *verticesVoid,
                                          const Coords &startCrds, const Coords &endCrds,
                                          float z,
                                          uint_fast32_t color1, uint_fast32_t color2,
                                          const UCoords &maxCrds
        ) {
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
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    endXTrans,
                    startYTrans,
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    textureRight,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    startXTrans,
                    endYTrans,
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    0.0f,
                    textureBottom
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    endXTrans,
                    endYTrans,
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    textureRight,
                    textureBottom
            );
            vertices++;
            return vertices;
        }

        void * PrimitiveCreator::genFillQuad(
                void *verticesVoid,
                const Coords &startCrds,
                const Coords &endCrds,
                float z,
                uint_fast32_t color
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            float textureRight = 1.0f * (endCrds.x - startCrds.x) / FPATTERN_SIZE;
            float textureBottom = 1.0f * (endCrds.y - startCrds.y) / FPATTERN_SIZE;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color),
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color),
                    textureRight,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color),
                    0.0f,
                    textureBottom
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color),
                    textureRight,
                    textureBottom
            );
            vertices++;
            return vertices;
        }

        void * PrimitiveCreator::genTexQuad(
                void *verticesVoid,
                const Coords &startCrds,
                const Coords &endCrds,
                const UCoords &maxCrds,
                float z
        ) {
            TexturedVertex *vertices = static_cast<TexturedVertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedVertex>(
                    startCrds.x - 0.5f, startCrds.y - 0.5f, z, 1.0f,
                    static_cast<float>(startCrds.x) / maxCrds.x, static_cast<float>(startCrds.y) / maxCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedVertex>(
                    endCrds.x - 0.5f, startCrds.y - 0.5f, z, 1.0f,
                    static_cast<float>(endCrds.x) / maxCrds.x, static_cast<float>(startCrds.y) / maxCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedVertex>(
                    startCrds.x - 0.5f, endCrds.y - 0.5f, z, 1.0f,
                    static_cast<float>(startCrds.x) / maxCrds.x, static_cast<float>(endCrds.y) / maxCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedVertex>(
                    endCrds.x - 0.5f, endCrds.y - 0.5f, z, 1.0f,
                    static_cast<float>(endCrds.x) / maxCrds.x, static_cast<float>(endCrds.y) / maxCrds.y
            );
            vertices++;
            return vertices;
        }

        PrimitiveCreator::PrimitiveCreator() {
        }

        void * PrimitiveCreator::genEllipseDegenerate(
                void *verticesVoid, const Coords &startCrds,
                const Coords &endCrds,
                const UCoords &maxCrds,
                float z
        ) {
            TexturedColorVertexNoRHW *vertices = static_cast<TexturedColorVertexNoRHW*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedColorVertexNoRHW>(
                    coordToPositionX(startCrds.x, maxCrds.x),
                    coordToPositionY(startCrds.y, maxCrds.y),
                    z,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    coordToPositionX(endCrds.x, maxCrds.x),
                    coordToPositionY(endCrds.y, maxCrds.y),
                    z,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f, 0.0f
            );
            vertices++;
            return vertices;
        }

        void * PrimitiveCreator::genEllipseQuad(
                void *verticesVoid,
                const Coords &centerCrds,
                const UCoords &radiusCrds,
                const UCoords &maxCrds,
                float z,
                uint_fast32_t color
        ) {
            float centerXTrans = coordToPositionX(centerCrds.x, maxCrds.x);
            float radiusXTrans = static_cast<float>(1.0 * radiusCrds.x / maxCrds.x);
            float centerYTrans = coordToPositionY(centerCrds.y, maxCrds.y);
            float radiusYTrans = static_cast<float>(1.0 * radiusCrds.y / maxCrds.y);
            TexturedColorVertexNoRHW *vertices = static_cast<TexturedColorVertexNoRHW*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    centerXTrans, centerYTrans, z, static_cast<DWORD>(swap_color_transp(color)), -radiusXTrans, -radiusYTrans
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    centerXTrans, centerYTrans, z, static_cast<DWORD>(swap_color_transp(color)), radiusXTrans, -radiusYTrans
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    centerXTrans, centerYTrans, z, static_cast<DWORD>(swap_color_transp(color)), -radiusXTrans, radiusYTrans
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    centerXTrans, centerYTrans, z, static_cast<DWORD>(swap_color_transp(color)), radiusXTrans, radiusYTrans
            );
            vertices++;
            return vertices;
        }

        void * PrimitiveCreator::genTexEllipseDegenerate(void *verticesVoid, 
                                                  const Coords &startCrds, const Coords &endCrds,
                                                  float z,
                                                  const UCoords &maxCrds) {
            TexturedColor2Vertex *vertices = static_cast<TexturedColor2Vertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedColor2Vertex>(
                    coordToPositionX(startCrds.x, maxCrds.x),
                    coordToPositionY(startCrds.y, maxCrds.y),
                    z,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f, 0.0f, 0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColor2Vertex>(
                    coordToPositionX(endCrds.x, maxCrds.x),
                    coordToPositionY(endCrds.y, maxCrds.y),
                    z,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f, 0.0f, 0.0f, 0.0f
            );
            vertices++;
            return vertices;
        }

        void * PrimitiveCreator::genTexEllipseQuad(void *verticesVoid, 
                                            const Coords &centerCrds, const UCoords &radiusCrds,
                                            float z,
                                            uint_fast32_t color1, uint_fast32_t color2,
                                            const UCoords &maxCrds) {
            float centerXTrans = coordToPositionX(centerCrds.x, maxCrds.x);
            float radiusXTrans = static_cast<float>(1.0 * radiusCrds.x / maxCrds.x);
            float centerYTrans = coordToPositionY(centerCrds.y, maxCrds.y);
            float radiusYTrans = static_cast<float>(1.0 * radiusCrds.y / maxCrds.y);
            float textureRight = (2.0f * radiusCrds.x) / FPATTERN_SIZE;
            float textureBottom = (2.0f * radiusCrds.y) / FPATTERN_SIZE;
            TexturedColor2Vertex *vertices = static_cast<TexturedColor2Vertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColor2Vertex>(
                    centerXTrans, centerYTrans, z,
                    swap_color_transp(color1), swap_color_transp(color2),
                    0.0f, 0.0f,
                    -radiusXTrans, -radiusYTrans
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColor2Vertex>(
                    centerXTrans, centerYTrans, z,
                    swap_color_transp(color1), swap_color_transp(color2),
                    textureRight, 0.0f,
                    radiusXTrans, -radiusYTrans
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColor2Vertex>(
                    centerXTrans, centerYTrans, z,
                    swap_color_transp(color1), swap_color_transp(color2),
                    0.0f, textureBottom,
                    -radiusXTrans, radiusYTrans
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColor2Vertex>(
                    centerXTrans, centerYTrans, z,
                    swap_color_transp(color1), swap_color_transp(color2),
                    textureRight, textureBottom,
                    radiusXTrans, radiusYTrans
            );
            vertices++;
            return vertices;
        }

        void * PrimitiveCreator::genEllipse(void *verticesVoid,
                                     const Coords &centerCrds, const UCoords &radiusCrds,
                                     float z,
                                     uint_fast32_t color,
                                     bool textured
        ) {
            return _ellipseHelper.genEllipse(verticesVoid, centerCrds, radiusCrds, z, color, textured);
        }

        uint_fast32_t PrimitiveCreator::getNumEllipseVertices(const UCoords &radiusCrds) {
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
