#pragma once

#include <misc.h>

namespace directgraph {
    namespace dx9 {
        class LineHelper {
        public:
            struct LineData{
                QuadPointsArr points;
                DCoords normal;
                DCoords dir;
                double len;
                double halfT;
            };
            struct PointsLen{
                QuadPointsArr points;
                double len;
            };
        public:
            LineData calcPoints(double x1d, double y1d, double x2d, double y2d, uint_fast32_t thickness);
            PointsLen getPointsLen(double x1d, double y1d, double x2d, double y2d, uint_fast32_t thickness);
            DCoords calcOffset(const LineData &lineData);
        };
    }
}


