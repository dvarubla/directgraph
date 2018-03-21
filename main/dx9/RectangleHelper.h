#pragma once

#include <main/util.h>
#include "SimplePrimHelper.h"
#include "misc.h"
#include "TextureCoordsCalc.h"

namespace directgraph {
    namespace dx9{
        class RectangleHelper {
        private:
            SimplePrimHelper *_simplePrimHelper;
            TextureCoordsCalc *_texCrdCalc;
        public:
            RectangleHelper(SimplePrimHelper *simplePrimHelper, TextureCoordsCalc *texCrdCalc);
            void * genRectangle(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    uint_fast32_t thickness,
                    float z,
                    uint_fast32_t color,
                    bool textured
            );
            StartEndCoords getCoords(
                    const Coords &startCrds,
                    const Coords &endCrds,
                    uint_fast32_t thickness
            );
        };
    }
}


