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
            LineHelper::LineData data1 = _lineHelper.calcPoints(p1.x, p1.y, p2.x, p2.y, thickness);
            LineHelper::LineData data2 = _lineHelper.calcPoints(p2.x, p2.y, p3.x, p3.y, thickness);
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
            FCoords off1 = genFCoords(_lineHelper.calcOffset(data1));
            FCoords off2 = genFCoords(_lineHelper.calcOffset(data2));
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
                res.newLineStart = intPoint + data2.normal * normSign * thickness/ 2;
                if(int2){
                    res.intFirst[0] = genFCoords(oppositePoint1);
                    res.intFirst[1] = genFCoords(intPoint);
                    res.intFirst[2] = genFCoords(arr1[1]);
                    res.intFirst[3] = genFCoords(midPoint);

                    if(useFirst) {
                        res.lineFirst[0] = genFCoords(arr1[0]);
                        res.lineFirst[1] = genFCoords(arr1[2]);
                        res.lineFirst[2] = genFCoords(oppositePoint1);
                        res.lineFirst[3] = genFCoords(intPoint);
                    }

                    res.intSecond[0] = genFCoords(midPoint);
                    res.intSecond[1] = genFCoords(arr2[0]);
                    res.intSecond[2] = genFCoords(intPoint);
                    res.intSecond[3] = genFCoords(oppositePoint2);

                    if(useLast) {
                        res.lineSecond[0] = genFCoords(intPoint);
                        res.lineSecond[1] = genFCoords(oppositePoint2);
                        res.lineSecond[2] = genFCoords(arr2[3]);
                        res.lineSecond[3] = genFCoords(arr2[1]);
                    }
                } else {
                    res.intFirst[0] = genFCoords(oppositePoint1);
                    res.intFirst[1] = genFCoords(intPoint);
                    res.intFirst[2] = genFCoords(arr1[3]);
                    res.intFirst[3] = genFCoords(midPoint);

                    if(useFirst) {
                        res.lineFirst[0] = genFCoords(arr1[2]);
                        res.lineFirst[1] = genFCoords(arr1[0]);
                        res.lineFirst[2] = genFCoords(oppositePoint1);
                        res.lineFirst[3] = genFCoords(intPoint);
                    }

                    res.intSecond[0] = genFCoords(midPoint);
                    res.intSecond[1] = genFCoords(arr2[2]);
                    res.intSecond[2] = genFCoords(intPoint);
                    res.intSecond[3] = genFCoords(oppositePoint2);

                    if(useLast) {
                        res.lineSecond[0] = genFCoords(intPoint);
                        res.lineSecond[1] = genFCoords(oppositePoint2);
                        res.lineSecond[2] = genFCoords(arr2[1]);
                        res.lineSecond[3] = genFCoords(arr2[3]);
                    }
                }
            } else {
                res.needConn = false;
                res.newLineStart = p2;
                res.lineFirst[0] = data1.points[0];
                res.lineFirst[1] = data1.points[2];
                res.lineFirst[2] = data1.points[1];
                res.lineFirst[3] = data1.points[3];
                res.lineSecond[0] = data2.points[0];
                res.lineSecond[1] = data2.points[2];
                res.lineSecond[2] = data2.points[1];
                res.lineSecond[3] = data2.points[3];
            }
            if(addToFirst){
                res.lineFirst[0] -= off1;
                res.lineFirst[1] -= off1;
            }
            if(addToLast){
                res.lineSecond[2] += off2;
                res.lineSecond[3] += off2;
            }
            return res;
        }

        CoordVect PolygonHelper::calcPolygonBorder(uint_fast32_t num, int32_t *points, uint_fast32_t thickness) {
            CoordVect res;
            uint_fast32_t size = (num - 1) * 2;
            bool isClosed = (points[0] == points[size] && points[1] == points[size + 1]);
            DCoords prev = genDCoords(points[0], points[1]);
            FCoords startCoords1, startCoords2;
            for(uint_fast32_t i = 2; i < size; i += 2){
                DCoords p2 = genDCoords(points[i], points[i + 1]);
                DCoords p3 = genDCoords(points[i + 2], points[i + 2 + 1]);
                TwoLines twoLines = calcTwoLines(
                        prev, p2, p3, thickness,
                        !isClosed || i != 2, i == size - 2 && !isClosed, i == 2 && !isClosed, i == size - 2 && !isClosed
                );
                addToRes(
                        res, twoLines, startCoords1, startCoords2,
                        !isClosed || i != 2,
                        isClosed && i == 2,
                        i == size - 2 && !isClosed
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
                        res, twoLines, startCoords1, startCoords2,
                        true, false, false
                );

                if(twoLines.needConn) {
                    res.push_back(startCoords2);
                    res.push_back(startCoords1);
                } else {
                    res.push_back(startCoords1);
                    res.push_back(startCoords2);
                }
            }

            delete [] points;

            return res;
        }

        void PolygonHelper::addToRes(
                CoordVect &vect, const PolygonHelper::TwoLines &twoLines,
                FCoords &startCoords1, FCoords &startCoords2,
                bool addFirstLine, bool rememberCoords, bool addSecondLine
        ) {
            if(addFirstLine) {
                vect.push_back(twoLines.lineFirst[0]);
                vect.push_back(twoLines.lineFirst[1]);
            }

            if(twoLines.needConn) {
                if(rememberCoords){
                    startCoords1 = twoLines.intFirst[0];
                    startCoords2 = twoLines.intFirst[1];
                }
                vect.push_back(twoLines.intFirst[0]);
                vect.push_back(twoLines.intFirst[1]);
                vect.push_back(twoLines.intFirst[2]);
                vect.push_back(twoLines.intFirst[3]);

                vect.push_back(twoLines.intSecond[0]);
                vect.push_back(twoLines.intSecond[1]);

                vect.push_back(twoLines.intSecond[0]);
                vect.push_back(twoLines.intSecond[1]);
                vect.push_back(twoLines.intSecond[2]);
                vect.push_back(twoLines.intSecond[3]);
            } else {
                if(rememberCoords){
                    startCoords1 = twoLines.lineFirst[2];
                    startCoords2 = twoLines.lineFirst[3];
                } else {
                    vect.push_back(twoLines.lineFirst[2]);
                    vect.push_back(twoLines.lineFirst[3]);
                    vect.push_back(twoLines.lineFirst[3]);
                    vect.push_back(twoLines.lineSecond[0]);
                }
                vect.push_back(twoLines.lineSecond[0]);
                vect.push_back(twoLines.lineSecond[1]);
            }
            if(addSecondLine) {
                vect.push_back(twoLines.lineSecond[2]);
                vect.push_back(twoLines.lineSecond[3]);
            }
        }
    }
}
