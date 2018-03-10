#pragma once

#include <stdint.h>
#include <cmath>
#include <string>
#include <sstream>

namespace directgraph{
    uint_fast32_t swap_color_transp(uint_fast32_t color);

    uint_fast32_t color_remove_alpha(uint_fast32_t color);

    bool color_has_alpha(uint_fast32_t color);

    uint_fast32_t swap_color(uint_fast32_t color);

    struct Rectangle{
        uint_fast32_t left;
        uint_fast32_t top;
        uint_fast32_t right;
        uint_fast32_t bottom;
    };

    template <typename T>
    struct Vect{
        T x;
        T y;
    };

    typedef Vect<int_fast32_t> Coords;
    typedef Vect<uint_fast32_t> UCoords;


    UCoords genUCoords(uint_fast32_t x, uint_fast32_t y);
    Coords genCoords(int_fast32_t x, int_fast32_t y);
    Coords genCoords(const UCoords &ucrds);

    template <typename T>
    T round(T val){
#if _MSC_VER > 1800
        return std::round(val);
#else
        return std::floor(val + (T)0.5);
#endif
    }

    template <typename T>
    T log2(T val){
#if _MSC_VER > 1800
        return std::log2(val);
#else
        return std::log(val) / std::log((T)2);
#endif
    }

    template <typename T>
    std::wstring to_wstring(const T &input){
        std::wstringstream strm;
        strm << input;
        return strm.str();
    }
}
