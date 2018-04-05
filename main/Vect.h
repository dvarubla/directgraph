#pragma once

#include <stdint.h>

namespace directgraph{
    template <typename T>
    struct Vect{
        T x;
        T y;
    };

    template <typename T>
    Vect<T>& operator+=(Vect<T> &l, const Vect<T> &r){
        l.x += r.x;
        l.y += r.y;
        return l;
    }

    template <typename T>
    Vect<T>& operator-=(Vect<T> &l, const Vect<T> &r){
        l.x -= r.x;
        l.y -= r.y;
        return l;
    }

    template <typename T>
    Vect<T>& operator*=(Vect<T> &l, const Vect<T> &r){
        l.x *= r.x;
        l.y *= r.y;
        return l;
    }

    template <typename T>
    Vect<T>& operator/=(Vect<T> &l, const Vect<T> &r){
        l.x /= r.x;
        l.y /= r.y;
        return l;
    }

    template <typename T, typename T2>
    Vect<T>& operator+=(Vect<T> &l, const T2 &r){
        l.x += static_cast<T>(r);
        l.y += static_cast<T>(r);
        return l;
    }

    template <typename T, typename T2>
    Vect<T>& operator-=(Vect<T> &l, const T2 &r){
        l.x -= static_cast<T>(r);
        l.y -= static_cast<T>(r);
        return l;
    }

    template <typename T, typename T2>
    Vect<T>& operator*=(Vect<T> &l, const T2 &r){
        l.x *= static_cast<T>(r);
        l.y *= static_cast<T>(r);
        return l;
    }

    template <typename T, typename T2>
    Vect<T>& operator/=(Vect<T> &l, const T2 &r){
        l.x /= static_cast<T>(r);
        l.y /= static_cast<T>(r);
        return l;
    }

    template <typename T, typename T2>
    Vect<T> operator+(const Vect<T> &l, const T2 &r){
        Vect<T> res = l;
        res += r;
        return res;
    }

    template <typename T, typename T2>
    Vect<T> operator-(const Vect<T> &l, const T2 &r){
        Vect<T> res = l;
        res -= r;
        return res;
    }

    template <typename T, typename T2>
    Vect<T> operator*(const Vect<T> &l, const T2 &r){
        Vect<T> res = l;
        res *= r;
        return res;
    }

    template <typename T, typename T2>
    Vect<T> operator/(const Vect<T> &l, const T2 &r){
        Vect<T> res = l;
        res /= r;
        return res;
    }

    typedef Vect<int_fast32_t> Coords;
    typedef Vect<uint_fast32_t> UCoords;
    typedef Vect<float> FCoords;
    typedef Vect<double> DCoords;

    UCoords genUCoords(uint_fast32_t x, uint_fast32_t y);
    Coords genCoords(int_fast32_t x, int_fast32_t y);
    Coords genCoords(const FCoords &fcrds);
    Coords genCoords(const UCoords &ucrds);

    template <typename T1, typename T2>
    FCoords genFCoords(T1 x,  T2 y){
        FCoords res = {static_cast<float>(x), static_cast<float>(y)};
        return res;
    }

    FCoords genFCoords(const DCoords &dCoords);

    template <typename T1, typename T2>
    DCoords genDCoords(T1 x,  T2 y){
        DCoords res = {static_cast<float>(x), static_cast<float>(y)};
        return res;
    }

    DCoords genDCoords(const FCoords &fCoords);
}

