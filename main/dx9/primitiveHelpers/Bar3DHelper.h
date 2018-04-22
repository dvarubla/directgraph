#pragma once

#include <TextureCoordsCalc.h>
#include "SimplePrimHelper.h"
#include "DegenerateHelper.h"

namespace directgraph {
    namespace dx9 {
        class Bar3DHelper {
        private:
            SimplePrimHelper *_simplePrimHelper;
            DegenerateHelper *_degHelper;
            TextureCoordsCalc *_texCrdCalc;
            void *genData(void *verticesVoid, float len, float &prev,
                          const QuadPointsArr &points, float thickness, float z,
                          uint_fast32_t color, bool textured, bool swapCoords,
                          bool canCreateVertsWithExtra);
        public:
            Bar3DHelper(
                    SimplePrimHelper *simplePrimHelper, DegenerateHelper *degHelper,
                    TextureCoordsCalc *texCrdCalc
            );
            void * genBar3D(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    uint_fast32_t depth,
                    uint_fast32_t thickness,
                    float z,
                    uint_fast32_t color,
                    bool textured,
                    bool haveTop,
                    bool canCreateVertsWithExtra
            );
            StartEndCoords getCoords(
                    const Coords &startCrds,
                    const Coords &endCrds,
                    uint_fast32_t thickness,
                    bool haveTop
            );
        };
    }
}


