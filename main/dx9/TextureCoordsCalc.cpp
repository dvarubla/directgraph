#include <main/patterns.h>
#include "TextureCoordsCalc.h"

namespace directgraph{
    namespace dx9{
        TextureCoords
        TextureCoordsCalc::calcLineCoords(float prevEnd, const FCoords &start, const FCoords &end, bool needAddOffset) {
            FCoords texStart = {
                    prevEnd, 0
            };

            if(needAddOffset){
                texStart = addOffset(texStart);
            }

            FCoords texEnd = {
                    1.0f * std::abs(end.x - start.x) / LPATTERN_SIZE + texStart.x, 0.0f
            };
            TextureCoords res = {texStart, texEnd};
            return res;
        }

        TextureCoords TextureCoordsCalc::calcBarCoords(const DCoords &start, const DCoords &end) {
            TextureCoords res = {
                    genFCoords(0, 0), genFCoords(1.0f * (end.x - start.x) / FPATTERN_SIZE, 1.0f * (end.y - start.y) / FPATTERN_SIZE)
            };
            return res;
        }

        TextureCoords TextureCoordsCalc::calcLineCoords(const FCoords &start, const FCoords &end, bool needAddOffset) {
            return calcLineCoords(0, start, end, needAddOffset);
        }

        FCoords TextureCoordsCalc::addOffset(const FCoords &coords) {
            FCoords res = coords;
            res.x += CORR_OFFSET;
            return res;
        }
    }
}