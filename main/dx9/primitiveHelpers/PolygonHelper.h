#pragma once

#include <TextureCoordsCalc.h>
#include "LineHelper.h"

namespace directgraph {
    namespace dx9 {
        class PolygonHelper {
        public:
        private:
            struct TwoLines{
                bool needConn;
                QuadPointsArr intFirst;
                QuadPointsArr intSecond;
                QuadPointsArr lineFirst;
                QuadPointsArr lineSecond;
                DCoords newLineStart;
                DCoords oldLineEnd;
            };
            LineHelper *_lineHelper;
            TextureCoordsCalc *_texCrdCalc;

            Polyline _polyline;
            FCoords _startCoords1;
            FCoords _startCoords2;
            bool _textured;
            FCoords _prevTexCrds;
            bool getIntersection(
                    const DCoords &start1, const DCoords &end1, const DCoords &start2, const DCoords &end2,
                    DCoords &intPoint
            );
            TwoLines calcTwoLines(
                    const DCoords &p1, const DCoords &p2, const DCoords &p3, uint_fast32_t thickness,
                    bool useFirst, bool useLast, bool addToFirst, bool addToLast
            );
            void addToRes(
                    const DCoords &p1, const DCoords &p2, const DCoords &p3,
                    const PolygonHelper::TwoLines &twoLines,
                    bool addFirstLine, bool rememberCoords, bool addSecondLine, bool addLastPoints
            );
        public:
            PolygonHelper(LineHelper *lineHelper, TextureCoordsCalc *texCrdCalc);
            Polyline calcPolyline(
                    uint_fast32_t numPoints, int32_t *points, uint_fast32_t thickness,
                    bool textured
            );
        };
    }
}


