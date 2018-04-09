#include <util.h>
#include "PolygonHelper.h"

namespace directgraph{
    namespace dx9{

        bool PolygonHelper::getIntersection(
                const DCoords &start1, const DCoords &end1, const DCoords &start2, const DCoords &end2,
                DCoords &intPoint
        ) {
            DCoords dir1 = end1 - start1;
            DCoords dir2 = end2 - start2;

            double a1 = -dir1.y;
            double b1 = dir1.x;
            double d1 = -(a1*start1.x + b1*start1.y);

            double a2 = -dir2.y;
            double b2 = dir2.x;
            double d2 = -(a2*start2.x + b2*start2.y);

            double seg1Line2Start = a2*start1.x + b2*start1.y + d2;
            double seg1Line2End = a2*end1.x + b2*end1.y + d2;

            double seg2Line1Start = a1*start2.x + b1*start2.y + d1;
            double seg2Line1End = a1*end2.x + b1*end2.y + d1;

            if(seg1Line2Start * seg1Line2End >= 0 || seg2Line1Start * seg2Line1End >= 0) {
                return false;
            }

            double u = seg1Line2Start / (seg1Line2Start - seg1Line2End);
            intPoint = start1 + dir1 * u;
            return true;
        }

        PolygonHelper::TwoLines
        PolygonHelper::calcTwoLines(
                const DCoords &p1, const DCoords &p2, const DCoords &p3, uint_fast32_t thickness,
                bool useFirst, bool useLast, bool addToFirst, bool addToLast
        ) {
            LineHelper::LineData data1 = _lineHelper->calcPoints(p1.x, p1.y, p2.x, p2.y, thickness);
            LineHelper::LineData data2 = _lineHelper->calcPoints(p2.x, p2.y, p3.x, p3.y, thickness);
            DCoords arr1[VERTICES_IN_QUAD] = {
                    genDCoords(data1.points[0]), genDCoords(data1.points[1]),
                    genDCoords(data1.points[2]), genDCoords(data1.points[3])
            };
            DCoords arr2[VERTICES_IN_QUAD] = {
                    genDCoords(data2.points[0]), genDCoords(data2.points[1]),
                    genDCoords(data2.points[2]), genDCoords(data2.points[3])
            };
            DCoords intPoint1, intPoint2;
            bool int1 = getIntersection(arr1[0], arr1[1], arr2[0], arr2[1], intPoint1);
            bool int2 = getIntersection(arr1[2], arr1[3], arr2[2], arr2[3], intPoint2);
            FCoords off1 = genFCoords(_lineHelper->calcOffset(data1));
            FCoords off2 = genFCoords(_lineHelper->calcOffset(data2));
            TwoLines res;
            if(int1 ^ int2){
                res.needConn = true;
                DCoords midPoint, intPoint, oppositePoint1, oppositePoint2;
                double normSign;
                if(int2){
                    normSign = 1;
                    midPoint = (arr1[1] + arr2[0]) / 2;
                    intPoint = intPoint2;
                } else {
                    midPoint = (arr1[3] + arr2[2]) / 2;
                    normSign = -1;
                    intPoint = intPoint1;
                }
                oppositePoint1 = intPoint + data1.normal * normSign * thickness;
                oppositePoint2 = intPoint + data2.normal * normSign * thickness;
                res.oldLineEnd = (intPoint + oppositePoint1) / 2;
                res.newLineStart = (intPoint + oppositePoint2) / 2;
                if(int2){
                    res.intFirst[0] = addCorrOffset(genFCoords(oppositePoint1));
                    res.intFirst[1] = addCorrOffset(genFCoords(intPoint));
                    res.intFirst[2] = addCorrOffset(genFCoords(arr1[1]));
                    res.intFirst[3] = addCorrOffset(genFCoords(midPoint));

                    if(useFirst) {
                        res.lineFirst[0] = genFCoords(arr1[0]);
                        res.lineFirst[1] = genFCoords(arr1[2]);
                        res.lineFirst[2] = genFCoords(oppositePoint1);
                        res.lineFirst[3] = genFCoords(intPoint);
                    }

                    res.intSecond[0] = addCorrOffset(genFCoords(midPoint));
                    res.intSecond[1] = addCorrOffset(genFCoords(arr2[0]));
                    res.intSecond[2] = addCorrOffset(genFCoords(intPoint));
                    res.intSecond[3] = addCorrOffset(genFCoords(oppositePoint2));

                    if(useLast) {
                        res.lineSecond[0] = genFCoords(intPoint);
                        res.lineSecond[1] = genFCoords(oppositePoint2);
                        res.lineSecond[2] = genFCoords(arr2[3]);
                        res.lineSecond[3] = genFCoords(arr2[1]);
                    }
                } else {
                    res.intFirst[0] = addCorrOffset(genFCoords(oppositePoint1));
                    res.intFirst[1] = addCorrOffset(genFCoords(intPoint));
                    res.intFirst[2] = addCorrOffset(genFCoords(arr1[3]));
                    res.intFirst[3] = addCorrOffset(genFCoords(midPoint));

                    if(useFirst) {
                        res.lineFirst[0] = genFCoords(arr1[2]);
                        res.lineFirst[1] = genFCoords(arr1[0]);
                        res.lineFirst[2] = genFCoords(oppositePoint1);
                        res.lineFirst[3] = genFCoords(intPoint);
                    }

                    res.intSecond[0] = addCorrOffset(genFCoords(midPoint));
                    res.intSecond[1] = addCorrOffset(genFCoords(arr2[2]));
                    res.intSecond[2] = addCorrOffset(genFCoords(intPoint));
                    res.intSecond[3] = addCorrOffset(genFCoords(oppositePoint2));

                    if(useLast) {
                        res.lineSecond[0] = genFCoords(intPoint);
                        res.lineSecond[1] = genFCoords(oppositePoint2);
                        res.lineSecond[2] = genFCoords(arr2[1]);
                        res.lineSecond[3] = genFCoords(arr2[3]);
                    }
                }
            } else {
                res.needConn = false;
                res.oldLineEnd = p2;
                res.newLineStart = p2;
                res.lineFirst[0] = data1.points[0];
                res.lineFirst[1] = data1.points[2];
                res.lineFirst[2] = data1.points[1];
                res.lineFirst[3] = data1.points[3];
                res.lineSecond[0] = data2.points[0];
                res.lineSecond[1] = data2.points[2];
                res.lineSecond[2] = data2.points[1];
                res.lineSecond[3] = data2.points[3];
                useFirst = true;
                useLast = true;
            }
            if(addToFirst){
                res.lineFirst[0] -= off1;
                res.lineFirst[1] -= off1;
            }
            if(addToLast){
                res.lineSecond[2] += off2;
                res.lineSecond[3] += off2;
            }
            if(useFirst){
                res.lineFirst[0] = addCorrOffset(res.lineFirst[0]);
                res.lineFirst[1] = addCorrOffset(res.lineFirst[1]);
                res.lineFirst[2] = addCorrOffset(res.lineFirst[2]);
                res.lineFirst[3] = addCorrOffset(res.lineFirst[3]);
            }
            if(useLast){
                res.lineSecond[0] = addCorrOffset(res.lineSecond[0]);
                res.lineSecond[1] = addCorrOffset(res.lineSecond[1]);
                res.lineSecond[2] = addCorrOffset(res.lineSecond[2]);
                res.lineSecond[3] = addCorrOffset(res.lineSecond[3]);
            }
            return res;
        }

