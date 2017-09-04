#ifndef DIRECTGRAPH_DPIHELPER_H
#define DIRECTGRAPH_DPIHELPER_H

#include <stdint.h>

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

        uint_fast32_t toPixelsX(float dip);

        uint_fast32_t toPixelsY(float dip);

        void setDpiX(float dpiX);

        void setDpiY(float dpiY);
    };

    void setDPIAware();
}
#endif //DIRECTGRAPH_DPIHELPER_H
