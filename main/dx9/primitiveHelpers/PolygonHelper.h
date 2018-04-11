#pragma once

#include <TextureCoordsCalc.h>
#include "LineHelper.h"
#include <list>

namespace directgraph {
    namespace dx9 {
        class PolygonHelper {
        public:
        private:
            const static double POLYGON_EPS;
            typedef std::list<DCoords> CoordsList;
            struct TwoLines{
                bool needConn;
                QuadPointsArr intFirst;
                QuadPointsArr intSecond;
                QuadPointsArr lineFirst;
                QuadPointsArr lineSecond;
                DCoords newLineStart;
                DCoords oldLineEnd;
                bool intersectionInside;
            };
            LineHelper *_lineHelper;
            TextureCoordsCalc *_texCrdCalc;

            Polyline _polyline;
            FCoords _startCoords1;
            FCoords _startCoords2;
            bool _texturedPolyline;
            FCoords _prevTexCrds;
            bool getIntersection(
                    const DCoords &start1, const DCoords &end1, const DCoords &start2, const DCoords &end2,
                    DCoords &intPoint
            );
            TwoLines calcTwoLines(
                    const DCoords &p1, const DCoords &p2, const DCoords &p3, uint_fast32_t thickness,
                    bool useFirst, bool useLast, bool addToFirst, bool addToLast
            );
            void addToPolylineRes(
                    const DCoords &p1, const DCoords &p2, const DCoords &p3,
                    const PolygonHelper::TwoLines &twoLines,
                    bool addFirstLine, bool rememberCoords, bool addSecondLine, bool addLastPoints
            );
            bool isPointInsideTriangle(const DCoords &p1, const DCoords &p2, const DCoords &p3, const DCoords &testP);
            PolygonHelper::CoordsList::iterator getPolygonIter(
                    const CoordsList::iterator &it, int_fast32_t offset, CoordsList &list
            );
            Polygon calcPolygon(PolygonHelper::CoordsList &pointsList, int_fast8_t sign, bool textured);
            bool getInsideDir(uint_fast32_t numPoints, int32_t *points, bool haveExtraPoint);
            bool checkHaveExtraPoint(uint_fast32_t numPoints, int32_t *points);
        public:
            PolygonHelper(LineHelper *lineHelper, TextureCoordsCalc *texCrdCalc);
            Polyline calcPolyline(
                    uint_fast32_t numPoints, int32_t *points, uint_fast32_t thickness,
                    bool textured
            );
            Polygon calcPolygon(
                    uint_fast32_t numPoints, int32_t *points, bool textured
            );
            PolylinePolygon calcPolylinePolygon(
                    uint_fast32_t numPoints, int32_t *points, uint_fast32_t thickness,
                    bool texturedPolyline, bool texturedPolygon
            );
        };
    }
}


