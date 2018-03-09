#pragma once

#include <stdint.h>
#include "EllipseHelper.h"
#include "VertexCreator.h"
#include "patterns.h"

namespace directgraph {
    namespace dx9 {
        class PrimitiveCreator {
        private:
            EllipseHelper _ellipseHelper;
            float coordToPositionX(int_fast32_t coord, uint_fast32_t width);
            float coordToPositionY(int_fast32_t coord, uint_fast32_t height);
        public:
            PrimitiveCreator();

            ColorVertex *genDegenerate(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds,
                    float z
            );

            TexturedVertex *genTexDegenerate(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds
            );

            ColorVertex *genQuad(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds,
                    float z,
                    uint_fast32_t color
            );

            TexturedColorVertex *genFillDegenerate(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds,
                    float z
            );

            TexturedColorVertex *genFillQuad(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds,
                    float z,
                    uint_fast32_t color
            );

            Color2Vertex *genFillCol2Degenerate(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds,
                    UCoords maxCrds,
                    float z
            );

            Color2Vertex *genFillCol2Quad(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds,
                    float z,
                    uint_fast32_t color1, uint_fast32_t color2,
                    UCoords maxCrds
            );

            TexturedVertex *genTexQuad(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds,
                    UCoords maxCrds,
                    float z
            );

            TexturedColorVertexNoRHW *genEllipseDegenerate(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds,
                    UCoords maxCrds,
                    float z
            );

            TexturedColorVertexNoRHW * genEllipseQuad(
                    void *verticesVoid,
                    Coords centerCrds,
                    UCoords radiusCrds,
                    UCoords maxCrds,
                    float z,
                    uint_fast32_t color
            );

            TexturedColor2Vertex *genTexEllipseDegenerate(
                    void *verticesVoid, Coords startCrds, Coords endCrds,
                    float z,
                    UCoords maxCrds
            );

            TexturedColor2Vertex *genTexEllipseQuad(
                    void *verticesVoid, Coords centerCrds, UCoords radiusCrds,
                    float z,
                    uint_fast32_t color1, uint_fast32_t color2,
                    UCoords maxCrds
            );

            ColorVertex* genEllipse(
                    void *verticesVoid,
                    Coords centerCrds,
                    UCoords radiusCrds,
                    float z,
                    uint_fast32_t color
            );

            uint_fast32_t getNumEllipseVertices(
                    UCoords radiusCrds
            );
        };

    }
}


