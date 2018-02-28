#pragma once

#include <IController.h>
#include <gmock/gmock.h>

namespace directgraph{
    class ControllerStub: public IController {
        MOCK_METHOD0(init, void ());
        MOCK_METHOD0(clear, void ());
        MOCK_METHOD4(bar, void (float left, float top, float right, float bottom));
        MOCK_METHOD2(setfillstyle, void (uint_fast8_t fillStyle, uint_fast32_t color));
        MOCK_METHOD2(setfillpattern, void(const char *fillpattern, uint_fast32_t color));
        MOCK_METHOD4(fillellipse, void(float x, float y, float xradius, float yradius));
        MOCK_METHOD3(setlinestyle, void(uint_fast8_t linestyle, uint_fast32_t pattern, uint_fast32_t thickness));
        MOCK_METHOD1(setbgcolor, void(uint_fast32_t color));
        MOCK_METHOD3(putpixel, void (int_fast32_t x, int_fast32_t y, uint_fast32_t color));
        MOCK_METHOD0(repaint, void ());
    };
}

