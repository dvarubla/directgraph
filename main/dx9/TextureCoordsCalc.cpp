#include <main/patterns.h>
#include "TextureCoordsCalc.h"

namespace directgraph{
    namespace dx9{
        TextureCoords
        TextureCoordsCalc::calcLineCoords(const FCoords &prevEnd, const Coords &start, const Coords &end) {
            float _;
            FCoords texStart = {
                    std::modf(prevEnd.x, &_), 0
            };
            FCoords texEnd = {
                    1.0f * std::abs(end.x - start.x) / LPATTERN_SIZE + texStart.x,
                    1.0f * std::abs(end.y - start.y) / LPATTERN_SIZE
            };
            TextureCoords res = {texStart, texEnd};
            return res;
        }

        TextureCoords TextureCoordsCalc::calcBarCoords(const Coords &start, const Coords &end) {
            TextureCoords res = {
                    genFCoords(0, 0), genFCoords(1.0f * (end.x - start.x) / FPATTERN_SIZE, 1.0f * (end.y - start.y) / FPATTERN_SIZE)
            };
            return res;
        }
    }
}