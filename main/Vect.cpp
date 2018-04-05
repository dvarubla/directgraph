#include "Vect.h"

#include "util.h"

namespace directgraph{
    UCoords genUCoords(uint_fast32_t x, uint_fast32_t y) {
        UCoords res = {x, y};
        return res;
    }

    Coords genCoords(int_fast32_t x, int_fast32_t y) {
        Coords res = {x, y};
        return res;
    }

    Coords genCoords(const UCoords &ucrds) {
        Coords res = {static_cast<int_fast32_t>(ucrds.x), static_cast<int_fast32_t>(ucrds.y)};
        return res;
    }

    Coords genCoords(const FCoords &fcrds) {
        Coords res = {static_cast<int_fast32_t>(round(fcrds.x)), static_cast<int_fast32_t>(round(fcrds.y))};
        return res;
    }

    DCoords genDCoords(const FCoords &fCoords) {
        DCoords res = {fCoords.x, fCoords.y};
        return res;
    }

    FCoords genFCoords(const DCoords &dCoords) {
        FCoords res = {static_cast<float>(dCoords.x), static_cast<float>(dCoords.y)};
        return res;
    }
}