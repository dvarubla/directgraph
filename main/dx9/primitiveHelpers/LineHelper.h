#pragma once

#include <misc.h>

namespace directgraph {
    namespace dx9 {
        class LineHelper {
        private:
            QuadPointsArr _points;
        public:
            QuadPointsArr &getPoints();
            void calcPoints(int_fast32_t x1, int_fast32_t y1, int_fast32_t x2, int_fast32_t y2, uint_fast32_t thickness);
        };
    }
}


