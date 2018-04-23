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
        SimplePrimHelper::genFillCol2NoRHWQuad(
                void *verticesVoid, const FCoords &startCrds, const FCoords &endCrds, float z,
                uint_fast32_t color1, uint_fast32_t color2,
                const TextureCoords &textureCoords
        ) {
            Color2VertexNoRHW *vertices = static_cast<Color2VertexNoRHW*>(verticesVoid);
            *vertices = VertexCreator::create<Color2VertexNoRHW>(
                    startCrds.x,
                    startCrds.y,
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    textureCoords.start.x,
                    textureCoords.start.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2VertexNoRHW>(
                    endCrds.x,
                    startCrds.y,
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    textureCoords.end.x,
                    textureCoords.start.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2VertexNoRHW>(
                    startCrds.x,
                    endCrds.y,
                    z,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    textureCoords.start.x,
                    textureCoords.end.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2VertexNoRHW>(
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

        void *SimplePrimHelper::genTexColorNoRHWQuad(void *verticesVoid, const FCoords &centerCrds,
                                                     const FCoords &endTextureCrds,
                                                     float z, uint_fast32_t color) {
            TexturedColorVertexNoRHW *vertices = static_cast<TexturedColorVertexNoRHW*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    centerCrds.x, centerCrds.y, z, static_cast<DWORD>(swap_color_transp(color)),
                    -endTextureCrds.x, -endTextureCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    centerCrds.x, centerCrds.y, z, static_cast<DWORD>(swap_color_transp(color)),
                    endTextureCrds.x, -endTextureCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    centerCrds.x, centerCrds.y, z, static_cast<DWORD>(swap_color_transp(color)),
                    -endTextureCrds.x, endTextureCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    centerCrds.x, centerCrds.y, z, static_cast<DWORD>(swap_color_transp(color)),
                    endTextureCrds.x, endTextureCrds.y
            );
            vertices++;
            return vertices;
        }

        void *
        SimplePrimHelper::genTexEllipseQuad(void *verticesVoid, const FCoords &centerCrds, const FCoords &radiusCrds,
                                            float z, uint_fast32_t color1, uint_fast32_t color2) {
            Color2VertexNoRHW *vertices = static_cast<Color2VertexNoRHW*>(verticesVoid);
            *vertices = VertexCreator::create<Color2VertexNoRHW>(
                    centerCrds.x, centerCrds.y, z,
                    static_cast<DWORD>(swap_color_transp(color1)), static_cast<DWORD>(swap_color_transp(color2)),
                    -radiusCrds.x, -radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2VertexNoRHW>(
                    centerCrds.x, centerCrds.y, z,
                    static_cast<DWORD>(swap_color_transp(color1)), static_cast<DWORD>(swap_color_transp(color2)),
                    radiusCrds.x, -radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2VertexNoRHW>(
                    centerCrds.x, centerCrds.y, z,
                    static_cast<DWORD>(swap_color_transp(color1)), static_cast<DWORD>(swap_color_transp(color2)),
                    -radiusCrds.x, radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2VertexNoRHW>(
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

        void *
        SimplePrimHelper::genTexColorQuad(void *verticesVoid, const QuadPointsArr &points, float z, uint_fast32_t color,
                                          const TextureCoords &textureCoords, bool rotate) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    points[0].x,
                    points[0].y,
                    z, 1.0f,
                    swap_color_transp(color),
                    textureCoords.start.x,
                    textureCoords.start.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    points[1].x,
                    points[1].y,
                    z, 1.0f,
                    swap_color_transp(color),
                    (rotate) ? textureCoords.start.x : textureCoords.end.x,
                    (rotate) ? textureCoords.end.y : textureCoords.start.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    points[2].x,
                    points[2].y,
                    z, 1.0f,
                    swap_color_transp(color),
                    (rotate) ? textureCoords.end.x : textureCoords.start.x,
                    (rotate) ? textureCoords.start.y : textureCoords.end.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    points[3].x,
                    points[3].y,
                    z, 1.0f,
                    swap_color_transp(color),
                    textureCoords.end.x,
                    textureCoords.end.y
            );
            vertices++;
            return vertices;
        }

        void *SimplePrimHelper::genQuadExtra(void *verticesVoid, const QuadPointsArr &points, const float (&extraVal)[4], float z, uint_fast32_t color) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<ColorVertex>(
                    points[0].x,
                    points[0].y,
                    z, extraVal[0],
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    points[1].x,
                    points[1].y,
                    z, extraVal[1],
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    points[2].x,
                    points[2].y,
                    z, extraVal[2],
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    points[3].x,
                    points[3].y,
                    z, extraVal[3],
                    swap_color_transp(color)
            );
            vertices++;
            return vertices;
        }

        void *SimplePrimHelper::genTriangles(void *verticesVoid, const CoordVect &points, float z,
                                             uint_fast32_t color) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            for(CoordVect::const_iterator it = points.begin(); it != points.end(); ++it){
                *vertices = VertexCreator::create<ColorVertex>(
                        it->x,
                        it->y,
                        z, 1.0f,
                        swap_color_transp(color)
                );
                vertices++;
            }
            return vertices;
        }

        void *SimplePrimHelper::genTexTriangles(
                void *verticesVoid, const CoordVect &points, const CoordVect &texCoords, float z, uint_fast32_t color
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            for(CoordVect::const_iterator it = points.begin(); it != points.end(); ++it){
                *vertices = VertexCreator::create<TexturedColorVertex>(
                        it->x,
                        it->y,
                        z, 1.0f,
                        swap_color_transp(color),
                        texCoords[it - points.begin()].x,
                        texCoords[it - points.begin()].y
                );
                vertices++;
            }
            return vertices;
        }

        void *
        SimplePrimHelper::genTrianglesExtra(void *verticesVoid, const CoordVect &points, const CoordVect &extraCoords,
                                            float z, uint_fast32_t color) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            for(CoordVect::const_iterator it = points.begin(); it != points.end(); ++it){
                *vertices = VertexCreator::create<ColorVertex>(
                        it->x,
                        it->y,
                        z, extraCoords[it - points.begin()].x,
                        swap_color_transp(color)
                );
                vertices++;
            }
            return vertices;
        }

        void *
        SimplePrimHelper::genFillCol2Triangles(void *verticesVoid, const CoordVect &points, const CoordVect &texCoords,
                                               float z, uint_fast32_t color1, uint_fast32_t color2) {
            Color2VertexNoRHW *vertices = static_cast<Color2VertexNoRHW*>(verticesVoid);
            for(CoordVect::const_iterator it = points.begin(); it != points.end(); ++it){
                *vertices = VertexCreator::create<Color2VertexNoRHW>(
                        it->x,
                        it->y,
                        z,
                        static_cast<DWORD>(swap_color_transp(color1)),
                        static_cast<DWORD>(swap_color_transp(color2)),
                        texCoords[it - points.begin()].x,
                        texCoords[it - points.begin()].y
                );
                vertices++;
            }
            return vertices;
        }

        void *
        SimplePrimHelper::genTexColorQuad(void *verticesVoid, const QuadPointsArr &points,
                                          const float (&texCoords)[4],
                                          float z, uint_fast32_t color) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    points[0].x,
                    points[0].y,
                    z, 1.0f,
                    static_cast<DWORD>(swap_color_transp(color)),
                    texCoords[0], 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    points[1].x,
                    points[1].y,
                    z, 1.0f,
                    static_cast<DWORD>(swap_color_transp(color)),
                    texCoords[1], 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    points[2].x,
                    points[2].y,
                    z, 1.0f,
                    static_cast<DWORD>(swap_color_transp(color)),
                    texCoords[2], 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    points[3].x,
                    points[3].y,
                    z, 1.0f,
                    static_cast<DWORD>(swap_color_transp(color)),
                    texCoords[3], 0.0f
            );
            vertices++;
            return vertices;
        }
        
        void *SimplePrimHelper::genOutlineEllipseQuad(void *verticesVoid, const FCoords &centerCrds,
                                                      const FCoords &radiusCrds, float z, uint_fast32_t color,
                                                      uint_fast32_t thickness) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    centerCrds.x, centerCrds.y, z, static_cast<float>(thickness), static_cast<DWORD>(swap_color_transp(color)),
                    -radiusCrds.x, -radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    centerCrds.x, centerCrds.y, z, static_cast<float>(thickness),  static_cast<DWORD>(swap_color_transp(color)),
                    radiusCrds.x, -radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    centerCrds.x, centerCrds.y, z, static_cast<float>(thickness),  static_cast<DWORD>(swap_color_transp(color)),
                    -radiusCrds.x, radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    centerCrds.x, centerCrds.y, z, static_cast<float>(thickness),  static_cast<DWORD>(swap_color_transp(color)),
                    radiusCrds.x, radiusCrds.y
            );
            vertices++;
            return vertices;
        }

        void *SimplePrimHelper::genEllipseWithOutlineQuad(void *verticesVoid, const FCoords &centerCrds,
                                                          const FCoords &radiusCrds, float z, uint_fast32_t color1,
                                                          uint_fast32_t color2, uint_fast32_t thickness) {
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            *vertices = VertexCreator::create<Color2Vertex>(
                    centerCrds.x,
                    centerCrds.y,
                    z, thickness,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    -radiusCrds.x, -radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    centerCrds.x,
                    centerCrds.y,
                    z, thickness,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    radiusCrds.x, -radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    centerCrds.x,
                    centerCrds.y,
                    z, thickness,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    -radiusCrds.x, radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    centerCrds.x,
                    centerCrds.y,
                    z, thickness,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    radiusCrds.x, radiusCrds.y
            );
            vertices++;
            return vertices;
        }

        void *SimplePrimHelper::genTexEllipseWithOutlineQuad(void *verticesVoid, const FCoords &centerCrds,
                                                             const FCoords &radiusCrds, float z, uint_fast32_t color1,
                                                             uint_fast32_t color2, uint_fast32_t color3,
                                                             uint_fast32_t thickness) {
            Color3Vertex *vertices = static_cast<Color3Vertex*>(verticesVoid);
            *vertices = VertexCreator::create<Color3Vertex>(
                    centerCrds.x,
                    centerCrds.y,
                    z, thickness,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    static_cast<DWORD>(swap_color_transp(color3)),
                    -radiusCrds.x, -radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color3Vertex>(
                    centerCrds.x,
                    centerCrds.y,
                    z, thickness,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    static_cast<DWORD>(swap_color_transp(color3)),
                    radiusCrds.x, -radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color3Vertex>(
                    centerCrds.x,
                    centerCrds.y,
                    z, thickness,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    static_cast<DWORD>(swap_color_transp(color3)),
                    -radiusCrds.x, radiusCrds.y
            );
            vertices++;
            *vertices = VertexCreator::create<Color3Vertex>(
                    centerCrds.x,
                    centerCrds.y,
                    z, thickness,
                    static_cast<DWORD>(swap_color_transp(color1)),
                    static_cast<DWORD>(swap_color_transp(color2)),
                    static_cast<DWORD>(swap_color_transp(color3)),
                    radiusCrds.x, radiusCrds.y
            );
            vertices++;
            return vertices;
        }
    }
}
