#pragma once

#include <stdint.h>
#include "util.h"

namespace directgraph {
    class DPIHelper {
    private:
        float _dpiX;
        float _dpiY;
    public:
        const static float DEFAULT_DPIX;
        const static float DEFAULT_DPIY;

        DPIHelper();

        ~DPIHelper();

        DPIHelper(float dpiX, float dpiY);

        int_fast32_t toPixelsX(float dip);

        int_fast32_t toPixelsY(float dip);

        Coords toPixelsXY(float dipX, float dipY);
        UCoords toPixelsXYU(float dipX, float dipY);

        void setDpiX(float dpiX);

        void setDpiY(float dpiY);
    };

    void setDPIAware();
}
