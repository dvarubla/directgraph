#include "util.h"

namespace directgraph{
    namespace dx9{
        FCoords subtHalfPixel(const FCoords &crds){
            FCoords ret = {crds.x - 0.5f, crds.y - 0.5f};
            return ret;
        }
    }
}