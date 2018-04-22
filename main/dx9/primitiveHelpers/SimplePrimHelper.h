#pragma once

#include <main/util.h>
#include "misc.h"

namespace directgraph {
    namespace dx9 {
        class SimplePrimHelper {
        public:
            void * genQuad(
                    void *verticesVoid,
                    const FCoords &startCrds,
                    const FCoords &endCrds,
                    float z,
                    uint_fast32_t color
            );

            void * genTexColorQuad(
                    void *verticesVoid,
                    const FCoords &startCrds,
                    const FCoords &endCrds,
                    float z,
                    uint_fast32_t color,
                    const TextureCoords &textureCoords,
                    bool rotate
            );

            void *genFillCol2Quad(
                    void *verticesVoid,
                    const FCoords &startCrds,
                    const FCoords &endCrds,
                    float z,
                    uint_fast32_t color1, uint_fast32_t color2,
                    const TextureCoords &textureCoords
            );

            void * genTexRectangle(
                    void *verticesVoid,
                    const FCoords &startCrds,
                    const FCoords &endCrds,
                    uint_fast32_t thickness,
                    float z,
                    uint_fast32_t color
            );

            void * genTexColorNoRHWQuad(
                    void *verticesVoid,
                    const FCoords &centerCrds,
                    const FCoords &endTextureCrds,
                    float z,
                    uint_fast32_t color
            );

            void * genTexEllipseQuad(
                    void *verticesVoid,
                    const FCoords &centerCrds, const FCoords &radiusCrds,
                    float z,
                    uint_fast32_t color1, uint_fast32_t color2
            );

            void * genTexQuad(
                    void *verticesVoid,
                    const FCoords &startCrds,
                    const FCoords &endCrds,
                    const UCoords &maxCrds,
                    float z
            );

            void *genFillCol2Triangles(
                    void *verticesVoid,
                    const CoordVect &points, const CoordVect &texCoords,
                    float z,
                    uint_fast32_t color1, uint_fast32_t color2
            );

            void *genQuad(void *verticesVoid, const QuadPointsArr &points, float z, uint_fast32_t color);

            void *genTexColorQuad(void *verticesVoid, const QuadPointsArr &points, float z, uint_fast32_t color,
                          const TextureCoords &textureCoords, bool rotate);

            void *genTexColorQuad(void *verticesVoid, const QuadPointsArr &points, const float (&texCoords)[4], float z, uint_fast32_t color);

            void *genQuadExtra(void *verticesVoid, const QuadPointsArr &points, const float (&extraVal)[4], float z, uint_fast32_t color);

            void *genTriangles(void *verticesVoid, const CoordVect &points, float z, uint_fast32_t color);

            void *genTexTriangles(
                    void *verticesVoid, const CoordVect &points, const CoordVect &texCoords, float z, uint_fast32_t color
            );

            void *genTrianglesExtra(
                    void *verticesVoid, const CoordVect &points, const CoordVect &extraCoords, float z, uint_fast32_t color
            );

        };
    }
}


