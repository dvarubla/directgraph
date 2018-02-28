#pragma once

#include <stdint.h>

namespace directgraph{
    class IController{
    public:
        virtual void init() = 0;
        virtual void clear() = 0;
        virtual void bar(float left, float top, float right, float bottom) = 0;
        virtual void setfillstyle(uint_fast8_t fillStyle, uint_fast32_t color) = 0;
        virtual void fillellipse(float x, float y, float xradius, float yradius) = 0;
        virtual void setlinestyle(uint_fast8_t linestyle, uint_fast32_t pattern, uint_fast32_t thickness) = 0;
        virtual void setbgcolor(uint_fast32_t color) = 0;
        virtual void setfillpattern(const char *fillpattern, uint_fast32_t color) = 0;
        virtual void putpixel(int_fast32_t x, int_fast32_t y, uint_fast32_t color) = 0;
        virtual void repaint() = 0;
        virtual ~IController(){};
    };
}

