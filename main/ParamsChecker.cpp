#include "ParamsChecker.h"
#include "WException.h"
#include "util.h"

namespace directgraph{
    bool ParamsChecker::checkEllipse(int32_t , int32_t , uint32_t rx, uint32_t ry) {
        if(rx == 0 || ry == 0){
            return false;
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

    void ParamsChecker::checkFillStyle(uint_fast8_t fillstyle) {
        if(fillstyle < EMPTY_FILL || fillstyle > USER_FILL){
            THROW_EXC_CODE(WException, WRONG_DRAW_PARAM, L"Wrong fillstyle param: " + to_wstring(fillstyle));
        }
    }
}