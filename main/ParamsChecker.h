#pragma once

#include <stdint.h>

namespace directgraph {
    class ParamsChecker {
    private:
    public:
        ParamsChecker();
        bool checkEllipse(float x, float y, float rx, float ry);
        bool checkPixel(int_fast32_t x, int_fast32_t y, uint_fast32_t maxWidth, uint_fast32_t maxHeight);
        void checkFillStyle(uint_fast8_t fillstyle);
    };
}


