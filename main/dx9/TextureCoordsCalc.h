#pragma once

#include <stdint.h>
#include <main/util.h>
#include "misc.h"

namespace directgraph {
    namespace dx9 {
        class TextureCoordsCalc {
        public:
            TextureCoords calcLineCoords(
                    const FCoords &prevEnd,  const Coords &start, const Coords &end
            );

            TextureCoords calcBarCoords(
                    const Coords &start, const Coords &end
            );
        };
    }
}


