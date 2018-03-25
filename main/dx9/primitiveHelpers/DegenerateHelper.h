#pragma once

#include <main/util.h>

namespace directgraph {
    namespace dx9 {
        class DegenerateHelper {
        public:
            void * genDegenerate(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    float z
            );

            void * genTexDegenerate(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    float z
            );

            void * genTexCol2Degenerate(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    float z
            );

            void * genTexRectangleDegenerate(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    float z
            );

            void * genEllipseDegenerate(
                    void *verticesVoid, const Coords &startCrds, const Coords &endCrds, float z
            );

            void * genTexEllipseDegenerate(
                    void *verticesVoid, const Coords &startCrds, const Coords &endCrds, float z
            );
        };
    }
}


