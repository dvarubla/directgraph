#include <util.h>
#include <misc.h>
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
                    res.intersectionInside = true;
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
                    res.intersectionInside = false;
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
                DCoords vect1 = p1 - p2;
                DCoords vect2 = p3 - p2;
                res.intersectionInside = (vect1.x * vect2.y - vect1.y * vect2.x >= -POLYGON_EPS);
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
            _texturedPolyline = textured;
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
                addToPolylineRes(
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

                addToPolylineRes(
                        prev, p2, p3,
                        twoLines,
                        true, false, false, true
                );
            }

            delete [] points;

            return _polyline;
        }

        void PolygonHelper::addToPolylineRes(
                const DCoords &p1, const DCoords &p2, const DCoords &p3,
                const PolygonHelper::TwoLines &twoLines,
                bool addFirstLine, bool rememberCoords, bool addSecondLine,
                bool addLastPoints
        ) {
            CoordVect &coords = _polyline.coords;
            CoordVect &texCoords = _polyline.texCoords;
            bool useXForTextureFirst = false, useXForTextureSecond = false;
            if(_texturedPolyline){
                useXForTextureFirst = std::abs(p1.x - p2.x) > std::abs(p1.y - p2.y);
                useXForTextureSecond = std::abs(p2.x - p3.x) > std::abs(p2.y - p3.y);
            }
            if(addFirstLine) {
                coords.push_back(twoLines.lineFirst[0]);
                coords.push_back(twoLines.lineFirst[1]);
                if(_texturedPolyline) {
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

                if(_texturedPolyline) {
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
                    if(_texturedPolyline) {
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
                if(_texturedPolyline) {
                    texCoords.push_back(_prevTexCrds);
                    texCoords.push_back(_prevTexCrds);
                }

                coords.push_back(twoLines.lineSecond[0]);
                coords.push_back(twoLines.lineSecond[1]);
            }
            if(addSecondLine) {
                coords.push_back(twoLines.lineSecond[2]);
                coords.push_back(twoLines.lineSecond[3]);
                if(_texturedPolyline) {
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
                if(_texturedPolyline) {
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

                DCoords _;
                if(
                        (
                                twoLines.needConn &&
                                getIntersection(genDCoords(twoLines.intSecond[2]), genDCoords(_startCoords2),
                                   genDCoords(twoLines.intSecond[3]), genDCoords(_startCoords1), _)
                        ) ||
                        (
                                !twoLines.needConn &&
                                getIntersection(genDCoords(twoLines.lineSecond[0]), genDCoords(_startCoords2),
                                    genDCoords(twoLines.lineSecond[1]), genDCoords(_startCoords1), _)
                        )
                ){
                    _polyline.coords.push_back(_startCoords1);
                    _polyline.coords.push_back(_startCoords2);
                } else {
                    _polyline.coords.push_back(_startCoords2);
                    _polyline.coords.push_back(_startCoords1);
                }
            }
        }

        PolygonHelper::PolygonHelper(LineHelper *lineHelper, TextureCoordsCalc *texCrdCalc)
        :_lineHelper(lineHelper), _texCrdCalc(texCrdCalc){

        }

        Polygon PolygonHelper::calcPolygon(PolygonHelper::CoordsList &pointsList, int_fast8_t sign, bool textured) {
            Polygon res;
            DCoords minCrds;
            if(textured) {
                minCrds.x = pointsList.begin()->x;
                minCrds.y = pointsList.begin()->y;
                for(PolygonHelper::CoordsList::iterator it = pointsList.begin(); it != pointsList.end(); ++it) {
                    if(it->x < minCrds.x){
                        minCrds.x = it->x;
                    }
                    if(it->y < minCrds.y){
                        minCrds.y = it->y;
                    }
                }
            }
            CoordsList::iterator curEl = pointsList.begin(), it;

            enum TrianglesShareEdge{
                NO_SHARE,
                SHARE1,
                SHARE2
            } trianglesShareEdge = NO_SHARE;
            DCoords prevPnt;
            uint_fast32_t index = 0;
            while(true){
                CoordsList::iterator curEl2 = getPolygonIter(curEl, 1, pointsList);
                CoordsList::iterator curEl3 = getPolygonIter(curEl2, 1, pointsList);
                DCoords vect1 = *curEl - *curEl2;
                DCoords vect2 = *curEl3 - *curEl2;
                bool nextPnt = true;
                double orientedArea = (vect1.x * vect2.y - vect1.y * vect2.x) * sign;
                if(pointsList.size() == 3 || orientedArea >= -POLYGON_EPS){
                    bool pointInside = false;
                    if(orientedArea > POLYGON_EPS) {
                        for (it = pointsList.begin(); it != pointsList.end(); ++it) {
                            if (it != curEl && it != curEl2 && it != curEl3 &&
                                isPointInsideTriangle(*curEl, *curEl2, *curEl3, *it)) {
                                pointInside = true;
                                break;
                            }
                        }
                    }
                    if(!pointInside){
                        index = 0;
                        int_fast8_t offset;
                        if(trianglesShareEdge != NO_SHARE){
                            if(trianglesShareEdge == SHARE1){
                                if(textured) {
                                    res.texCoords.push_back(_texCrdCalc->addOffset(_texCrdCalc->calcBarCoords(minCrds, *curEl).end));
                                }
                                res.coords.push_back(genFCoords(*curEl));
                                prevPnt = *curEl;
                                trianglesShareEdge = SHARE2;
                                offset = 0;
                            } else {
                                if(textured) {
                                    res.texCoords.push_back(_texCrdCalc->addOffset(_texCrdCalc->calcBarCoords(minCrds, *curEl3).end));
                                }
                                res.coords.push_back(genFCoords(*curEl3));
                                prevPnt = *curEl3;
                                offset = -1;
                                trianglesShareEdge = SHARE1;
                            }
                        } else {
                            if(res.coords.size() != 0){
                                res.coords.push_back(genFCoords(prevPnt));
                                res.coords.push_back(genFCoords(*curEl2));
                                if(textured) {
                                    res.texCoords.push_back(_texCrdCalc->addOffset(_texCrdCalc->calcBarCoords(minCrds, prevPnt).end));
                                    res.texCoords.push_back(_texCrdCalc->addOffset(_texCrdCalc->calcBarCoords(minCrds, *curEl2).end));
                                }
                            }
                            res.coords.push_back(genFCoords(*curEl2));
                            res.coords.push_back(genFCoords(*curEl));
                            res.coords.push_back(genFCoords(*curEl3));
                            if(textured) {
                                res.texCoords.push_back(_texCrdCalc->addOffset(_texCrdCalc->calcBarCoords(minCrds, *curEl2).end));
                                res.texCoords.push_back(_texCrdCalc->addOffset(_texCrdCalc->calcBarCoords(minCrds, *curEl).end));
                                res.texCoords.push_back(_texCrdCalc->addOffset(_texCrdCalc->calcBarCoords(minCrds, *curEl3).end));
                            }
                            prevPnt = *curEl3;
                            offset = -1;
                            trianglesShareEdge = SHARE1;
                        }
                        if(pointsList.size() == 3){
                            break;
                        }
                        pointsList.erase(curEl2);
                        if(offset != 0) {
                            curEl = getPolygonIter(curEl, offset, pointsList);
                        }
                        nextPnt = false;
                    }
                }
                if(nextPnt){
                    trianglesShareEdge = NO_SHARE;
                    curEl = curEl2;
                    index++;
                    if(index > pointsList.size()){
                        break;
                    }
                }
            }
            return res;
        }

        Polygon PolygonHelper::calcPolygon(uint_fast32_t numPoints, int32_t *points, bool textured) {
            bool haveExtraPoint = checkHaveExtraPoint(numPoints, points);
            uint_fast32_t numCoords = (numPoints - ((haveExtraPoint) ? 1 : 0)) * 2;
            CoordsList pointsList;
            for(uint_fast32_t i = 0; i < numCoords; i += 2) {
                DCoords point = {
                        addCorrOffset(static_cast<float>(points[i])),
                        addCorrOffset(static_cast<float>(points[i + 1]))
                };
                pointsList.push_back(point);
            }
            bool insideDir = getInsideDir(numPoints, points, haveExtraPoint);
            delete [] points;
            return calcPolygon(pointsList, static_cast<int_fast8_t>(insideDir ? -1 : 1), textured);
        }

        bool PolygonHelper::checkHaveExtraPoint(uint_fast32_t numPoints, int32_t *points) {
            return (points[0] == points[numPoints * 2 - 2] && points[1] == points[numPoints * 2 - 1]);
        }

        PolylinePolygon PolygonHelper::calcPolylinePolygon(
                uint_fast32_t numPoints, int32_t *points, uint_fast32_t thickness,
                bool texturedPolyline, bool texturedPolygon
        ) {
            bool haveExtraPoint = checkHaveExtraPoint(numPoints, points);
            PolylinePolygon res;
            CoordsList pointsList;
            _polyline.coords.clear();
            _polyline.texCoords.clear();
            _prevTexCrds = genFCoords(0, 0);
            _texturedPolyline = texturedPolyline;
            uint_fast32_t size = (numPoints - ((haveExtraPoint) ? 1 : 0)) * 2;

            bool insideDir = !getInsideDir(numPoints, points, haveExtraPoint);

            bool dontAddFirstLinePoint = false;
            DCoords prev = genDCoords(points[0], points[1]);
            for(uint_fast32_t i = 2; i < size; i += 2){
                DCoords p2 = genDCoords(points[i], points[i + 1]);
                DCoords p3;
                if(i == size - 2){
                    p3.x = points[0];
                    p3.y = points[1];
                } else {
                    p3.x = points[i + 2];
                    p3.y = points[i + 2 + 1];
                }
                TwoLines twoLines = calcTwoLines(
                        prev, p2, p3, thickness,
                        i != 2, false, false, false
                );
                if(twoLines.needConn) {
                    if (twoLines.intersectionInside ^ insideDir) {
                        if (i != 2 && !dontAddFirstLinePoint) {
                            pointsList.push_back(genDCoords(twoLines.lineFirst[1]));
                        }
                    } else {
                        if (i != 2 && !dontAddFirstLinePoint) {
                            pointsList.push_back(genDCoords(twoLines.lineFirst[0]));
                        }
                        pointsList.push_back(genDCoords(twoLines.intFirst[2]));
                        pointsList.push_back(genDCoords(twoLines.intFirst[3]));
                        pointsList.push_back(genDCoords(twoLines.intSecond[1]));
                    }
                    dontAddFirstLinePoint = false;
                } else {
                    if(i != 2) {
                        if (twoLines.intersectionInside ^ insideDir) {
                            pointsList.push_back(genDCoords(twoLines.lineFirst[1]));
                        } else {
                            pointsList.push_back(genDCoords(twoLines.lineFirst[0]));
                        }
                    }
                }
                addToPolylineRes(
                        prev, p2, p3,
                        twoLines,
                        i != 2,
                        i == 2,
                        false,
                        false
                );
                prev = twoLines.newLineStart;
            }

            DCoords p2 = genDCoords(points[0], points[1]);
            DCoords p3 = genDCoords(points[2], points[3]);
            TwoLines twoLines = calcTwoLines(
                    prev, p2, p3, thickness, true, false, false, false
            );
            addToPolylineRes(
                    prev, p2, p3,
                    twoLines,
                    true, false, false, true
            );
            if(twoLines.needConn) {
                if (twoLines.intersectionInside ^ insideDir) {
                    pointsList.push_back(genDCoords(twoLines.lineFirst[1]));
                    pointsList.push_back(genDCoords(twoLines.lineFirst[3]));
                } else {
                    pointsList.push_back(genDCoords(twoLines.lineFirst[0]));
                    pointsList.push_back(genDCoords(twoLines.lineFirst[2]));
                    pointsList.push_back(genDCoords(twoLines.intFirst[2]));
                    pointsList.push_back(genDCoords(twoLines.intFirst[3]));
                    pointsList.push_back(genDCoords(twoLines.intSecond[1]));
                }
            } else {
                if(twoLines.intersectionInside ^ insideDir){
                    pointsList.push_back(genDCoords(twoLines.lineFirst[1]));
                    pointsList.push_back(genDCoords(twoLines.lineFirst[3]));
                } else {
                    pointsList.push_back(genDCoords(twoLines.lineFirst[0]));
                    pointsList.push_back(genDCoords(twoLines.lineFirst[2]));
                }
            }
            res.polyline = _polyline;
            res.polygon = calcPolygon(pointsList, static_cast<int_fast8_t>(!insideDir ? -1 : 1), texturedPolygon);
            delete [] points;
            return res;
        }

        bool PolygonHelper::getInsideDir(uint_fast32_t numPoints, int32_t *points, bool haveExtraPoint) {
            uint_fast32_t size = (numPoints - ((haveExtraPoint) ? 1 : 0)) * 2;
            uint_fast32_t minInd = 0;
            for(uint_fast32_t i = 2; i < size; i += 2){
                if(points[minInd] > points[i] || (points[minInd] == points[i] && points[minInd + 1] > points[i + 1])){
                    minInd = i;
                }
            }
            uint_fast32_t nextInd = (minInd == size - 2) ? 0 : minInd + 2;
            uint_fast32_t prevInd = (minInd == 0) ? size - 2 : minInd - 2;
            return ((points[prevInd] - points[minInd]) * (points[nextInd + 1] - points[minInd + 1]) -
                    (points[prevInd + 1] - points[minInd + 1]) * (points[nextInd] - points[minInd])) < 0;
        }

        bool PolygonHelper::isPointInsideTriangle(const DCoords &p1, const DCoords &p2, const DCoords &p3,
                                                  const DCoords &testP) {
            double val1 = (p1.x - testP.x) * (p2.y - p1.y) - (p1.y - testP.y) * (p2.x - p1.x);
            double val2 = (p2.x - testP.x) * (p3.y - p2.y) - (p2.y - testP.y) * (p3.x - p2.x);
            double val3 = (p3.x - testP.x) * (p1.y - p3.y) - (p3.y - testP.y) * (p1.x - p3.x);
            return (val1 <= POLYGON_EPS && val2 <= POLYGON_EPS && val3 <= POLYGON_EPS) ||
                   (val1 >= -POLYGON_EPS && val2 >= -POLYGON_EPS && val3 >= -POLYGON_EPS);
        }

        PolygonHelper::CoordsList::iterator PolygonHelper::getPolygonIter(
                const CoordsList::iterator &it, int_fast32_t offset, CoordsList &list
        ) {
            CoordsList::iterator res = it;
            int_fast32_t sign = (offset > 0) ? 1 : -1;
            for(int_fast32_t i = 0; i != offset; i += sign){
                if(sign == 1){
                    ++res;
                    if(res == list.end()){
                        res = list.begin();
                    }
                } else {
                    if(res == list.begin()){
                        res = list.end();
                    }
                    --res;
                }
            }
            return res;
        }

        const double PolygonHelper::POLYGON_EPS = 0.0000001;
    }
}
