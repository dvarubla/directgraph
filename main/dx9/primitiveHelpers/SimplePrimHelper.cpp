#include <util.h>
#include "SimplePrimHelper.h"
#include "VertexCreator.h"

namespace directgraph {
    namespace dx9 {
        void *SimplePrimHelper::genQuad(void *verticesVoid, const FCoords &startCrds, const FCoords &endCrds, float z,
                                        uint_fast32_t color) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<ColorVertex>(
                    startCrds.x,
                    startCrds.y,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    endCrds.x,
                    startCrds.y,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    startCrds.x,
                    endCrds.y,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    endCrds.x,
                    endCrds.y,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            return vertices;
        }

        void *
        SimplePrimHelper::genTexColorQuad(
                void *verticesVoid, const FCoords &startCrds, const FCoords &endCrds, float z,
                uint_fast32_t color, const TextureCoords &textureCoords, bool rotate
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    startCrds.x,
                    startCrds.y,
                    z, 1.0f,
                    swap_color_transp(color),
                    textureCoords.start.x,
                    textureCoords.start.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    endCrds.x,
                    startCrds.y,
                    z, 1.0f,
                    swap_color_transp(color),
                    (rotate) ? textureCoords.start.x : textureCoords.end.x,
                    (rotate) ? textureCoords.end.y : textureCoords.start.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    startCrds.x,
                    endCrds.y,
                    z, 1.0f,
                    swap_color_transp(color),
                    (rotate) ? textureCoords.end.x : textureCoords.start.x,
                    (rotate) ? textureCoords.start.y : textureCoords.end.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    endCrds.x,
                    endCrds.y,
                    z, 1.0f,
                    swap_color_transp(color),
                    textureCoords.end.x,
                    textureCoords.end.y
            );
            vertices++;
            return vertices;
        }

        void *
        SimplePrimHelper::genFillCol2Quad(
                void *verticesVoid, const FCoords &startCrds, const FCoords &endCrds, float z,
                uint_fast32_t color1, uint_fast32_t color2,
                const TextureCoords &textureCoords
        ) {
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            *vertices = VertexCreator::create<Color2Vertex>(
                    startCrds.x,
                    startCrds.y,
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    textureCoords.start.x,
                    textureCoords.start.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    endCrds.x,
                    startCrds.y,
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    textureCoords.end.x,
                    textureCoords.start.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    startCrds.x,
                    endCrds.y,
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    textureCoords.start.x,
                    textureCoords.end.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    endCrds.x,
                    endCrds.y,
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    textureCoords.end.x,
                    textureCoords.end.y
            );
            vertices++;
            return vertices;
        }

        void *SimplePrimHelper::genTexRectangle(
                void *verticesVoid, const FCoords &startCrds, const FCoords &endCrds,
                uint_fast32_t thickness, float z, uint_fast32_t color
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    startCrds.x, startCrds.y,
                    z, static_cast<float>(thickness), static_cast<DWORD>(swap_color_transp(color)),
                    -(endCrds.x - startCrds.x),
                    -(endCrds.y - startCrds.y)
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    endCrds.x, startCrds.y,
                    z, static_cast<float>(thickness), static_cast<DWORD>(swap_color_transp(color)),
                    endCrds.x - startCrds.x,
                    -(endCrds.y - startCrds.y)
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    startCrds.x, endCrds.y,
                    z, static_cast<float>(thickness), static_cast<DWORD>(swap_color_transp(color)),
                    -(endCrds.x - startCrds.x),
                    endCrds.y - startCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    endCrds.x, endCrds.y,
                    z, static_cast<float>(thickness), static_cast<DWORD>(swap_color_transp(color)),
                    endCrds.x - startCrds.x,
                    endCrds.y - startCrds.y
            );
            vertices++;
            return vertices;
        }

        void *SimplePrimHelper::genEllipseQuad(void *verticesVoid, const FCoords &centerCrds, const FCoords &radiusCrds,
                                               float z, uint_fast32_t color) {
            TexturedColorVertexNoRHW *vertices = static_cast<TexturedColorVertexNoRHW*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    centerCrds.x, centerCrds.y, z, static_cast<DWORD>(swap_color_transp(color)),
                    -radiusCrds.x, -radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    centerCrds.x, centerCrds.y, z, static_cast<DWORD>(swap_color_transp(color)),
                    radiusCrds.x, -radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    centerCrds.x, centerCrds.y, z, static_cast<DWORD>(swap_color_transp(color)),
                    -radiusCrds.x, radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    centerCrds.x, centerCrds.y, z, static_cast<DWORD>(swap_color_transp(color)),
                    radiusCrds.x, radiusCrds.y
            );
            vertices++;
            return vertices;
        }

        void *
        SimplePrimHelper::genTexEllipseQuad(void *verticesVoid, const FCoords &centerCrds, const FCoords &radiusCrds,
                                            float z, uint_fast32_t color1, uint_fast32_t color2) {
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            *vertices = VertexCreator::create<Color2Vertex>(
                    centerCrds.x, centerCrds.y, z,
                    static_cast<DWORD>(swap_color_transp(color1)), static_cast<DWORD>(swap_color_transp(color2)),
                    -radiusCrds.x, -radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    centerCrds.x, centerCrds.y, z,
                    static_cast<DWORD>(swap_color_transp(color1)), static_cast<DWORD>(swap_color_transp(color2)),
                    radiusCrds.x, -radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    centerCrds.x, centerCrds.y, z,
                    static_cast<DWORD>(swap_color_transp(color1)), static_cast<DWORD>(swap_color_transp(color2)),
                    -radiusCrds.x, radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    centerCrds.x, centerCrds.y, z,
                    static_cast<DWORD>(swap_color_transp(color1)), static_cast<DWORD>(swap_color_transp(color2)),
                    radiusCrds.x, radiusCrds.y
            );
            vertices++;
            return vertices;
        }

        void *SimplePrimHelper::genTexQuad(
                void *verticesVoid, const FCoords &startCrds, const FCoords &endCrds,
                const UCoords &maxCrds, float z
        ) {
            TexturedVertex *vertices = static_cast<TexturedVertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedVertex>(
                    startCrds.x - 0.5f, startCrds.y - 0.5f, z, 1.0f,
                    startCrds.x / maxCrds.x, startCrds.y / maxCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedVertex>(
                    endCrds.x - 0.5f, startCrds.y - 0.5f, z, 1.0f,
                    endCrds.x / maxCrds.x, startCrds.y / maxCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedVertex>(
                    startCrds.x - 0.5f, endCrds.y - 0.5f, z, 1.0f,
                    startCrds.x / maxCrds.x, endCrds.y / maxCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedVertex>(
                    endCrds.x - 0.5f, endCrds.y - 0.5f, z, 1.0f,
                    endCrds.x / maxCrds.x, endCrds.y / maxCrds.y
            );
            vertices++;
            return vertices;
        }

        void *SimplePrimHelper::genQuad(void *verticesVoid, const QuadPointsArr &points, float z, uint_fast32_t color) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<ColorVertex>(
                    points[0].x,
                    points[0].y,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    points[1].x,
                    points[1].y,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    points[2].x,
                    points[2].y,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    points[3].x,
                    points[3].y,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            return vertices;
        }
    }
}
