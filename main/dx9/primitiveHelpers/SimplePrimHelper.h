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

            void * genEllipseQuad(
                    void *verticesVoid,
                    const FCoords &centerCrds,
                    const FCoords &radiusCrds,
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

            void *genQuad(void *verticesVoid, const QuadPointsArr &points, float z, uint_fast32_t color);

            void *genTexColorQuad(void *verticesVoid, const QuadPointsArr &points, float z, uint_fast32_t color,
                          const TextureCoords &textureCoords, bool rotate);
        };
    }
}


