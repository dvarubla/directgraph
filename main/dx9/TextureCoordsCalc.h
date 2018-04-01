#pragma once

#include <stdint.h>
#include <main/util.h>
#include "misc.h"

namespace directgraph {
    namespace dx9 {
        class TextureCoordsCalc {
        public:
            TextureCoords calcLineCoords(
                    const FCoords &prevEnd, const FCoords &start, const FCoords &end
            );

            TextureCoords calcLineCoords(
                    const FCoords &start, const FCoords &end
            );

            TextureCoords calcBarCoords(
                    const Coords &start, const Coords &end
            );

            TextureCoords addHalfPixel(const TextureCoords &textureCoords);
        };
    }
}


