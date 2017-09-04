#ifndef DIRECTGRAPH_DPICONVERTER_H
#define DIRECTGRAPH_DPICONVERTER_H

namespace directgraph {
    class DPIConverter {
        double _mulX, _mulY;
    public:
        DPIConverter(float curDpiX, float curDpiY, float tarDpiX, float tarDpiY);
        float convertX(float dip);
        float convertY(float dip);
    };
}

#endif //DIRECTGRAPH_DPICONVERTER_H
