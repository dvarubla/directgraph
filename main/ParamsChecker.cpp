#include "ParamsChecker.h"
#include "WException.h"
#include "util.h"

namespace directgraph{
    bool ParamsChecker::checkEllipse(float , float , float rx, float ry) {
        if(rx == 0 || ry == 0){
            return false;
        }
        if(rx < 0){
            THROW_EXC_CODE(WException, WRONG_DRAW_PARAM, L"X radius " + to_wstring(rx) + L" must be greater than 0 ");
        }
        if(ry < 0){
            THROW_EXC_CODE(WException, WRONG_DRAW_PARAM, L"Y radius " + to_wstring(ry) + L" must be greater than 0 ");
        }
        return true;
    }

    ParamsChecker::ParamsChecker() {

    }

    bool ParamsChecker::checkPixel(int_fast32_t x, int_fast32_t y, uint_fast32_t maxWidth, uint_fast32_t maxHeight) {
        return !(
                x < 0 || y < 0 ||
                static_cast<uint_fast32_t>(x) >= maxWidth || static_cast<uint_fast32_t>(y) >= maxHeight
        );
    }
}