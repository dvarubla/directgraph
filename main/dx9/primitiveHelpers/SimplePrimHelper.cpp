#include "SimplePrimHelper.h"
#include "VertexCreator.h"

namespace directgraph {
    namespace dx9 {
        void *SimplePrimHelper::genQuad(void *verticesVoid, const Coords &startCrds, const Coords &endCrds, float z,
                                        uint_fast32_t color) {
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

        void *
        SimplePrimHelper::genTexColorQuad(
                void *verticesVoid, const Coords &startCrds, const Coords &endCrds, float z,
                uint_fast32_t color, const TextureCoords &textureCoords, bool rotate
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color),
                    textureCoords.start.x,
                    textureCoords.start.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color),
                    (rotate) ? textureCoords.start.x : textureCoords.end.x,
                    (rotate) ? textureCoords.end.y : textureCoords.start.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color),
                    (rotate) ? textureCoords.end.x : textureCoords.start.x,
                    (rotate) ? textureCoords.start.y : textureCoords.end.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
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
                void *verticesVoid, const Coords &startCrds, const Coords &endCrds, float z,
                uint_fast32_t color1, uint_fast32_t color2,
                const TextureCoords &textureCoords
        ) {
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(startCrds.x),
                    static_cast<float>(startCrds.y),
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    textureCoords.start.x,
                    textureCoords.start.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(endCrds.x),
                    static_cast<float>(startCrds.y),
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    textureCoords.end.x,
                    textureCoords.start.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(startCrds.x),
                    static_cast<float>(endCrds.y),
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    textureCoords.start.x,
                    textureCoords.end.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(endCrds.x),
                    static_cast<float>(endCrds.y),
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
                void *verticesVoid, const Coords &startCrds, const Coords &endCrds,
                uint_fast32_t thickness, float z, uint_fast32_t color
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startCrds.x), static_cast<float>(startCrds.y),
                    z, static_cast<float>(thickness), static_cast<DWORD>(swap_color_transp(color)),
                    static_cast<float>(-(endCrds.x - startCrds.x)),
                    static_cast<float>(-(endCrds.y - startCrds.y))
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endCrds.x), static_cast<float>(startCrds.y),
                    z, static_cast<float>(thickness), static_cast<DWORD>(swap_color_transp(color)),
                    static_cast<float>(endCrds.x - startCrds.x),
                    static_cast<float>(-(endCrds.y - startCrds.y))
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startCrds.x), static_cast<float>(endCrds.y),
                    z, static_cast<float>(thickness), static_cast<DWORD>(swap_color_transp(color)),
                    static_cast<float>(-(endCrds.x - startCrds.x)),
                    static_cast<float>(endCrds.y - startCrds.y)
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endCrds.x), static_cast<float>(endCrds.y),
                    z, static_cast<float>(thickness), static_cast<DWORD>(swap_color_transp(color)),
                    static_cast<float>(endCrds.x - startCrds.x),
                    static_cast<float>(endCrds.y - startCrds.y)
            );
            vertices++;
            return vertices;
        }

        void *SimplePrimHelper::genEllipseQuad(void *verticesVoid, const Coords &centerCrds, const UCoords &radiusCrds,
                                               float z, uint_fast32_t color) {
            TexturedColorVertexNoRHW *vertices = static_cast<TexturedColorVertexNoRHW*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    static_cast<float>(centerCrds.x), static_cast<float>(centerCrds.y), z, static_cast<DWORD>(swap_color_transp(color)),
                    -static_cast<float>(radiusCrds.x), -static_cast<float>(radiusCrds.y)
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    static_cast<float>(centerCrds.x), static_cast<float>(centerCrds.y), z, static_cast<DWORD>(swap_color_transp(color)),
                    static_cast<float>(radiusCrds.x), -static_cast<float>(radiusCrds.y)
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    static_cast<float>(centerCrds.x), static_cast<float>(centerCrds.y), z, static_cast<DWORD>(swap_color_transp(color)),
                    -static_cast<float>(radiusCrds.x), static_cast<float>(radiusCrds.y)
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    static_cast<float>(centerCrds.x), static_cast<float>(centerCrds.y), z, static_cast<DWORD>(swap_color_transp(color)),
                    static_cast<float>(radiusCrds.x), static_cast<float>(radiusCrds.y)
            );
            vertices++;
            return vertices;
        }

        void *
        SimplePrimHelper::genTexEllipseQuad(void *verticesVoid, const Coords &centerCrds, const UCoords &radiusCrds,
                                            float z, uint_fast32_t color1, uint_fast32_t color2) {
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(centerCrds.x), static_cast<float>(centerCrds.y), z,
                    static_cast<DWORD>(swap_color_transp(color1)), static_cast<DWORD>(swap_color_transp(color2)),
                    -static_cast<float>(radiusCrds.x), -static_cast<float>(radiusCrds.y)
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(centerCrds.x), static_cast<float>(centerCrds.y), z,
                    static_cast<DWORD>(swap_color_transp(color1)), static_cast<DWORD>(swap_color_transp(color2)),
                    static_cast<float>(radiusCrds.x), -static_cast<float>(radiusCrds.y)
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(centerCrds.x), static_cast<float>(centerCrds.y), z,
                    static_cast<DWORD>(swap_color_transp(color1)), static_cast<DWORD>(swap_color_transp(color2)),
                    -static_cast<float>(radiusCrds.x), static_cast<float>(radiusCrds.y)
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(centerCrds.x), static_cast<float>(centerCrds.y), z,
                    static_cast<DWORD>(swap_color_transp(color1)), static_cast<DWORD>(swap_color_transp(color2)),
                    static_cast<float>(radiusCrds.x), static_cast<float>(radiusCrds.y)
            );
            vertices++;
            return vertices;
        }

        void *SimplePrimHelper::genTexQuad(
                void *verticesVoid, const Coords &startCrds, const Coords &endCrds,
                const UCoords &maxCrds, float z
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
    }
}
