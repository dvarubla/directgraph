#pragma once

#include <main/util.h>
#include <main/Vect.h>

namespace directgraph{
    namespace dx9{
        FCoords subtHalfPixel(const FCoords &crds);

        float addCorrOffset(float coord);

        FCoords addCorrOffset(const FCoords &coords);
    }
}