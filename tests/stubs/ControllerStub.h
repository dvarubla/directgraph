#pragma once

#include <IController.h>
#include <gmock/gmock.h>

namespace directgraph{
    class ControllerStub: public IController {
        MOCK_METHOD0(init, void ());
        MOCK_METHOD0(clear, void ());
        MOCK_METHOD4(bar, void (int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom));
        MOCK_METHOD4(rectangle, void (int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom));
        MOCK_METHOD2(setfillstyle, void (uint_fast8_t fillStyle, uint_fast32_t color));
        MOCK_METHOD2(setfillpattern, void(const char *fillpattern, uint_fast32_t color));
        MOCK_METHOD4(fillellipse, void(int_fast32_t x, int_fast32_t y, uint_fast32_t xradius, uint_fast32_t yradius));
        MOCK_METHOD3(setlinestyle, void(uint_fast8_t linestyle, uint_fast16_t pattern, uint_fast32_t thickness));
        MOCK_METHOD1(setbgcolor, void(uint_fast32_t color));
        MOCK_METHOD1(setcolor, void(uint_fast32_t color));
        MOCK_METHOD3(putpixel, void (int_fast32_t x, int_fast32_t y, uint_fast32_t color));
        MOCK_METHOD0(repaint, void ());
        MOCK_METHOD0(getcolor, uint_fast32_t());
        MOCK_METHOD0(getbgcolor, uint_fast32_t());
        MOCK_METHOD1(getlinesettings, void(linesettingstype *lineinfo));
        MOCK_METHOD1(getfillsettings, void(fillsettingstype *fillinfo));
        MOCK_METHOD1(getfillpattern, void(char *pattern));
        MOCK_METHOD4(line, void(int_fast32_t startx, int_fast32_t starty, int_fast32_t endx, int_fast32_t endy));
        MOCK_METHOD2(lineto, void(int_fast32_t x, int_fast32_t y));
        MOCK_METHOD2(linerel, void(int_fast32_t x, int_fast32_t y));
        MOCK_METHOD2(moveto, void(int_fast32_t x, int_fast32_t y));
        MOCK_METHOD2(moverel, void(int_fast32_t x, int_fast32_t y));
        MOCK_METHOD0(getx, int_fast32_t());
        MOCK_METHOD0(gety, int_fast32_t());
    };
}

