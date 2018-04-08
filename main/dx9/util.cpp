#include "util.h"
#include "misc.h"

namespace directgraph{
    namespace dx9{
        FCoords subtHalfPixel(const FCoords &crds){
            FCoords ret = {crds.x - 0.5f, crds.y - 0.5f};
            return ret;
        }

        float addCorrOffset(float coord) {
            float _;
            if(std::abs(std::modf(coord, &_)) < CORR_OFFSET || std::abs(std::modf(coord, &_)) > (1 - CORR_OFFSET) ){
                coord -= CORR_OFFSET * 1.5f;
            }
            return coord;
        }

        FCoords addCorrOffset(const FCoords &coords) {
            FCoords res = {addCorrOffset(coords.x), addCorrOffset(coords.y)};
            return res;
        }
    }
}