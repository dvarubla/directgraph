#pragma once

#include <stdint.h>
#include <directgraph/directgraph_constants.h>

namespace directgraph{
    class IController{
    public:
        virtual void init() = 0;
        virtual void clear() = 0;
        virtual void bar(int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom) = 0;
        virtual void rectangle(
                int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom,
                bool useColor = false, uint_fast32_t color = 0
        ) = 0;
        virtual void fillellipse(
                int_fast32_t x, int_fast32_t y, uint_fast32_t xradius, uint_fast32_t yradius,
                bool useColor = false, uint_fast32_t color = 0
        ) = 0;
        virtual void ellipse(
                int_fast32_t x, int_fast32_t y, uint_fast32_t xradius, uint_fast32_t yradius,
                bool useColor = false, uint_fast32_t color = 0
        ) = 0;
        virtual void bar3d(
                int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom,
                uint_fast32_t depth, bool haveTop, bool useColor = false, uint_fast32_t color = 0
        ) = 0;
        virtual void line(
                int_fast32_t startx, int_fast32_t starty, int_fast32_t endx, int_fast32_t endy,
                bool useColor = false, uint_fast32_t color = 0
        ) = 0;
        virtual void lineto(
                int_fast32_t x, int_fast32_t y,
                bool useColor = false, uint_fast32_t color = 0
        ) = 0;
        virtual void linerel(
                int_fast32_t x, int_fast32_t y,
                bool useColor = false, uint_fast32_t color = 0
        ) = 0;
        virtual void drawpoly(
                uint_fast32_t numPoints, const int32_t *points,
                bool useColor = false, uint_fast32_t color = 0
        ) = 0;
        virtual void fillpoly(
                uint_fast32_t numPoints, const int32_t *points,
                bool useColor = false, uint_fast32_t color = 0
        ) = 0;
        virtual void putpixel(
                int_fast32_t x, int_fast32_t y, uint_fast32_t color
        ) = 0;
        virtual void putpixel(
                int_fast32_t x, int_fast32_t y
        ) = 0;

        virtual void setfillstyle(uint_fast8_t fillStyle, uint_fast32_t color) = 0;
        virtual void setlinestyle(uint_fast8_t linestyle, uint_fast16_t pattern, uint_fast32_t thickness) = 0;
        virtual void setcolor(uint_fast32_t color) = 0;
        virtual void setbgcolor(uint_fast32_t color) = 0;
        virtual void setfillpattern(const char *fillpattern, uint_fast32_t color) = 0;
        virtual uint_fast32_t getcolor() = 0;
        virtual uint_fast32_t getbgcolor() = 0;
        virtual void getlinesettings (linesettingstype *lineinfo) = 0;
        virtual void getfillsettings(fillsettingstype *fillinfo) = 0;
        virtual void getfillpattern(char *pattern) = 0;
        virtual void moveto(int_fast32_t x, int_fast32_t y) = 0;
        virtual void moverel(int_fast32_t x, int_fast32_t y) = 0;
        virtual void initpalette(bool havePalette, uint_fast32_t size, bool fillFirstColors) = 0;
        virtual void setpalette(uint_fast32_t index, uint_fast32_t color) = 0;
        virtual void clearpalette() = 0;
        virtual int_fast32_t getx() = 0;
        virtual int_fast32_t gety() = 0;
        virtual uint_fast32_t getmaxx() = 0;
        virtual uint_fast32_t getmaxy() = 0;
        virtual void repaint() = 0;
        virtual void clearAndReset() = 0;
        virtual ~IController(){};
    };
}

