#pragma once

#include <stdint.h>
#include <main/util.h>
#include "misc.h"

namespace directgraph {
    namespace dx9 {
        class TextureCoordsCalc {
        public:
            TextureCoords calcLineCoords(
                    float prevEnd, const FCoords &start, const FCoords &end,
                    bool needAddOffset = false
            );

            TextureCoords calcLineCoords(
                    const FCoords &start, const FCoords &end,
                    bool needAddOffset = false
            );

            FCoords addOffset(const FCoords &coords);

            TextureCoords calcBarCoords(
                    const DCoords &start, const DCoords &end
            );
        };
    }
}


