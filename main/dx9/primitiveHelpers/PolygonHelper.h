#pragma once

#include "LineHelper.h"

namespace directgraph {
    namespace dx9 {
        class PolygonHelper {
        private:
            struct TwoLines{
                bool needConn;
                QuadPointsArr intFirst;
                QuadPointsArr intSecond;
                QuadPointsArr lineFirst;
                QuadPointsArr lineSecond;
                DCoords newLineStart;
            };
            LineHelper _lineHelper;
            bool getIntersection(
                    const DCoords &start1, const DCoords &end1, const DCoords &start2, const DCoords &end2,
                    DCoords &intPoint
            );
            TwoLines calcTwoLines(
                    const DCoords &p1, const DCoords &p2, const DCoords &p3, uint_fast32_t thickness,
                    bool useFirst, bool useLast, bool addToFirst, bool addToLast
            );
            void addToRes(
                    CoordVect &vect, const PolygonHelper::TwoLines &twoLines,
                    FCoords &startCoords1, FCoords &startCoords2,
                    bool addFirstLine, bool rememberCoords, bool addSecondLine
            );
        public:
            CoordVect calcPolygonBorder(uint_fast32_t numPoints, int32_t *points, uint_fast32_t thickness);
        };
    }
}


