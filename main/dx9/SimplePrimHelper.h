#pragma once

#include <main/util.h>
#include "misc.h"

namespace directgraph {
    namespace dx9 {
        class SimplePrimHelper {
        public:
            void * genQuad(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    float z,
                    uint_fast32_t color
            );

            void * genTexColorQuad(
                    void *verticesVoid,
                    const Coords &startCrds,
                    const Coords &endCrds,
                    float z,
                    uint_fast32_t color,
                    const TextureCoords &textureCoords,
                    bool rotate
            );
        };
    }
}


