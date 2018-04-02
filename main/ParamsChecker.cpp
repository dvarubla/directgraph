#include "ParamsChecker.h"
#include "WException.h"
#include "util.h"
#include <algorithm>

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
        if(fillstyle > USER_FILL){
            THROW_EXC_CODE(WException, WRONG_DRAW_PARAM, L"Wrong fillstyle param: " + to_wstring(fillstyle));
        }
    }

    void ParamsChecker::checkLineStyle(uint_fast8_t linestyle) {
        if(linestyle > NULL_LINE){
            THROW_EXC_CODE(WException, WRONG_DRAW_PARAM, L"Wrong linestyle param: " + to_wstring(linestyle));
        }
    }

    bool ParamsChecker::checkRectangle(int_fast32_t &left, int_fast32_t &top, int_fast32_t &right, int_fast32_t &bottom, uint_fast8_t linestyle) {
        if(linestyle == NULL_LINE){
            return false;
        }
        if(right == left || top == bottom){
            return false;
        }
        if(left > right){
            std::swap(left, right);
        }
        if(top > bottom){
            std::swap(top, bottom);
        }
        return true;
    }

    bool ParamsChecker::checkBar(int_fast32_t &left, int_fast32_t &top, int_fast32_t &right, int_fast32_t &bottom) {
        if(right == left || top == bottom){
            return false;
        }
        if(left > right){
            std::swap(left, right);
        }
        if(top > bottom){
            std::swap(top, bottom);
        }
        return true;
    }
}