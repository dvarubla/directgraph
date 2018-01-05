#pragma once

namespace directgraph {
    class DPIConverter {
        double _mulX, _mulY;
    public:
        DPIConverter(float curDpiX, float curDpiY, float tarDpiX, float tarDpiY);
        float convertX(float dip);
        float convertY(float dip);
    };
}
