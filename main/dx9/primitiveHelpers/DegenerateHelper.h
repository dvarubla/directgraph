#pragma once

#include <main/util.h>

namespace directgraph {
    namespace dx9 {
        class DegenerateHelper {
        public:
            void * genDegenerate(
                    void *verticesVoid,
                    const FCoords &startCrds,
                    const FCoords &endCrds,
                    float z
            );

            void * genTexDegenerate(
                    void *verticesVoid,
                    const FCoords &startCrds,
                    const FCoords &endCrds,
                    float z
            );

            void * genTexCol2Degenerate(
                    void *verticesVoid,
                    const FCoords &startCrds,
                    const FCoords &endCrds,
                    float z
            );

            void * genShaderRectangleDegenerate(
                    void *verticesVoid,
                    const FCoords &startCrds,
                    const FCoords &endCrds,
                    float z
            );

            void * genEllipseDegenerate(
                    void *verticesVoid, const FCoords &startCrds, const FCoords &endCrds, float z
            );

            void * genTexEllipseDegenerate(
                    void *verticesVoid, const FCoords &startCrds, const FCoords &endCrds, float z
            );
        };
    }
}


