#include <algorithm>
#include <main/patterns.h>
#include "RectangleHelper.h"
#include <VertexCreator.h>
#include <util.h>

#undef min
#undef max

namespace directgraph {
    namespace dx9 {

        void* RectangleHelper::genRectangle(void *verticesVoid, const Coords &startCrds, const Coords &endCrds,
                                        uint_fast32_t thickness, float z, uint_fast32_t color, bool textured
        ) {
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
            if (textured) {
                TextureCoords texCrds = _texCrdCalc->calcLineCoords(
                        genFCoords(0, 0),
                        genFCoords(topSepCrd, outlineStartCoords.y),
                        genFCoords(outlineStartCoords.x, leftSepCrd)
                );
                verticesVoid = _simplePrimHelper->genTexColorQuad(
                        verticesVoid,
                        subtHalfPixel(genFCoords(outlineStartCoords.x, leftSepCrd)),
                        subtHalfPixel(genFCoords(topSepCrd, outlineStartCoords.y)),
                        z, color, texCrds, false
                );

                texCrds = _texCrdCalc->calcLineCoords(
                        texCrds.end,
                        genFCoords(outlineStartCoords.y, topSepCrd),
                        genFCoords(rightSepCrd, outlineEndCoords.x)
                );
                verticesVoid = _simplePrimHelper->genTexColorQuad(
                        verticesVoid,
                        subtHalfPixel(genFCoords(topSepCrd, outlineStartCoords.y)),
                        subtHalfPixel(genFCoords(outlineEndCoords.x, rightSepCrd)),
                        z, color, texCrds, true
                );

                texCrds = _texCrdCalc->calcLineCoords(
                        texCrds.end,
                        genFCoords(outlineEndCoords.x, rightSepCrd),
                        genFCoords(bottomSepCrd, outlineEndCoords.y)
                );
                verticesVoid = _simplePrimHelper->genTexColorQuad(
                        verticesVoid,
                        subtHalfPixel(genFCoords(outlineEndCoords.x, rightSepCrd)),
                        subtHalfPixel(genFCoords(bottomSepCrd, outlineEndCoords.y)),
                        z, color, texCrds, false
                );

                texCrds = _texCrdCalc->calcLineCoords(
                        texCrds.end,
                        genFCoords(leftSepCrd, outlineStartCoords.x),
                        genFCoords(outlineEndCoords.y, bottomSepCrd)
                );
                verticesVoid = _simplePrimHelper->genTexColorQuad(
                        verticesVoid,
                        subtHalfPixel(genFCoords(bottomSepCrd, outlineEndCoords.y)),
                        subtHalfPixel(genFCoords(outlineStartCoords.x, leftSepCrd)),
                        z, color, texCrds, true
                );
            } else {
                verticesVoid = _simplePrimHelper->genQuad(
                        verticesVoid,
                        subtHalfPixel(genFCoords(outlineStartCoords.x, leftSepCrd)),
                        subtHalfPixel(genFCoords(topSepCrd, outlineStartCoords.y)),
                        z, color
                );
                verticesVoid = _simplePrimHelper->genQuad(
                        verticesVoid,
                        subtHalfPixel(genFCoords(topSepCrd, outlineStartCoords.y)),
                        subtHalfPixel(genFCoords(outlineEndCoords.x, rightSepCrd)),
                        z, color
                );
                verticesVoid = _simplePrimHelper->genQuad(
                        verticesVoid,
                        subtHalfPixel(genFCoords(outlineEndCoords.x, rightSepCrd)),
                        subtHalfPixel(genFCoords(bottomSepCrd, outlineEndCoords.y)),
                        z, color
                );
                verticesVoid = _simplePrimHelper->genQuad(
                        verticesVoid,
                        subtHalfPixel(genFCoords(bottomSepCrd, outlineEndCoords.y)),
                        subtHalfPixel(genFCoords(outlineStartCoords.x, leftSepCrd)),
                        z, color
                );
            }
            return verticesVoid;
        }

        StartEndCoords RectangleHelper::getCoords(const Coords &startCrds, const Coords &endCrds, uint_fast32_t thickness) {
            int_fast32_t leftSepCrd = std::min<int_fast32_t>(
                    (startCrds.y + endCrds.y) / 2 + (startCrds.y + endCrds.y) % 2,
                    startCrds.y + (thickness / 2 + thickness % 2)
            );
            StartEndCoords res;
            res.start = subtHalfPixel(genFCoords(startCrds.x  - thickness / 2, leftSepCrd));
            res.end = subtHalfPixel(genFCoords(startCrds.x - thickness / 2, leftSepCrd));
            return res;
        }

        RectangleHelper::RectangleHelper(
                SimplePrimHelper *simplePrimHelper, TextureCoordsCalc *texCrdCalc
        ) : _simplePrimHelper(simplePrimHelper), _texCrdCalc(texCrdCalc){
        }
    }
}
