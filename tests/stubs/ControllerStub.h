#pragma once

#include <IController.h>
#include <gmock/gmock.h>

namespace directgraph{
    class ControllerStub: public IController {
        MOCK_METHOD0(init, void ());
        MOCK_METHOD0(clear, void ());
        MOCK_METHOD4(bar, void (float left, float top, float right, float bottom));
        MOCK_METHOD2(setfillstyle, void (uint_fast8_t fillStyle, uint_fast32_t color));
        MOCK_METHOD3(putpixel, void (int_fast32_t x, int_fast32_t y, uint_fast32_t color));
        MOCK_METHOD0(repaint, void ());
    };
}
