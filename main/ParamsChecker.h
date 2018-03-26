#pragma once

#include <stdint.h>

namespace directgraph {
    class ParamsChecker {
    private:
    public:
        ParamsChecker();
        bool checkEllipse(int32_t x, int32_t y, uint32_t rx, uint32_t ry);
        bool checkPixel(int_fast32_t x, int_fast32_t y, uint_fast32_t maxWidth, uint_fast32_t maxHeight);
        bool checkRectangle(int_fast32_t &left, int_fast32_t &top, int_fast32_t &right, int_fast32_t &bottom);
        bool checkBar(int_fast32_t &left, int_fast32_t &top, int_fast32_t &right, int_fast32_t &bottom);
        void checkFillStyle(uint_fast8_t fillstyle);
    };
}