        Polyline PolygonHelper::calcPolyline(uint_fast32_t num, int32_t *points, uint_fast32_t thickness,
                                              bool textured) {
            _polyline.coords.clear();
            _polyline.texCoords.clear();
            _prevTexCrds = genFCoords(0, 0);
            _textured = textured;
            uint_fast32_t size = (num - 1) * 2;
            bool isClosed = (points[0] == points[size] && points[1] == points[size + 1]);
            DCoords prev = genDCoords(points[0], points[1]);
            for(uint_fast32_t i = 2; i < size; i += 2){
                DCoords p2 = genDCoords(points[i], points[i + 1]);
                DCoords p3 = genDCoords(points[i + 2], points[i + 2 + 1]);
                TwoLines twoLines = calcTwoLines(
                        prev, p2, p3, thickness,
                        !isClosed || i != 2, i == size - 2 && !isClosed, i == 2 && !isClosed, i == size - 2 && !isClosed
                );
                addToRes(
                        prev, p2, p3,
                        twoLines,
                        !isClosed || i != 2,
                        isClosed && i == 2,
                        i == size - 2 && !isClosed,
                        false
                );
                prev = twoLines.newLineStart;
            }

            if(isClosed){
                DCoords p2 = genDCoords(points[0], points[1]);
                DCoords p3 = genDCoords(points[2], points[3]);
                TwoLines twoLines = calcTwoLines(
                        prev, p2, p3, thickness, true, false, false, false
                );
                addToRes(
                        prev, p2, p3,
                        twoLines,
                        true, false, false, true
                );
            }

            delete [] points;

            return _polyline;
        }

