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
            return _simplePrimHelper.genQuad(verticesVoid, startCrds, endCrds, z, color);
        }

        void * PrimitiveCreator::genFillCol2Degenerate(void *verticesVoid, const Coords &startCrds,
                                                const Coords &endCrds,
                                                float z
        ) {
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(startCrds.x),
                    static_cast<float>(startCrds.y),
                    z,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(endCrds.x),
                    static_cast<float>(endCrds.y),
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
                                          uint_fast32_t color1, uint_fast32_t color2
        ) {
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            float textureRight = 1.0f * (endCrds.x - startCrds.x) / FPATTERN_SIZE;
            float textureBottom = 1.0f * (endCrds.y - startCrds.y) / FPATTERN_SIZE;
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(startCrds.x),
                    static_cast<float>(startCrds.y),
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(endCrds.x),
                    static_cast<float>(startCrds.y),
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    textureRight,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(startCrds.x),
                    static_cast<float>(endCrds.y),
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    0.0f,
                    textureBottom
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(endCrds.x),
                    static_cast<float>(endCrds.y),
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
            TextureCoords barCoords = _texCrdCalc.calcBarCoords(startCrds, endCrds);
            return _simplePrimHelper.genTexColorQuad(verticesVoid, startCrds, endCrds, z, color, barCoords, false);
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

        PrimitiveCreator::PrimitiveCreator() : _rectangleHelper(&_simplePrimHelper, &_texCrdCalc) {
        }

        void * PrimitiveCreator::genEllipseDegenerate(
                void *verticesVoid, const Coords &startCrds,
                const Coords &endCrds,
                float z
        ) {
            TexturedColorVertexNoRHW *vertices = static_cast<TexturedColorVertexNoRHW*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedColorVertexNoRHW>(
                    static_cast<float>(startCrds.x),
                    static_cast<float>(startCrds.y),
                    z,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    static_cast<float>(endCrds.x),
                    static_cast<float>(endCrds.y),
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
                float z,
                uint_fast32_t color
        ) {
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

        void * PrimitiveCreator::genTexEllipseDegenerate(void *verticesVoid, 
                                                  const Coords &startCrds, const Coords &endCrds,
                                                  float z) {
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(startCrds.x),
                    static_cast<float>(startCrds.y),
                    z,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(endCrds.x),
                    static_cast<float>(endCrds.y),
                    z,
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    D3DCOLOR_ARGB(0, 0, 0, 0),
                    0.0f, 0.0f
            );
            vertices++;
            return vertices;
        }

        void * PrimitiveCreator::genTexEllipseQuad(void *verticesVoid, 
                                            const Coords &centerCrds, const UCoords &radiusCrds,
                                            float z,
                                            uint_fast32_t color1, uint_fast32_t color2) {
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

        void *
        PrimitiveCreator::genRectangle(void *verticesVoid,
                                       const Coords &startCrds, const Coords &endCrds,
                                       uint_fast32_t thickness,
                                       float z,
                                       uint_fast32_t color,
                                       bool textured
        ) {
            return _rectangleHelper.genRectangle(verticesVoid, startCrds, endCrds, thickness, z, color, textured);
        }

        StartEndCoords PrimitiveCreator::getRectangleCoords(
                const Coords &startCrds, const Coords &endCrds,
                uint_fast32_t thickness
        ) {
            return _rectangleHelper.getCoords(startCrds, endCrds, thickness);
        }
    }
}
