#include <algorithm>
#include "RectangleHelper.h"
#include "VertexCreator.h"
#include "misc.h"

#undef min
#undef max

namespace directgraph {
    namespace dx9 {

        void* RectangleHelper::genRectangle(void *verticesVoid, const Coords &startCrds, const Coords &endCrds,
                                        uint_fast32_t thickness, float z, uint_fast32_t color) {
            Coords outlineStartCoords = genCoords(startCrds.x - thickness / 2, startCrds.y - thickness / 2);
            Coords outlineEndCoords = genCoords(endCrds.x + thickness / 2, endCrds.y + thickness / 2);
            int_fast32_t topSepCrd = std::max<int_fast32_t>(
                    (startCrds.x + endCrds.x) / 2 + (startCrds.x + endCrds.x) % 2,
                    outlineEndCoords.x - thickness
            );
            int_fast32_t rightSepCrd = std::max<int_fast32_t>(
                    (startCrds.y + endCrds.y) / 2 + (startCrds.y + endCrds.y) % 2,
                    outlineEndCoords.y - thickness
            );
            int_fast32_t bottomSepCrd = std::min<int_fast32_t>(
                    (startCrds.x + endCrds.x) / 2,
                    outlineStartCoords.x + thickness
            );
            int_fast32_t leftSepCrd = std::min<int_fast32_t>(
                    (startCrds.y + endCrds.y) / 2 + (startCrds.y + endCrds.y) % 2,
                    outlineStartCoords.y + thickness
            );

            verticesVoid = _simplePrimHelper->genQuad(
                    verticesVoid,
                    genCoords(outlineStartCoords.x, leftSepCrd),
                    genCoords(topSepCrd, outlineStartCoords.y),
                    z, color
            );

            verticesVoid = _simplePrimHelper->genQuad(
                    verticesVoid,
                    genCoords(topSepCrd, outlineStartCoords.y),
                    genCoords(outlineEndCoords.x, rightSepCrd),
                    z, color
            );

            verticesVoid = _simplePrimHelper->genQuad(
                    verticesVoid,
                    genCoords(outlineEndCoords.x, rightSepCrd),
                    genCoords(bottomSepCrd, outlineEndCoords.y),
                    z, color
            );
            verticesVoid = _simplePrimHelper->genQuad(
                    verticesVoid,
                    genCoords(bottomSepCrd, outlineEndCoords.y),
                    genCoords(outlineStartCoords.x, leftSepCrd),
                    z, color
            );
            return verticesVoid;
        }

        StartEndCoords RectangleHelper::getCoords(const Coords &startCrds, const Coords &endCrds, uint_fast32_t thickness) {
            int_fast32_t leftSepCrd = std::min<int_fast32_t>(
                    (startCrds.y + endCrds.y) / 2 + (startCrds.y + endCrds.y) % 2,
                    startCrds.y + (thickness / 2 + thickness % 2)
            );
            StartEndCoords res;
            res.start = genCoords(startCrds.x  - thickness / 2, leftSepCrd);
            res.end = genCoords(startCrds.x - thickness / 2, leftSepCrd);
            return res;
        }

        RectangleHelper::RectangleHelper(SimplePrimHelper *simplePrimHelper) : _simplePrimHelper(simplePrimHelper){
        }
    }
}
