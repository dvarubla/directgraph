#include <util.h>
#include "Bar3DHelper.h"

namespace directgraph {
    namespace dx9 {

        Bar3DHelper::Bar3DHelper(SimplePrimHelper *simplePrimHelper, DegenerateHelper *degHelper, TextureCoordsCalc *texCrdCalc)
        :_simplePrimHelper(simplePrimHelper), _degHelper(degHelper), _texCrdCalc(texCrdCalc){

        }

        void *Bar3DHelper::genBar3D(void *verticesVoid, const Coords &startCrds, const Coords &endCrds,
                                    uint_fast32_t depth,
                                    uint_fast32_t thickness, float z, uint_fast32_t color, bool textured,
                                    bool haveTop, bool canCreateVertsWithExtra) {
            double dy = depth;
            int_fast32_t halfT = thickness / 2;
            float incThickness = static_cast<float>(thickness);
            double lineDepth = thickness;
            double lineDy = thickness;
            FCoords sideLeftBottom = genFCoords(endCrds.x, endCrds.y) + halfT;
            FCoords sideLeftTop = genFCoords(endCrds.x, startCrds.y) + genFCoords(halfT, -halfT - 0.5);
            FCoords sideRightTop = sideLeftTop + genFCoords(depth, -dy);
            FCoords sideRightBottom = sideLeftBottom + genFCoords(depth, -dy);
            
            FCoords sideTopMid = sideLeftTop + genFCoords(0, incThickness);
            FCoords sideRightMid = _texCrdCalc->addOffset(sideRightTop + genFCoords(-lineDepth, lineDy));
            FCoords sideBottomMid = sideRightBottom - genFCoords(0, incThickness);

            FCoords sideRightTopIns = sideRightMid + genFCoords(0, incThickness);
            FCoords sideRightBottomIns = sideBottomMid + genFCoords(-lineDepth, lineDy);
            FCoords sideLeftBottomIns = sideLeftBottom - genFCoords(0, incThickness);

            QuadPointsArr points;
            points[0] = subtHalfPixel(sideTopMid);
            points[1] = subtHalfPixel(sideLeftTop);
            points[2] = subtHalfPixel(sideRightTopIns);
            points[3] = subtHalfPixel(sideRightMid);
            float prev = 0;
            verticesVoid = genData(verticesVoid, static_cast<float>(points[3].x - points[1].x - thickness / 2.), prev, points,
                                   static_cast<float>(thickness) / 2, z, color, textured, false, canCreateVertsWithExtra);
            points[0] = subtHalfPixel(sideRightMid);
            points[1] = subtHalfPixel(sideRightTop);
            points[2] = subtHalfPixel(sideRightBottomIns);
            points[3] = subtHalfPixel(sideBottomMid);

            verticesVoid =  genData(verticesVoid,
                                    points[0].y - points[3].y, prev, points,
                                    points[1].y - points[0].y, z, color, textured, true, canCreateVertsWithExtra);
            points[0] = subtHalfPixel(sideBottomMid);
            points[1] = subtHalfPixel(sideRightBottom);
            points[2] = subtHalfPixel(sideLeftBottomIns);
            points[3] = subtHalfPixel(sideLeftBottom);
            verticesVoid = genData(verticesVoid, static_cast<float>(depth - thickness / 2.), prev, points,
                                   static_cast<float>(thickness / 2.),
                                   z, color, textured, false, canCreateVertsWithExtra);
            if(haveTop){
                FCoords topLeftBottom = genFCoords(startCrds.x, startCrds.y) - halfT;
                FCoords topLeftTop = topLeftBottom + genFCoords(static_cast<int_fast32_t>(depth), -dy);

                FCoords topLeftMid = topLeftTop + genFCoords(incThickness, 0);
                FCoords topBottomMid = topLeftBottom + genFCoords(incThickness, 0);
                FCoords topLeftIns = topLeftMid + genFCoords(-lineDepth, lineDy);
                prev = 0;
                if(canCreateVertsWithExtra || !textured) {
                    verticesVoid = _degHelper->genDegenerate(
                            verticesVoid, subtHalfPixel(sideLeftBottom), subtHalfPixel(sideRightMid), z
                    );
                } else {
                    verticesVoid = _degHelper->genTexDegenerate(
                            verticesVoid, subtHalfPixel(sideLeftBottom), subtHalfPixel(sideRightMid), z
                    );
                }

                points[0] = subtHalfPixel(sideRightMid);
                points[1] = subtHalfPixel(sideRightTop);
                points[2] = subtHalfPixel(topLeftIns);
                points[3] = subtHalfPixel(topLeftMid);
                verticesVoid = genData(verticesVoid, points[0].x - points[3].x, prev, points,
                                       points[1].x - points[0].x, z, color, textured, true, canCreateVertsWithExtra);

                points[0] = subtHalfPixel(topLeftMid);
                points[1] = subtHalfPixel(topLeftTop);
                points[2] = subtHalfPixel(topBottomMid);
                points[3] = subtHalfPixel(topLeftBottom);
                verticesVoid = genData(verticesVoid, static_cast<float>(depth - thickness / 2.), prev, points,
                                       static_cast<float>(thickness / 2.), z, color, textured, false, canCreateVertsWithExtra);
            }

            return verticesVoid;
        }

        StartEndCoords
        Bar3DHelper::getCoords(const Coords &startCrds, const Coords &endCrds, uint_fast32_t thickness, bool haveTop) {
            StartEndCoords res;
            int_fast32_t halfT = thickness / 2;
            res.start = subtHalfPixel(genFCoords(endCrds.x, startCrds.y) + genFCoords(halfT, -halfT - 0.5f) +
                                  genFCoords(0, thickness));
            if(haveTop){
                res.end = subtHalfPixel(genFCoords(startCrds.x, startCrds.y) - halfT);
            } else {
                res.end = subtHalfPixel(genFCoords(endCrds.x, endCrds.y) + halfT);
            }
            return res;
        }

        void *Bar3DHelper::genData(void *verticesVoid, float len, float &prev,
                                   const QuadPointsArr &points, float thickness, float z,
                                   uint_fast32_t color, bool textured, bool swapCoords, bool canCreateVertsWithExtra) {
            if(textured){
                float t1 = _texCrdCalc->calcLineCoords(prev, genFCoords(0,0), genFCoords(thickness,0), true).end.x;
                float t2 = _texCrdCalc->calcLineCoords(t1, genFCoords(0,0), genFCoords(len, 0), true).end.x;
                float t3 = _texCrdCalc->calcLineCoords(t2, genFCoords(0,0), genFCoords(thickness, 0), true).end.x;
                float coords[VERTICES_IN_QUAD];
                if(swapCoords) {
                    coords[0] = t1;
                    coords[1] = prev;
                    coords[2] = t3;
                    coords[3] = t2;
                    prev = t2;
                } else {
                    coords[0] = prev;
                    coords[1] = t1;
                    coords[2] = t2;
                    coords[3] = t3;
                    prev = t3;
                }
                if(canCreateVertsWithExtra) {
                    return _simplePrimHelper->genQuadExtra(
                            verticesVoid, points, coords, z, color
                    );
                } else {
                    return _simplePrimHelper->genTexColorQuad(
                            verticesVoid, points, coords, z, color
                    );
                }
            } else {
                return _simplePrimHelper->genQuad(
                        verticesVoid, points, z, color
                );
            }
        }
    }
}
