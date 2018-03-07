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
                    Coords endCrds
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
                    uint_fast32_t color
            );

            TexturedColorVertex *genFillDegenerate(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds
            );

            TexturedColorVertex *genFillQuad(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds,
                    uint_fast32_t color
            );

            Color2Vertex *genFillCol2Degenerate(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds,
                    UCoords maxCrds
            );

            Color2Vertex *genFillCol2Quad(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds,
                    uint_fast32_t color1, uint_fast32_t color2,
                    UCoords maxCrds
            );

            TexturedVertex *genTexQuad(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds,
                    UCoords maxCrds
            );

            ColorVertex *genEllipseDegenerate(
                    void *verticesVoid,
                    Coords startCrds,
                    Coords endCrds,
                    UCoords maxCrds
            );

            ColorVertex *genEllipseQuad(
                    void *verticesVoid,
                    Coords centerCrds,
                    UCoords radiusCrds,
                    UCoords maxCrds,
                    uint_fast32_t color
            );

            TexturedColor2Vertex *genTexEllipseDegenerate(
                    void *verticesVoid, Coords startCrds, Coords endCrds,
                    UCoords maxCrds
            );

            TexturedColor2Vertex *genTexEllipseQuad(
                    void *verticesVoid, Coords centerCrds, UCoords radiusCrds,
                    uint_fast32_t color1, uint_fast32_t color2,
                    UCoords maxCrds
            );

            ColorVertex* genEllipse(
                    void *verticesVoid,
                    Coords centerCrds,
                    UCoords radiusCrds,
                    uint_fast32_t color
            );

            uint_fast32_t getNumEllipseVertices(
                    UCoords radiusCrds
            );
        };

    }
}


