#include "DPIConverter.h"

namespace directgraph {

    DPIConverter::DPIConverter(float curDpiX, float curDpiY, float tarDpiX, float tarDpiY)
            : _mulX(curDpiX / tarDpiX), _mulY(curDpiY / tarDpiY) {
    }

    float DPIConverter::convertX(float dip) {
        return static_cast<float>(dip * _mulX);
    }

    float DPIConverter::convertY(float dip) {
        return static_cast<float>(dip * _mulY);
    }
}
