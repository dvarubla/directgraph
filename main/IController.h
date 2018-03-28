#pragma once

#include <stdint.h>
#include <directgraph/directgraph_constants.h>

namespace directgraph{
    class IController{
    public:
        virtual void init() = 0;
        virtual void clear() = 0;
        virtual void bar(int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom) = 0;
        virtual void rectangle(int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom) = 0;
        virtual void setfillstyle(uint_fast8_t fillStyle, uint_fast32_t color) = 0;
        virtual void fillellipse(int_fast32_t x, int_fast32_t y, uint_fast32_t xradius, uint_fast32_t yradius) = 0;
        virtual void setlinestyle(uint_fast8_t linestyle, uint_fast16_t pattern, uint_fast32_t thickness) = 0;
        virtual void setcolor(uint_fast32_t color) = 0;
        virtual void setbgcolor(uint_fast32_t color) = 0;
        virtual void setfillpattern(const char *fillpattern, uint_fast32_t color) = 0;
        virtual uint_fast32_t getcolor() = 0;
        virtual uint_fast32_t getbgcolor() = 0;
        virtual void getlinesettings (linesettingstype *lineinfo) = 0;
        virtual void getfillsettings(fillsettingstype *fillinfo) = 0;
        virtual void getfillpattern(char *pattern) = 0;
        virtual void putpixel(int_fast32_t x, int_fast32_t y, uint_fast32_t color) = 0;
        virtual void repaint() = 0;
        virtual ~IController(){};
    };
}

