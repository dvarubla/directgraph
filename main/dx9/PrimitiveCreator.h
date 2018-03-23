#pragma once

#include <stdint.h>
#include "EllipseHelper.h"
#include "VertexCreator.h"
#include "misc.h"
#include "patterns.h"
#include "RectangleHelper.h"
#include "SimplePrimHelper.h"
#include "TextureCoordsCalc.h"

namespace directgraph {
    namespace dx9 {
        class PrimitiveCreator {
        private:
            TextureCoordsCalc _texCrdCalc;
            EllipseHelper _ellipseHelper;
            SimplePrimHelper _simplePrimHelper;
            RectangleHelper _rectangleHelper;
        public:
            PrimitiveCreator();

            void * genDegenerate(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    float z
            );

            void * genQuad(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    float z,
                    uint_fast32_t color
            );

            void * genFillDegenerate(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    float z
            );

            void * genFillQuad(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    float z,
                    uint_fast32_t color
            );

            void * genFillCol2Degenerate(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    float z
            );

            void * genFillCol2Quad(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    float z,
                    uint_fast32_t color1, uint_fast32_t color2
            );

            void * genTexQuad(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    const UCoords &maxCrds,
                    float z
            );

            void * genEllipseDegenerate(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    float z
            );

            void * genEllipseQuad(
                    void *verticesVoid,
                    const Coords &centerCrds,
                    const UCoords &radiusCrds,
                    float z,
                    uint_fast32_t color
            );

            void * genTexRectangleDegenerate(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    float z
            );

            void * genTexRectangle(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    uint_fast32_t thickness,
                    float z,
                    uint_fast32_t color
            );

            void * genTexEllipseDegenerate(
                    void *verticesVoid, const Coords &startCrds, const Coords &endCrds,
                    float z
            );

            void * genTexEllipseQuad(
                    void *verticesVoid, const Coords &centerCrds, const UCoords &radiusCrds,
                    float z,
                    uint_fast32_t color1, uint_fast32_t color2
            );

            void * genEllipse(
                    void *verticesVoid,
                    const Coords &centerCrds,
                    const UCoords &radiusCrds,
                    float z,
                    uint_fast32_t color,
                    bool textured
            );

            uint_fast32_t getNumEllipseVertices(
                    const UCoords &radiusCrds
            );

            void * genRectangle(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    uint_fast32_t thickness,
                    float z,
                    uint_fast32_t color,
                    bool textured
            );

            StartEndCoords getRectangleCoords(
                    const Coords &startCrds,
                    const Coords &endCrds,
                    uint_fast32_t thickness
            );
        };

    }
}