        void PolygonHelper::addToRes(
                const DCoords &p1, const DCoords &p2, const DCoords &p3,
                const PolygonHelper::TwoLines &twoLines,
                bool addFirstLine, bool rememberCoords, bool addSecondLine,
                bool addLastPoints
        ) {
            CoordVect &coords = _polyline.coords;
            CoordVect &texCoords = _polyline.texCoords;
            bool useXForTextureFirst = false, useXForTextureSecond = false;
            if(_textured){
                useXForTextureFirst = std::abs(p1.x - p2.x) > std::abs(p1.y - p2.y);
                useXForTextureSecond = std::abs(p2.x - p3.x) > std::abs(p2.y - p3.y);
            }
            if(addFirstLine) {
                coords.push_back(twoLines.lineFirst[0]);
                coords.push_back(twoLines.lineFirst[1]);
                if(_textured) {
                    _prevTexCrds = _texCrdCalc->addOffset(_prevTexCrds);
                    texCoords.push_back(_prevTexCrds);
                    texCoords.push_back(_prevTexCrds);
                    FCoords mid = (twoLines.lineFirst[0] + twoLines.lineFirst[1]) / 2;

                    _prevTexCrds = _texCrdCalc->calcLineCoords(
                            _prevTexCrds.x,
                            genFCoords(0,0),
                            (useXForTextureFirst) ?
                            genFCoords(std::abs(mid.x - twoLines.oldLineEnd.x), 0) :
                            genFCoords(std::abs(mid.y - twoLines.oldLineEnd.y), 0),
                            true
                    ).end;
                }
            }

            if(twoLines.needConn) {
                if(rememberCoords){
                    _startCoords1 = twoLines.intFirst[0];
                    _startCoords2 = twoLines.intFirst[1];
                }
                coords.push_back(twoLines.intFirst[0]);
                coords.push_back(twoLines.intFirst[1]);
                coords.push_back(twoLines.intFirst[2]);
                coords.push_back(twoLines.intFirst[3]);

                coords.push_back(twoLines.intSecond[0]);
                coords.push_back(twoLines.intSecond[1]);

                coords.push_back(twoLines.intSecond[0]);
                coords.push_back(twoLines.intSecond[1]);
                coords.push_back(twoLines.intSecond[2]);
                coords.push_back(twoLines.intSecond[3]);

                if(_textured) {
                    texCoords.push_back(_prevTexCrds);
                    texCoords.push_back(_prevTexCrds);

                    FCoords diff1 = genFCoords(p2) - genFCoords(twoLines.oldLineEnd);

                    _prevTexCrds = _texCrdCalc->calcLineCoords(
                            _prevTexCrds.x,
                            genFCoords(0, 0),
                            (useXForTextureFirst) ?
                            genFCoords(std::abs(diff1.x), 0) :
                            genFCoords(std::abs(diff1.y), 0),
                            true
                    ).end;

                    texCoords.push_back(_prevTexCrds);

                    DCoords vect1 = genDCoords(twoLines.intFirst[3]) - p2;
                    DCoords vect2 = p1 - p2;
                    double dotProd = vect1.x * vect2.x + vect1.y * vect2.y;
                    DCoords proj =  vect2 / (vect2.x * vect2.x + vect2.y * vect2.y) * dotProd + p2;

                    FCoords diff2 = genFCoords(proj) - genFCoords(p2);

                    _prevTexCrds = _texCrdCalc->calcLineCoords(
                            _prevTexCrds.x,
                            genFCoords(0, 0),
                            (useXForTextureFirst) ?
                            genFCoords(std::abs(diff2.x), 0) :
                            genFCoords(std::abs(diff2.y), 0),
                            true
                    ).end;

                    texCoords.push_back(_prevTexCrds);

                    texCoords.push_back(_prevTexCrds);
                    FCoords t = _prevTexCrds;

                    vect1 = genDCoords(twoLines.intSecond[0]) - p2;
                    vect2 = p2 - p3;
                    dotProd = vect1.x * vect2.x + vect1.y * vect2.y;
                    proj =  vect2 / (vect2.x * vect2.x + vect2.y * vect2.y) * dotProd + p2;

                    diff2 = genFCoords(proj) - genFCoords(p2);

                    _prevTexCrds = _texCrdCalc->calcLineCoords(
                            _prevTexCrds.x,
                            genFCoords(0, 0),
                            (useXForTextureSecond) ?
                            genFCoords(std::abs(diff2.x), 0) :
                            genFCoords(std::abs(diff2.y), 0),
                            true
                    ).end;

                    texCoords.push_back(_prevTexCrds);
                    texCoords.push_back(t);
                    texCoords.push_back(_prevTexCrds);

                    diff1 = genFCoords(p2) - genFCoords(twoLines.newLineStart);

                    _prevTexCrds = _texCrdCalc->calcLineCoords(
                            _prevTexCrds.x,
                            genFCoords(0, 0),
                            (useXForTextureSecond) ?
                            genFCoords(std::abs(diff1.x), 0) :
                            genFCoords(std::abs(diff1.y), 0),
                            true
                    ).end;

                    texCoords.push_back(_prevTexCrds);
                    texCoords.push_back(_prevTexCrds);
                }
            } else {
                if(rememberCoords){
                    _startCoords1 = twoLines.lineFirst[2];
                    _startCoords2 = twoLines.lineFirst[3];
                }
                if(addFirstLine){
                    if(_textured) {
                        texCoords.push_back(_prevTexCrds);
                        texCoords.push_back(_prevTexCrds);
                        texCoords.push_back(_prevTexCrds);
                        texCoords.push_back(_prevTexCrds);
                    }
                    coords.push_back(twoLines.lineFirst[2]);
                    coords.push_back(twoLines.lineFirst[3]);
                    coords.push_back(twoLines.lineFirst[3]);

                    coords.push_back(twoLines.lineSecond[0]);
                }
                if(_textured) {
                    texCoords.push_back(_prevTexCrds);
                    texCoords.push_back(_prevTexCrds);
                }

                coords.push_back(twoLines.lineSecond[0]);
                coords.push_back(twoLines.lineSecond[1]);
            }
            if(addSecondLine) {
                coords.push_back(twoLines.lineSecond[2]);
                coords.push_back(twoLines.lineSecond[3]);
                if(_textured) {
                    FCoords mid = (twoLines.lineSecond[2] + twoLines.lineSecond[3]) / 2;

                    _prevTexCrds = _texCrdCalc->calcLineCoords(
                            _prevTexCrds.x,
                            genFCoords(0, 0),
                            (useXForTextureSecond) ?
                            genFCoords(std::abs(mid.x - twoLines.newLineStart.x), 0) :
                            genFCoords(std::abs(mid.y - twoLines.newLineStart.y), 0)
                    ).end;

                    texCoords.push_back(_prevTexCrds);
                    texCoords.push_back(_prevTexCrds);
                }
            }
            if(addLastPoints) {
                if(_textured) {
                    FCoords mid = (_startCoords2 + _startCoords1) / 2;
                    _prevTexCrds = _texCrdCalc->calcLineCoords(
                            _prevTexCrds.x,
                            genFCoords(0, 0),
                            (useXForTextureSecond) ?
                            genFCoords(std::abs(mid.x - twoLines.newLineStart.x), 0) :
                            genFCoords(std::abs(mid.y - twoLines.newLineStart.y), 0)
                    ).end;
                    texCoords.push_back(_prevTexCrds);
                    texCoords.push_back(_prevTexCrds);
                }
                if (twoLines.needConn) {
                    _polyline.coords.push_back(_startCoords2);
                    _polyline.coords.push_back(_startCoords1);
                } else {
                    _polyline.coords.push_back(_startCoords1);
                    _polyline.coords.push_back(_startCoords2);
                }
            }
        }

        PolygonHelper::PolygonHelper(LineHelper *lineHelper, TextureCoordsCalc *texCrdCalc)
        :_lineHelper(lineHelper), _texCrdCalc(texCrdCalc){

        }
    }
}
