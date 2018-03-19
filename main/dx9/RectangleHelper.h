#pragma once

#include <main/util.h>
#include "SimplePrimHelper.h"
#include "misc.h"

namespace directgraph {
    namespace dx9{
        class RectangleHelper {
        private:
            SimplePrimHelper *_simplePrimHelper;
        public:
            RectangleHelper(SimplePrimHelper *simplePrimHelper);
            void * genRectangle(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    uint_fast32_t thickness,
                    float z,
                    uint_fast32_t color
            );
            StartEndCoords getCoords(
                    const Coords &startCrds,
                    const Coords &endCrds,
                    uint_fast32_t thickness
            );
        };
    }
}


